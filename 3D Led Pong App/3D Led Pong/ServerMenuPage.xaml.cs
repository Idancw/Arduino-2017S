using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace Led_Pong_App
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ServerMenuPage : Page
    {
        public ServerMenuPage()
        {
            this.InitializeComponent();
            this.BTName.Text = App.dataServiceDevice.Name;
        }

        private void ButtonCreate_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ServerCreateGamePage));
        }

        private void ButtonJoin_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ServerJoinGamePage));
        }

        private async void ButtonStart_Click(object sender, RoutedEventArgs e)
        {
            var n = App._btWriter.WriteString("j\n");
            await App._btWriter.StoreAsync();
            this.Frame.Navigate(typeof(ControllerPage));
        }

        private async void ButtonDemo_Click(object sender, RoutedEventArgs e)
        {
            var n = App._btWriter.WriteString("a\n");
            await App._btWriter.StoreAsync();
        }

        private async void ButtonScoreBoard_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ScoreBoardPage));
        }
    }
}
