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
using Windows.Globalization.DateTimeFormatting;
using Windows.Globalization;
using System.Diagnostics;


#if OFFLINE_SYNC_ENABLED
using Microsoft.WindowsAzure.MobileServices.SQLiteStore;  // offline sync
using Microsoft.WindowsAzure.MobileServices.Sync;         // offline sync
#endif

namespace Led_Pong_App
{
    public sealed partial class ServerCreateGamePage : Page
    {
        private MobileServiceCollection<TodoItem, TodoItem> items;

        private MobileServiceCollection<NatanTest, NatanTest> natanItems;
        //private MobileServiceCollection<RunningGameItem, RunningGameItem> gameItems;

#if OFFLINE_SYNC_ENABLED
        private IMobileServiceSyncTable<TodoItem> todoTable = App.MobileService.GetSyncTable<TodoItem>(); // offline sync
#else

        private IMobileServiceTable<TodoItem> todoTable = App.MobileService.GetTable<TodoItem>();
        private IMobileServiceTable<NatanTest> natanTable = App.MobileService.GetTable<NatanTest>();
        private IMobileServiceTable<RunningGameItem> gameTable = App.MobileService.GetTable<RunningGameItem>();

        private bool createGame = false;

#endif

        public ServerCreateGamePage()
        {
            this.InitializeComponent();
        }

        /*
        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
#if OFFLINE_SYNC_ENABLED
            await InitLocalStoreAsync(); // offline sync
#endif
            ButtonRefresh_Click(this, null);
        }
        */

        /*
        private async Task InsertTodoItem(TodoItem todoItem)
        {
            // This code inserts a new TodoItem into the database. After the operation completes
            // and the mobile app backend has assigned an id, the item is added to the CollectionView.
            await todoTable.InsertAsync(todoItem);
            items.Add(todoItem);

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
        */

        private async Task InsertGameItem(RunningGameItem gameItem)
        {
            // This code inserts a new TodoItem into the database. After the operation completes
            // and the mobile app backend has assigned an id, the item is added to the CollectionView.
            try
            {
                await gameTable.InsertAsync(gameItem);
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
            
            //gameItems.Add(gameItem);

#if OFFLINE_SYNC_ENABLED
            await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
        }

        private async Task<RunningGameItem> GetgameItem(string id)
        {
            // This code inserts a new TodoItem into the database. After the operation completes
            // and the mobile app backend has assigned an id, the item is added to the CollectionView.
            RunningGameItem result = new RunningGameItem();
            try 
            {
                result = await gameTable.LookupAsync(id);
               
            }
            catch (System.Net.Http.HttpRequestException ex)
            {
                ContentDialog deleteFileDialog = new ContentDialog
                {
                    FontSize = 10,
                    Title = "Network Error",
                    PrimaryButtonText = "Home"
                };

                ContentDialogResult result2 = await deleteFileDialog.ShowAsync();
                this.Frame.Navigate(typeof(ServerMenuPage));
            }
            return result;

            //gameItems.Add(gameItem);

#if OFFLINE_SYNC_ENABLED
            await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
        }

        /*
        private async Task RefreshTodoItems()
        {
            MobileServiceInvalidOperationException exception = null;
            try
            {
                // This code refreshes the entries in the list view by querying the TodoItems table.
                // The query excludes completed TodoItems.
                items = await todoTable
                    .Where(todoItem => todoItem.Complete == false)
                    .ToCollectionAsync();

                natanItems = await natanTable
                    .Where(natanItem => natanItem.Complete == false)
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
                this.ButtonSave.IsEnabled = true;
            }
        }
        */

        /*private async Task UpdateCheckedTodoItem(TodoItem item)
        {
            // This code takes a freshly completed TodoItem and updates the database.
			// After the MobileService client responds, the item is removed from the list.
            await todoTable.UpdateAsync(item);
            items.Remove(item);

#if OFFLINE_SYNC_ENABLED
            await App.MobileService.SyncContext.PushAsync(); // offline sync
#endif
        }*/

        /*private async void ButtonRefresh_Click(object sender, RoutedEventArgs e)
        {

#if OFFLINE_SYNC_ENABLED
            await SyncAsync(); // offline sync
#endif
            await RefreshTodoItems();
        }*/

        private async void ButtonSave_Click(object sender, RoutedEventArgs e)
        {
            //var todoItem = new TodoItem { Text = TextInput.Text };
            ButtonSave.IsEnabled = false;

            if (this.NickNameInput.Text == "" || this.TextInput.Text == "")
            {
                ContentDialog deleteFileDialog = new ContentDialog
                {
                    FontSize = 10,
                    Title = "Create game with blank name?",
                    PrimaryButtonText = "Create",
                    SecondaryButtonText = "Cancel"
                };

                ContentDialogResult result = await deleteFileDialog.ShowAsync();

                // Delete the file if the user clicked the primary button.
                /// Otherwise, do nothing.
                if (result == ContentDialogResult.Primary)
                {
                    App.game = new RunningGameItem();
                    if (TextInput.Text == "")
                    {
                        GeographicRegion userRegion = new GeographicRegion();
                        //App.game = game;
                        string regionCode = userRegion.CodeTwoLetter;
                        DateTimeFormatter timeFormatter = new DateTimeFormatter("hour minute", new[] { regionCode });
                        string time = timeFormatter.Format(DateTime.Now);
                        Debug.WriteLine("time= " + time);

                        /*
                        if (String.Compare(App.dataServiceDevice.Name,"LedP1")==0)
                        {
                            App.game.GameName = "LedP1 " + time;
                        }
                        if (String.Compare(App.dataServiceDevice.Name, "LedP4") == 0)
                        {
                            App.game.GameName = "LedP4 " + time;
                        }
                        */
                        switch (App.dataServiceDevice.Name)
                        {
                            case "LedP1":
                                App.game.GameName = "LedP1 " + time;
                                App.game.Player1 = this.NickNameInput.Text == "" ? "LedP1" : this.NickNameInput.Text;
                                App.game.Player2 = App.game.Player3 = App.game.Player4 = "";
                                break;
                            case "LedP2":
                                App.game.GameName = "LedP2 " + time;
                                App.game.Player2 = this.NickNameInput.Text == "" ? "LedP2" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player3 = App.game.Player4 = "";
                                break;
                            case "LedP3":
                                App.game.GameName = "LedP3 " + time;
                                App.game.Player3 = this.NickNameInput.Text == "" ? "LedP3" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player2 = App.game.Player4 = "";
                                break;
                            case "LedP4":
                                App.game.GameName = "LedP4 " + time;
                                App.game.Player4 = this.NickNameInput.Text == "" ? "LedP4" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player2 = App.game.Player3 = "";
                                break;
                        }
                    }
                    if (NickNameInput.Text == "")
                    {
                        switch (App.dataServiceDevice.Name)
                        {
                            case "LedP1":
                                App.game.Player1 = this.NickNameInput.Text == "" ? "LedP1" : this.NickNameInput.Text;
                                App.game.Player2 = App.game.Player3 = App.game.Player4 = "";
                                break;
                            case "LedP2":
                                App.game.Player2 = this.NickNameInput.Text == "" ? "LedP2" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player3 = App.game.Player4 = "";
                                break;
                            case "LedP3":
                                App.game.Player3 = this.NickNameInput.Text == "" ? "LedP3" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player2 = App.game.Player4 = "";
                                break;
                            case "LedP4":
                                App.game.Player4 = this.NickNameInput.Text == "" ? "LedP4" : this.NickNameInput.Text;
                                App.game.Player1 = App.game.Player2 = App.game.Player3 = "";
                                break;
                        }
                    }

                    App.game.PlayersNum = 1;
                    App.game.PointsPlayer1 = 0;
                    App.game.PointsPlayer2 = 0;
                    App.game.PointsPlayer3 = 0;
                    App.game.PointsPlayer4 = 0;
                    App.game.GameStarted = false;
                    App.game.Complete = false;

                    try
                    {
                        await InsertGameItem(App.game);

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

                    App.gameID = App.game.Id;
                    switch (App.dataServiceDevice.Name)
                    {
                        case "LedP1":
                            App.playerNum = 1;
                            break;
                        case "LedP2":
                            App.playerNum = 2;
                            break;
                        case "LedP3":
                            App.playerNum = 3;
                            break;
                        case "LedP4":
                            App.playerNum = 4;
                            break;
                    }
                    GameCreatedText.Text = "Game Created";

                    var n = App._btWriter.WriteString("c33\n");
                    await App._btWriter.StoreAsync();
                    App.playing = true;
                    this.Frame.Navigate(typeof(ControllerPage));
                }
                else
                {
                    createGame = false;
                }
            }
            else
            {
                var game = new RunningGameItem { GameName = TextInput.Text };
                App.game = game;

                switch (App.dataServiceDevice.Name)
                {
                    case "LedP1":
                        App.game.Player1 = this.NickNameInput.Text == "" ? "LedP1" : this.NickNameInput.Text;
                        App.game.Player2 = App.game.Player3 = App.game.Player4 = "";
                        break;
                    case "LedP2":
                        App.game.Player2 = this.NickNameInput.Text == "" ? "LedP2" : this.NickNameInput.Text;
                        App.game.Player1 = App.game.Player3 = App.game.Player4 = "";
                        break;
                    case "LedP3":
                        App.game.Player3 = this.NickNameInput.Text == "" ? "LedP3" : this.NickNameInput.Text;
                        App.game.Player1 = App.game.Player2 = App.game.Player4 = "";
                        break;
                    case "LedP4":
                        App.game.Player4 = this.NickNameInput.Text == "" ? "LedP4" : this.NickNameInput.Text;
                        App.game.Player1 = App.game.Player2 = App.game.Player3 = "";
                        break;
                }

                App.game.PlayersNum = 1;

                App.game.PointsPlayer1 = 0;
                App.game.PointsPlayer2 = 0;
                App.game.PointsPlayer3 = 0;
                App.game.PointsPlayer4 = 0;
                App.game.GameStarted = false;
                App.game.Complete = false;

                await InsertGameItem(App.game);
                App.gameID = App.game.Id;
                switch (App.dataServiceDevice.Name)
                {
                    case "LedP1":
                        App.playerNum = 1;
                        break;
                    case "LedP2":
                        App.playerNum = 2;
                        break;
                    case "LedP3":
                        App.playerNum = 3;
                        break;
                    case "LedP4":
                        App.playerNum = 4;
                        break;
                }
                GameCreatedText.Text = "Game Created";

                var n = App._btWriter.WriteString("c33\n");
                await App._btWriter.StoreAsync();
                App.playing = true;
                this.Frame.Navigate(typeof(ControllerPage));
                /*await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal, () =>
                {
                    Frame.Navigate(typeof(ControllerPage));
                });*/

                //var natanItem = new NatanTest { GameName = "NatiGame" };
                //await InsertNatanItem(natanItem);
            }

            ButtonSave.IsEnabled = true;
        }

        private void ButtonBack_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(ServerMenuPage));
        }

        /*private async void CheckBoxComplete_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox cb = (CheckBox)sender;
            TodoItem item = cb.DataContext as TodoItem;
            await UpdateCheckedTodoItem(item);
        }*/

        private void TextInput_KeyDown(object sender, Windows.UI.Xaml.Input.KeyRoutedEventArgs e)
        {
            if (e.Key == Windows.System.VirtualKey.Enter) {
                ButtonSave.Focus(FocusState.Programmatic);
            }
        }

        #region Offline sync
#if OFFLINE_SYNC_ENABLED
        private async Task InitLocalStoreAsync()
        {
           if (!App.MobileService.SyncContext.IsInitialized)
           {
               var store = new MobileServiceSQLiteStore("localstore.db");
               store.DefineTable<TodoItem>();
               await App.MobileService.SyncContext.InitializeAsync(store);
           }

           await SyncAsync();
        }

        private async Task SyncAsync()
        {
           await App.MobileService.SyncContext.PushAsync();
           await todoTable.PullAsync("todoItems", todoTable.CreateQuery());
        }
#endif
        #endregion

        private void quickStartTask_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
