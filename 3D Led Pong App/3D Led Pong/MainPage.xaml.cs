using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Devices.Bluetooth.Rfcomm;
using Windows.Devices.Enumeration;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Networking.Sockets;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using System.Diagnostics;
using System.Threading.Tasks;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Led_Pong_App
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {

        private RfcommDeviceService dataService = null;
        private DeviceInformationCollection dataServiceDeviceCollection = null;
        //private bool IsConnected = false;

        public MainPage()
        {
            this.InitializeComponent();
            panelDisconnect.Visibility = Visibility.Collapsed;
            StatusPanel.Visibility = Visibility.Collapsed;
        }

        public void start()
        {
            dataService = null;
            dataServiceDeviceCollection = null;
            panelDisconnect.Visibility = Visibility.Collapsed;
            StatusPanel.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// Used to display messages to the user
        /// </summary>
        /// <param name="strMessage"></param>
        /// <param name="type"></param>
        public void NotifyUser(string strMessage, NotifyType type)
        {
            switch (type)
            {
                case NotifyType.StatusMessage:
                    StatusBorder.Background = new SolidColorBrush(Windows.UI.Colors.Green);
                    break;
                case NotifyType.ErrorMessage:
                    StatusBorder.Background = new SolidColorBrush(Windows.UI.Colors.Red);
                    break;
            }
            StatusBlock.Text = strMessage;

            // Collapse the StatusBlock if it has no text to conserve real estate.
            StatusBorder.Visibility = (StatusBlock.Text != String.Empty) ? Visibility.Visible : Visibility.Collapsed;
            if (StatusBlock.Text != String.Empty)
            {
                StatusBorder.Visibility = Visibility.Visible;
                StatusPanel.Visibility = Visibility.Visible;
            }
            else
            {
                StatusBorder.Visibility = Visibility.Collapsed;
                StatusPanel.Visibility = Visibility.Collapsed;
            }
        }
        public enum NotifyType
        {
            StatusMessage,
            ErrorMessage
        };

        private async void btnListPrinters_Click(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            // Disable the button while we do async operations so the user can't Run twice.
            button.IsEnabled = false;

            // Clear any previous messages
            NotifyUser("", NotifyType.StatusMessage);

            // Find all paired instances of the Rfcomm service and display them in a list
            dataServiceDeviceCollection = await DeviceInformation.FindAllAsync(RfcommDeviceService.GetDeviceSelector(RfcommServiceId.SerialPort));

            if (dataServiceDeviceCollection.Count > 0)
            {
                DeviceList.Items.Clear();
                foreach (var dataServiceDevice in dataServiceDeviceCollection)
                {
                    DeviceList.Items.Add(dataServiceDevice.Name);
                }
                DeviceList.Visibility = Windows.UI.Xaml.Visibility.Visible;
            }
            else
            {
                NotifyUser(
                    "No SPP services were found. Please pair with a device that is advertising the SPP service.",
                    NotifyType.ErrorMessage);
            }
            button.IsEnabled = true;

        }

        private async void DeviceList_Tapped(object sender, TappedRoutedEventArgs e)
        {
            btnListPrinters.IsEnabled = false;
            DeviceList.Visibility = Windows.UI.Xaml.Visibility.Collapsed;

            var dataServiceDevice = dataServiceDeviceCollection[DeviceList.SelectedIndex];
            App.dataServiceDevice = dataServiceDevice;
            dataService = await RfcommDeviceService.FromIdAsync(dataServiceDevice.Id);

            Debug.WriteLine("After FromIdAsync");

            if (dataService == null)
            {
                NotifyUser(
                    "Access to the device is denied because the application was not granted access",
                    NotifyType.StatusMessage);
                return;
            }

            App.Stream = new StreamSocket();
            try
            {
                await App.Stream.ConnectAsync(dataService.ConnectionHostName, dataService.ConnectionServiceName);
            }
            catch
            {
                //return false;
            }

            //IsConnected = true;
            App._btReader = new DataReader(App.Stream.InputStream);
            App._btWriter = new DataWriter(App.Stream.OutputStream);
            App._btWriter.UnicodeEncoding = UnicodeEncoding.Utf8;
            App._btReader.UnicodeEncoding = UnicodeEncoding.Utf8;

            //App._btReader.InputStreamOptions = InputStreamOptions.Partial;

            Debug.WriteLine("Finished");

            var n = App._btWriter.WriteString("b\n");
            await App._btWriter.StoreAsync();
            n = App._btWriter.WriteString("b\n");
            await App._btWriter.StoreAsync();
            n = App._btWriter.WriteString("b\n");
            await App._btWriter.StoreAsync();
            n = App._btWriter.WriteString("b\n");
            await App._btWriter.StoreAsync();

            //this.Frame.Navigate(typeof(ControllerPage));
            this.Frame.Navigate(typeof(ServerMenuPage));

            Debug.WriteLine("After Frame.Navigate");

        }

    }
}