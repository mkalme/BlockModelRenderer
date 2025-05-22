using System.Windows;

namespace Model.Gui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void OnCreate(object sender, StartupEventArgs e)
        {
            MainWindowModel model = new MainWindowModel(Dispatcher);

            MainWindow mainView = new(model);
            mainView.Show();
        }
    }
}
