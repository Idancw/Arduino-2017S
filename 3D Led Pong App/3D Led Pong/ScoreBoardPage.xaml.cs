/*
 * To add Offline Sync Support:
 *  1) Add the NuGet package Microsoft.Azure.Mobile.Client.SQLiteStore (and dependencies) to all client projects
 *  2) Uncomment the #define OFFLINE_SYNC_ENABLED
 *
 * For more information, see: http://go.microsoft.com/fwlink/?LinkId=717898
 */
//#define OFFLINE_SYNC_ENABLED

using Microsoft.WindowsAzure.MobileServices;
using System;
using System.Threading.Tasks;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

#if OFFLINE_SYNC_ENABLED
using Microsoft.WindowsAzure.MobileServices.SQLiteStore;  // offline sync
using Microsoft.WindowsAzure.MobileServices.Sync;         // offline sync
#endif

namespace Led_Pong_App
{
    public sealed partial class ScoreBoardPage : Page
    {
        private MobileServiceCollection<TodoItem, TodoItem> items;

        private MobileServiceCollection<NatanTest, NatanTest> natanItems;

        private MobileServiceCollection<RunningGameItem, RunningGameItem> gameItems;

        private MobileServiceCollection<PlayerItem, PlayerItem> playerItems;

#if OFFLINE_SYNC_ENABLED
        private IMobileServiceSyncTable<TodoItem> todoTable = App.MobileService.GetSyncTable<TodoItem>(); // offline sync
#else
        private IMobileServiceTable<TodoItem> todoTable = App.MobileService.GetTable<TodoItem>();
        private IMobileServiceTable<NatanTest> natanTable = App.MobileService.GetTable<NatanTest>();
        private IMobileServiceTable<RunningGameItem> gameTable = App.MobileService.GetTable<RunningGameItem>();
        private IMobileServiceTable<PlayerItem> playerTable = App.MobileService.GetTable<PlayerItem>();

#endif

        public ScoreBoardPage()
        {
            this.InitializeComponent();
        }

        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
#if OFFLINE_SYNC_ENABLED
            await InitLocalStoreAsync(); // offline sync
#endif
            ButtonRefresh_Click(this, null);
        }

        private async Task RefreshTodoItems()
        {
            MobileServiceInvalidOperationException exception = null;
            try
            {
                // This code refreshes the entries in the list view by querying the TodoItems table.
                // The query excludes completed TodoItems.
                /*
                items = await todoTable
                    .Where(todoItem => todoItem.Complete == false)
                    .ToCollectionAsync();

                natanItems = await natanTable
                    .Where(natanItem => natanItem.Complete == false)
                    .ToCollectionAsync();
                */
                try
                {
                    playerItems = await playerTable
                        .OrderByDescending(playerItem => playerItem.Score)
                       .ToCollectionAsync();
                } catch (System.Net.Http.HttpRequestException ex)
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
                
            }
            catch (MobileServiceInvalidOperationException e)
            {
                exception = e;
            }

            if (exception != null)
            {
                await new MessageDialog(exception.Message, "Error loading items").ShowAsync();
            }
            else
            {
                //ListItems.ItemsSource = items;
                ListItems.ItemsSource = playerItems;
                //this.ButtonSave.IsEnabled = true;
            }
        }

       
        private async void ButtonRefresh_Click(object sender, RoutedEventArgs e)
        {
            //ButtonRefresh.IsEnabled = false;

#if OFFLINE_SYNC_ENABLED
            await SyncAsync(); // offline sync
#endif
            await RefreshTodoItems();

            //ButtonRefresh.IsEnabled = true;
        }

        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ServerMenuPage));
        }

        private async void ButtonSoftReset_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog deleteFileDialog = new ContentDialog
            {
                FontSize = 10,
                Title = "Soft reset the Cube",
                PrimaryButtonText = "Reset",
                SecondaryButtonText = "Cancel"
            };

            ContentDialogResult result = await deleteFileDialog.ShowAsync();

            // Delete the file if the user clicked the primary button.
            /// Otherwise, do nothing.
            if (result == ContentDialogResult.Primary)
            {
                var n = App._btWriter.WriteString("m\n");
                await App._btWriter.StoreAsync();
            } else
            {

            }
        }

        /*
        private void TextInput_KeyDown(object sender, Windows.UI.Xaml.Input.KeyRoutedEventArgs e)
        {
            if (e.Key == Windows.System.VirtualKey.Enter) {
                ButtonSave.Focus(FocusState.Programmatic);
            }
        }
        */


        private void TitleTextBlock_SelectionChanged(object sender, RoutedEventArgs e)
        {

        }

        private void ListItems_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
