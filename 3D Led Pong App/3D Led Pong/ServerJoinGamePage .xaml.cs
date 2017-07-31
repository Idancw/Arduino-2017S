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
    public sealed partial class ServerJoinGamePage : Page
    {
        private MobileServiceCollection<TodoItem, TodoItem> items;

        private MobileServiceCollection<NatanTest, NatanTest> natanItems;

        private MobileServiceCollection<RunningGameItem, RunningGameItem> gameItems;

        private bool joinGame = false;

#if OFFLINE_SYNC_ENABLED
        private IMobileServiceSyncTable<TodoItem> todoTable = App.MobileService.GetSyncTable<TodoItem>(); // offline sync
#else
        private IMobileServiceTable<TodoItem> todoTable = App.MobileService.GetTable<TodoItem>();
        private IMobileServiceTable<NatanTest> natanTable = App.MobileService.GetTable<NatanTest>();
        private IMobileServiceTable<RunningGameItem> gameTable = App.MobileService.GetTable<RunningGameItem>();

#endif

        public ServerJoinGamePage()
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

        private async Task InsertTodoItem(TodoItem todoItem)
        {
            // This code inserts a new TodoItem into the database. After the operation completes
            // and the mobile app backend has assigned an id, the item is added to the CollectionView.
            try
            {
                await todoTable.InsertAsync(todoItem);
                items.Add(todoItem);
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
            

#if OFFLINE_SYNC_ENABLED
            await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
        }

        private async Task InsertNatanItem(NatanTest natanItem)
        {
            // This code inserts a new TodoItem into the database. After the operation completes
            // and the mobile app backend has assigned an id, the item is added to the CollectionView.
            await natanTable.InsertAsync(natanItem);
            natanItems.Add(natanItem);

#if OFFLINE_SYNC_ENABLED
            await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
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

                gameItems = await gameTable
                    .Where(gameItem => gameItem.Complete == false && gameItem.PlayersNum<4 && gameItem.GameStarted==false)
                    .ToCollectionAsync();
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
                ListItems.ItemsSource = gameItems;
                //this.ButtonSave.IsEnabled = true;
            }
        }

        private async Task UpdateCheckedTodoItem(RunningGameItem item)
        {
            // This code takes a freshly completed TodoItem and updates the database.
            // After the MobileService client responds, the item is removed from the list.
            /*
            await todoTable.UpdateAsync(item);
            items.Remove(item);
            ListItems.Focus(Windows.UI.Xaml.FocusState.Unfocused);
            */

            if (this.TextInput.Text == "") {
                ContentDialog deleteFileDialog = new ContentDialog
                {
                    FontSize = 10,
                    Title = "Join game with blank name?",
                    PrimaryButtonText = "Join",
                    SecondaryButtonText = "Cancel"
                };

                ContentDialogResult result = await deleteFileDialog.ShowAsync();

                // Delete the file if the user clicked the primary button.
                /// Otherwise, do nothing.
                if (result == ContentDialogResult.Primary)
                {
                    var joinedGame = await gameTable.LookupAsync(item.Id);
                    joinedGame.PlayersNum++; // Joining the game as next player
                    switch (App.dataServiceDevice.Name)
                    {
                        case "LedP1":
                            joinedGame.Player1 = this.TextInput.Text == "" ? "LedP1" : this.TextInput.Text;
                            App.playerNum = 1;
                            break;
                        case "LedP2":
                            joinedGame.Player2 = this.TextInput.Text == "" ? "LedP2" : this.TextInput.Text;
                            App.playerNum = 2;
                            break;
                        case "LedP3":
                            joinedGame.Player3 = this.TextInput.Text == "" ? "LedP3" : this.TextInput.Text;
                            App.playerNum = 3;
                            break;
                        case "LedP4":
                            joinedGame.Player4 = this.TextInput.Text == "" ? "LedP4" : this.TextInput.Text;
                            App.playerNum = 4;
                            break;
                    }
                    await gameTable.UpdateAsync(joinedGame);

                    //items.Remove(item);
                    ListItems.Focus(Windows.UI.Xaml.FocusState.Unfocused);
                    App.gameID = joinedGame.Id;
                    App.game = joinedGame;
                    joinGame = true;

#if OFFLINE_SYNC_ENABLED
                                await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
                } else
                {
                    joinGame = false;
                }
            } else
            {
                var joinedGame = await gameTable.LookupAsync(item.Id);
                App.game = joinedGame;
                App.game.PlayersNum++; // Joining the game as next player
                switch (App.dataServiceDevice.Name)
                {
                    case "LedP1":
                        App.game.Player1 = this.TextInput.Text == "" ? "LedP1" : this.TextInput.Text;
                        App.playerNum = 1;
                        break;
                    case "LedP2":
                        App.game.Player2 = this.TextInput.Text == "" ? "LedP2" : this.TextInput.Text;
                        App.playerNum = 2;
                        break;
                    case "LedP3":
                        App.game.Player3 = this.TextInput.Text == "" ? "LedP3" : this.TextInput.Text;
                        App.playerNum = 3;
                        break;
                    case "LedP4":
                        App.game.Player4 = this.TextInput.Text == "" ? "LedP4" : this.TextInput.Text;
                        App.playerNum = 4;
                        break;
                }
                await gameTable.UpdateAsync(App.game);

                //items.Remove(item);
                ListItems.Focus(Windows.UI.Xaml.FocusState.Unfocused);
                App.gameID = joinedGame.Id;
                
                joinGame = true;

#if OFFLINE_SYNC_ENABLED
                await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
            }
        }

        private async void ButtonRefresh_Click(object sender, RoutedEventArgs e)
        {
            ButtonRefresh.IsEnabled = false;

#if OFFLINE_SYNC_ENABLED
            await SyncAsync(); // offline sync
#endif
            await RefreshTodoItems();

            ButtonRefresh.IsEnabled = true;
        }

        private async void ButtonSave_Click(object sender, RoutedEventArgs e)
        {
            var todoItem = new TodoItem { Text = TextInput.Text };
            TextInput.Text = "";
            await InsertTodoItem(todoItem);

            var natanItem = new NatanTest { GameName = "NatiGame" };
            await InsertNatanItem(natanItem);
        }

        private async void CheckBoxComplete_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox cb = (CheckBox)sender;
            RunningGameItem item = cb.DataContext as RunningGameItem;
            await UpdateCheckedTodoItem(item);

            if (joinGame == true)
            {

                var n = App._btWriter.WriteString("c33\n");
                await App._btWriter.StoreAsync();

                App.playing = true;
                joinGame = false;

                this.Frame.Navigate(typeof(ControllerPage));
            } else
            {
                cb.IsChecked = false;
            }

        }

        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ServerMenuPage));
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
    }
}
