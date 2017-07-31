using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;

using Windows.Storage;

using Windows.UI.Core;

using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Networking.Proximity;
using System.Diagnostics;
using Windows.Networking.Sockets;
using Windows.Storage.Streams;

using Windows.Devices.Bluetooth.Rfcomm;
using System.Threading;
using System.Threading.Tasks;
using Windows.Devices.Enumeration;

using Windows.System.Threading;

using Microsoft.WindowsAzure.MobileServices;



// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Led_Pong_App
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    /// 
    

    public sealed partial class ControllerPage : Page
    {
        StreamSocket Stream;
        private bool IsConnected = false;
        DataReader _btReader;
        DataWriter _btWriter;
        CancellationTokenSource cts;

        private IMobileServiceTable<RunningGameItem> gameTable = App.MobileService.GetTable<RunningGameItem>();
        private IMobileServiceTable<PlayerItem> playersTable = App.MobileService.GetTable<PlayerItem>();

        public async Task<DeviceInformationCollection> FindPairedDevicesAsync()
        {
            var aqsDevices = RfcommDeviceService.GetDeviceSelector(RfcommServiceId.SerialPort);
            DeviceInformationCollection devices = await DeviceInformation.FindAllAsync(aqsDevices);
            return devices;
        }

        public async Task<bool> ConnectAsync(DeviceInformation device)
        {
            //RfcommDeviceService rfcommService = await RfcommDeviceService.FromIdAsync(device.Id);
            RfcommDeviceService rfcommService;
            try
            {
                rfcommService = await RfcommDeviceService.FromIdAsync(device.Id);
                if (rfcommService == null) return false;
                Stream = new StreamSocket();
                try
                {
                    await Stream.ConnectAsync(rfcommService.ConnectionHostName, rfcommService.ConnectionServiceName);
                }
                catch
                {
                    return false;
                }

                IsConnected = true;
                _btReader = new DataReader(Stream.InputStream);
                _btWriter = new DataWriter(Stream.OutputStream);
                _btWriter.UnicodeEncoding = UnicodeEncoding.Utf8;
            }

            catch (Exception ex)
            {
            }
            
            return true;
        }

        public ControllerPage()
        {
            InitializeComponent();
            App.enableX = true;

            if (/*App.controllerInit == false*/ true)
            {
                App.controllerInit = true;
                TimeSpan period = TimeSpan.FromSeconds(5);

                ThreadPoolTimer PeriodicTimer = ThreadPoolTimer.CreatePeriodicTimer((source) =>
               {
                    //
                    // TODO: Work
                    //

                    //
                    // Update the UI thread by using the UI core dispatcher.
                    //
                    Dispatcher.RunAsync(CoreDispatcherPriority.High,
                       async  () =>
                       {
                            //
                            // UI components can be accessed within this scope.
                            //

                            {
                               if (App.playing == true)
                               {
                                   try
                                   {
                                       App.game = await gameTable.LookupAsync(App.gameID);
                                   }
                                   catch (System.Exception ex)
                                   {

                                   }
                                   PlayerName1.Text = App.game.Player1;
                                   if (App.game.Player1 != "")
                                   {
                                       PlayerPoints1.Text = App.game.PointsPlayer1.ToString();
                                   }
                                   PlayerName2.Text = App.game.Player2;
                                   if (App.game.Player2 != "")
                                   {
                                       PlayerPoints2.Text = App.game.PointsPlayer2.ToString();
                                   }
                                   PlayerName3.Text = App.game.Player3;
                                   if (App.game.Player3 != "")
                                   {
                                       PlayerPoints3.Text = App.game.PointsPlayer3.ToString();
                                   }
                                   PlayerName4.Text = App.game.Player4;
                                   if (App.game.Player4 != "")
                                   {
                                       PlayerPoints4.Text = App.game.PointsPlayer4.ToString();
                                   }
                               }
                           }

                       });

               }, period);
            }
        }

        private async void btRx()
        {
            App.btRxStarted = true;
            try
            {
                // Based on the protocol we've defined, the first uint is the size of the message
                uint readLength = await App._btReader.LoadAsync(1);
                char readChar = (char)App._btReader.ReadByte();
                //char dummy = (char)App._btReader.ReadByte();
                Debug.WriteLine("char recieved: " + readChar);

                while (readChar != 'x' && readChar != 'w')
                {
                    readLength = await App._btReader.LoadAsync(1);
                    readChar = (char)App._btReader.ReadByte();
                    //char dummy = (char)App._btReader.ReadByte();
                    Debug.WriteLine("char recieved: " + readChar);
                }

                if (App.enableX == true)
                {
                    ButtonStart.IsEnabled = true;
                    if (readChar == 'x')
                    {
                        var PlayMusic = new MediaElement();
                        StorageFolder Folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                        Folder = await Folder.GetFolderAsync("Sound");
                        StorageFile sf = await Folder.GetFileAsync("Pacman-lost.mp3");
                        PlayMusic.SetSource(await sf.OpenAsync(FileAccessMode.Read), sf.ContentType);
                        PlayMusic.Play();

                        if (App.playerNum != 1 && App.game.Player1 != "" && App.game.PlayerQuit1 == false)
                        {
                            App.game.PointsPlayer1++;
                        }

                        if (App.playerNum != 2 && App.game.Player2 != "" && App.game.PlayerQuit2 == false)
                        {
                            App.game.PointsPlayer2++;
                        }

                        if (App.playerNum != 3 && App.game.Player3 != "" && App.game.PlayerQuit3 == false)
                        {
                            App.game.PointsPlayer3++;
                        }

                        if (App.playerNum != 4 && App.game.Player4 != "" && App.game.PlayerQuit4 == false)
                        {
                            App.game.PointsPlayer4++;
                        }

                        try
                        {
                            await gameTable.UpdateAsync(App.game);
                        }
                        catch (System.Net.Http.HttpRequestException ex)
                        {
                            ContentDialog deleteFileDialog = new ContentDialog
                            {
                                FontSize = 10,
                                Title = "Network Error",
                                PrimaryButtonText = "Home"
                            };

                            ContentDialogResult result = await deleteFileDialog.ShowAsync();
                            this.Frame.Navigate(typeof(ServerMenuPage));
                        }


                        this.Frame.Navigate(typeof(LostPage));
                    }
                    else if (readChar=='w') // readChar=='w'
                    {
                        var PlayMusic = new MediaElement();
                        StorageFolder Folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                        Folder = await Folder.GetFolderAsync("Sound");
                        StorageFile sf = await Folder.GetFileAsync("win.mp3");
                        PlayMusic.SetSource(await sf.OpenAsync(FileAccessMode.Read), sf.ContentType);
                        PlayMusic.Play();

                        this.Frame.Navigate(typeof(WinPage));
                    }

                    readChar = 'a';


                }

            }
            // Catch exception HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED).
            catch (Exception ex) when ((uint)ex.HResult == 0x800703E3)
            {
                Debug.Write("Client Disconnected Successfully");
                //break;
            }

            App.btRxStarted = false;
        }

        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            App.enableX = true;
            if (App.btRxStarted==false)
            {
                btRx();
            }

            {
                App.game = await gameTable.LookupAsync(App.gameID);
                PlayerName1.Text = App.game.Player1;
                if (App.game.Player1 != "")
                {
                    PlayerPoints1.Text = App.game.PointsPlayer1.ToString();
                }
                PlayerName2.Text = App.game.Player2;
                if (App.game.Player2 != "")
                {
                    PlayerPoints2.Text = App.game.PointsPlayer2.ToString();
                }
                PlayerName3.Text = App.game.Player3;
                if (App.game.Player3 != "")
                {
                    PlayerPoints3.Text = App.game.PointsPlayer3.ToString();
                }
                PlayerName4.Text = App.game.Player4;
                if (App.game.Player4 != "")
                {
                    PlayerPoints4.Text = App.game.PointsPlayer4.ToString();
                }
            }
        }

        /*
        protected async override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            App.enableX = true;

            try
            {
                // Based on the protocol we've defined, the first uint is the size of the message
                uint readLength = await App._btReader.LoadAsync(1);
                char readChar = (char)App._btReader.ReadByte();
                //char dummy = (char)App._btReader.ReadByte();
                Debug.WriteLine("char recieved: " + readChar);

                while (readChar!='x')
                {
                    readLength = await App._btReader.LoadAsync(1);
                    readChar = (char)App._btReader.ReadByte();
                    //char dummy = (char)App._btReader.ReadByte();
                    Debug.WriteLine("char recieved: " + readChar);
                }

                if (App.enableX == true)
                {
                    if (App.playerNum != 1 && App.game.Player1 != "" && App.game.PlayerQuit1 == false)
                    {
                        App.game.PointsPlayer1++;
                    }

                    if (App.playerNum != 2 && App.game.Player2 != "" && App.game.PlayerQuit2 == false)
                    {
                        App.game.PointsPlayer2++;
                    }

                    if (App.playerNum != 3 && App.game.Player3 != "" && App.game.PlayerQuit3 == false)
                    {
                        App.game.PointsPlayer3++;
                    }

                    if (App.playerNum != 4 && App.game.Player4 != "" && App.game.PlayerQuit4 == false)
                    {
                        App.game.PointsPlayer4++;
                    }

                    await gameTable.UpdateAsync(App.game);

                    this.Frame.Navigate(typeof(LostPage));
                }

            }
            // Catch exception HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED).
            catch (Exception ex) when ((uint)ex.HResult == 0x800703E3)
            {
                Debug.Write("Client Disconnected Successfully");
                //break;
            }
            

        }
        */


        //
        // TODO: Work
        //

        /*

        TimeSpan period = TimeSpan.FromSeconds(5);

        ThreadPoolTimer PeriodicTimer = ThreadPoolTimer.CreatePeriodicTimer((source) =>
        {
            //
            // TODO: Work
            //

            //
            // Update the UI thread by using the UI core dispatcher.
            //
            Dispatcher.RunAsync(CoreDispatcherPriority.High,
                   () =>
                {
                    //
                    // UI components can be accessed within this scope.
                    //

                    /*
                    var length =  App._btReader.ReadInt32();
                    var rxString =  App._btReader.ReadString((uint)length);
                    Debug.WriteLine(rxString);
                    */

        /*
        string rxString;
        var bytesRecieved = await App._btReader.LoadAsync(128);
        while (_btReader.UnconsumedBufferLength > 0)
        {
            uint bytesToRead = _btReader.ReadUInt32();
            receivedStrings += _btReader.ReadString(bytesToRead) + "\n";
        }


        var rxString = ReadAsync();
        Debug.WriteLine("String recieved: " + rxString);


        //await App._btWriter.StoreAsync();

    });

}, period);

*/

        public async Task<String> ReadAsync()
        {
            var receivedStrings = "";
            try
            {
                
                uint size = await App._btReader.LoadAsync(sizeof(uint));

                /*
                if (size < sizeof(uint))
                {
                    Disconnect();
                    return null;
                }
                */
                

                while (App._btReader.UnconsumedBufferLength > 0)
                {
                    uint bytesToRead = App._btReader.ReadUInt32();
                    receivedStrings += App._btReader.ReadString(bytesToRead) + "\n";
                }

                return receivedStrings;
            }
            catch (Exception ex)
            {
                return null;
            }
        }


        private void ThumbStick_OnNewPosition(object sender, string e)
        {
            if (IsConnected)
            {
                //Write(e);
            }
        }
        

        private void ThumbStick_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void ThumbStick_Loaded_1(object sender, RoutedEventArgs e)
        {

        }

        private void ThumbStick_Loaded_2(object sender, RoutedEventArgs e)
        {

        }

        private async void ButtonStart_Click(object sender, RoutedEventArgs e)
        {
            ButtonStart.IsEnabled = false;
            var PlayMusic = new MediaElement();
            StorageFolder Folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
            Folder = await Folder.GetFolderAsync("Sound");
            StorageFile sf = await Folder.GetFileAsync("start.mp3");
            PlayMusic.SetSource(await sf.OpenAsync(FileAccessMode.Read), sf.ContentType);
            PlayMusic.Play();

            await Task.Delay(2500);

            var n = App._btWriter.WriteString("s\n");
            await App._btWriter.StoreAsync();

            if (App.game.GameStarted == false)
            {
                App.game.GameStarted = true;
                await gameTable.UpdateAsync(App.game);
            }

            /*
            MediaElement PlayMusic = new MediaElement();
            PlayMusic.AudioCategory = Windows.UI.Xaml.Media.AudioCategory.Media;
            PlayMusic.Source = new Uri("start.mp3");
            PlayMusic.Play();
            */

            

            /*
            //var rxString = ReadAsync();
            var rxString = "";
            Debug.WriteLine("UnconsumedBufferLength= " + App._btReader.UnconsumedBufferLength.ToString() );
            while (App._btReader.UnconsumedBufferLength > 0)
            {
                // Note that the call to readString requires a length of "code units" 
                // to read. This is the reason each string is preceded by its length 
                // when "on the wire".
                uint bytesToRead = App._btReader.ReadUInt32();
                rxString += App._btReader.ReadString(bytesToRead) + "\n";
            }
            */



            /*
                try
                {
                    Debug.WriteLine("1");
                    // Based on the protocol we've defined, the first uint is the size of the message
                    uint readLength = await App._btReader.LoadAsync(1);
                    char readChar = (char)App._btReader.ReadByte();
                    //char dummy = (char)App._btReader.ReadByte();
                    Debug.WriteLine("char recieved: "+ readChar);

                }
                // Catch exception HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED).
                catch (Exception ex) when ((uint)ex.HResult == 0x800703E3)
                {
                    Debug.Write("Client Disconnected Successfully");
                    //break;
                }

    */


        }

        private async void DisplayDeleteFileDialog()
        {
            ContentDialog deleteFileDialog = new ContentDialog
            {
                Title = "Delete file permanently?",
                Content = "If you delete this file, you won't be able to recover it. Do you want to delete it?",
                PrimaryButtonText = "Delete",
                SecondaryButtonText = "Cancel"
            };

            ContentDialogResult result = await deleteFileDialog.ShowAsync();

            // Delete the file if the user clicked the primary button.
            /// Otherwise, do nothing.
            if (result == ContentDialogResult.Primary)
            {
                // Delete the file.
            }
            else
            {
                // The user clicked the CLoseButton, pressed ESC, Gamepad B, or the system back button.
                // Do nothing.
            }
        }

        private async void ButtonRestart_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog deleteFileDialog = new ContentDialog
            {
                Title = "Are you sure you want to restart the game?",
                Content = "If you restart this game, all points will be initialized to 0",
                PrimaryButtonText = "Restart",
                SecondaryButtonText = "Cancel"
            };

            ContentDialogResult result = await deleteFileDialog.ShowAsync();

            // Delete the file if the user clicked the primary button.
            /// Otherwise, do nothing.
            if (result == ContentDialogResult.Primary)
            {
                var n = App._btWriter.WriteString("r\n");
                await App._btWriter.StoreAsync();
                App.game.PointsPlayer1 = App.game.PointsPlayer2 = App.game.PointsPlayer3 = App.game.PointsPlayer4 = 0;
                try
                {
                    await gameTable.UpdateAsync(App.game);
                }
                catch (System.Net.Http.HttpRequestException ex)
                {
                    ContentDialog deleteFileDialog2 = new ContentDialog
                    {
                        FontSize = 10,
                        Title = "Network Error",
                        PrimaryButtonText = "Home"
                    };

                    ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                    this.Frame.Navigate(typeof(ServerMenuPage));
                }
                
                PlayerName1.Text = App.game.Player1;
                PlayerPoints1.Text = App.game.PointsPlayer1.ToString();
                PlayerName2.Text = App.game.Player2;
                if (App.game.Player2 != "")
                {
                    PlayerPoints2.Text = App.game.PointsPlayer2.ToString();
                }
                PlayerName3.Text = App.game.Player3;
                if (App.game.Player3 != "")
                {
                    PlayerPoints3.Text = App.game.PointsPlayer3.ToString();
                }
                PlayerName4.Text = App.game.Player4;
                if (App.game.Player4 != "")
                {
                    PlayerPoints4.Text = App.game.PointsPlayer4.ToString();
                }
            }
            else
            {
                // The user clicked the CLoseButton, pressed ESC, Gamepad B, or the system back button.
                // Do nothing.
            }
            

        }

        private async void ButtonQuit_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog deleteFileDialog = new ContentDialog
            {
                Title = "Are you sure you want to quit the game?",
                PrimaryButtonText = "Quit",
                SecondaryButtonText = "Cancel"
            };

            ContentDialogResult result = await deleteFileDialog.ShowAsync();

            // Delete the file if the user clicked the primary button.
            /// Otherwise, do nothing.
            if (result == ContentDialogResult.Primary)
            {
                App.enableX = false;
                App.playing = false;
                var n = App._btWriter.WriteString("q\n");
                await App._btWriter.StoreAsync();

                try
                {
                    App.game = await gameTable.LookupAsync(App.gameID);
                }
                catch (System.Net.Http.HttpRequestException ex)
                {
                    ContentDialog deleteFileDialog2 = new ContentDialog
                    {
                        FontSize = 10,
                        Title = "Network Error",
                        PrimaryButtonText = "Home"
                    };

                    ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                    this.Frame.Navigate(typeof(ServerMenuPage));
                }

                switch (App.dataServiceDevice.Name)
                {

                    case "LedP1":
                        try
                        {
                            var p1 = await playersTable.LookupAsync(App.game.Player1);
                            p1.Score += App.game.PointsPlayer1;
                            p1.GamesPlayed++;
                            await playersTable.UpdateAsync(p1);
                        }
                        catch (MobileServiceInvalidOperationException ex)
                        {
                            var p1 = new PlayerItem { Id = App.game.Player1, Score = App.game.PointsPlayer1, GamesPlayed = 1 };
                            await playersTable.InsertAsync(p1);
                        }
                        catch (System.Net.Http.HttpRequestException ex)
                        {
                            ContentDialog deleteFileDialog2 = new ContentDialog
                            {
                                FontSize = 10,
                                Title = "Network Error",
                                PrimaryButtonText = "Home"
                            };

                            ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                            this.Frame.Navigate(typeof(ServerMenuPage));
                        }
                        break;
                    case "LedP2":
                        try
                        {
                            var p2 = await playersTable.LookupAsync(App.game.Player2);
                            p2.Score += App.game.PointsPlayer2;
                            p2.GamesPlayed++;
                            await playersTable.UpdateAsync(p2);
                        }
                        catch (MobileServiceInvalidOperationException ex)
                        {
                            var p2 = new PlayerItem { Id = App.game.Player2, Score = App.game.PointsPlayer2, GamesPlayed = 1 };
                            await playersTable.InsertAsync(p2);
                        }
                        catch (System.Net.Http.HttpRequestException ex)
                        {
                            ContentDialog deleteFileDialog2 = new ContentDialog
                            {
                                FontSize = 10,
                                Title = "Network Error",
                                PrimaryButtonText = "Home"
                            };

                            ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                            this.Frame.Navigate(typeof(ServerMenuPage));
                        }
                        break;
                    case "LedP3":
                        try
                        {
                            var p3 = await playersTable.LookupAsync(App.game.Player3);
                            p3.Score += App.game.PointsPlayer3;
                            p3.GamesPlayed++;
                            await playersTable.UpdateAsync(p3);
                        }
                        catch (MobileServiceInvalidOperationException ex)
                        {
                            var p3 = new PlayerItem { Id = App.game.Player3, Score = App.game.PointsPlayer3, GamesPlayed = 1 };
                            await playersTable.InsertAsync(p3);
                        }
                        catch (System.Net.Http.HttpRequestException ex)
                        {
                            ContentDialog deleteFileDialog2 = new ContentDialog
                            {
                                FontSize = 10,
                                Title = "Network Error",
                                PrimaryButtonText = "Home"
                            };

                            ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                            this.Frame.Navigate(typeof(ServerMenuPage));
                        }
                        break;
                    case "LedP4":
                        try
                        {
                            var p4 = await playersTable.LookupAsync(App.game.Player4);
                            p4.Score += App.game.PointsPlayer4;
                            p4.GamesPlayed++;
                            await playersTable.UpdateAsync(p4);
                        }
                        catch (MobileServiceInvalidOperationException ex)
                        {
                            var p4 = new PlayerItem { Id = App.game.Player4, Score = App.game.PointsPlayer4, GamesPlayed = 1 };
                            await playersTable.InsertAsync(p4);
                        }
                        catch (System.Net.Http.HttpRequestException ex)
                        {
                            ContentDialog deleteFileDialog2 = new ContentDialog
                            {
                                FontSize = 10,
                                Title = "Network Error",
                                PrimaryButtonText = "Home"
                            };

                            ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                            this.Frame.Navigate(typeof(ServerMenuPage));
                        }
                        break;
                }



                try
                {
                    App.game = await gameTable.LookupAsync(App.gameID);
                }
                catch (System.Net.Http.HttpRequestException ex)
                {
                    ContentDialog deleteFileDialog2 = new ContentDialog
                    {
                        FontSize = 10,
                        Title = "Network Error",
                        PrimaryButtonText = "Home"
                    };

                    ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                    this.Frame.Navigate(typeof(ServerMenuPage));
                }
                switch (App.dataServiceDevice.Name)
                {
                    case "LedP1":
                        App.game.PlayerQuit1 = true;
                        break;
                    case "LedP2":
                        App.game.PlayerQuit2 = true;
                        break;
                    case "LedP3":
                        App.game.PlayerQuit3 = true;
                        break;
                    case "LedP4":
                        App.game.PlayerQuit4 = true;
                        break;
                }
                switch (App.game.PlayersNum)
                {
                    case 1:

                        if (App.game.PlayerQuit1)
                        {
                            App.game.Complete = true;
                        }
                        break;
                    case 2:
                        if (App.game.PlayerQuit1 && App.game.PlayerQuit2)
                        {
                            App.game.Complete = true;
                        }
                        break;
                    case 3:
                        if (App.game.PlayerQuit1 && App.game.PlayerQuit2 && App.game.PlayerQuit3)
                        {
                            App.game.Complete = true;
                        }
                        break;
                    case 4:
                        if (App.game.PlayerQuit1 && App.game.PlayerQuit2 && App.game.PlayerQuit3 && App.game.PlayerQuit4)
                        {
                            App.game.Complete = true;
                        }
                        break;
                }

                try
                {
                    await gameTable.UpdateAsync(App.game);
                }
                catch (System.Net.Http.HttpRequestException ex)
                {
                    ContentDialog deleteFileDialog2 = new ContentDialog
                    {
                        FontSize = 10,
                        Title = "Network Error",
                        PrimaryButtonText = "Home"
                    };

                    ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                    this.Frame.Navigate(typeof(ServerMenuPage));
                }
                
                this.Frame.Navigate(typeof(ServerMenuPage));

            }
            else
            {

            }
        }


    }   

}
