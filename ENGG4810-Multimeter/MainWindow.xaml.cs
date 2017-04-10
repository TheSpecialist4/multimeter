using ENGG4810_Multimeter.ViewModel;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace ENGG4810_Multimeter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainViewModel vm;

        public MainWindow()
        {
            InitializeComponent();

            vm = (MainViewModel)this.DataContext;
       }

        private void btnConnected_Click(object sender, RoutedEventArgs e)
        {
            vm.IsModeConnected = true;
            btnDisconnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnConnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnPlay.Content = "\xE768";

            btnOpenFile.Visibility = Visibility.Collapsed;
            btnSave.Visibility = Visibility.Visible;

            vm.SwitchMode();

            BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);
        }

        private void btnDisconnected_Click(object sender, RoutedEventArgs e)
        {
            if (vm.SeriesCollection[0].Values.Count != 0)
            {
                if (MessageBox.Show("Switching Mode? You will lose all unsaved data. Click OK to continue",
                "Unsaved Data", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                {
                    SwitchToDisconnectedMode();
                }
            }
            else
            {
                SwitchToDisconnectedMode();
            }
        }

        private void SwitchToDisconnectedMode()
        {
            vm.IsModeConnected = false;
            btnConnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnDisconnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnOpenFile.Visibility = Visibility.Visible;
            btnSave.Visibility = Visibility.Collapsed;

            btnPlay.Content = "\xE768";

            vm.SwitchMode();

            BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);
        }

        private void btnPlay_Click(object sender, RoutedEventArgs e)
        {
            vm.IsReading = !vm.IsReading;
            
            btnPlay.Content = ((vm.IsReading) ? "\xE769" : "\xE768");
            btnPlay.ToolTip = ((vm.IsReading) ? "Pause Plotting" : "Start Plotting");

            if (vm.IsReading && vm.IsModeConnected)
            {
                Task.Factory.StartNew(vm.StartContinuousGraph);
                return;
            }

            if (!vm.IsModeConnected)
            {
                btnPlay.Content = "\xE768";
                btnPlay.ToolTip = "Start Plotting";
                vm.IsReading = false;

                vm.StartGraphingDisconnected();
            }
        }

        private void btnMultimeterHide_Click(object sender, RoutedEventArgs e)
        {
            columnMultimeter.Width = new GridLength(0.0);
        }

        private void btnCurrent_Click(object sender, RoutedEventArgs e)
        {
            vm.Unit = "A";
            vm.Value = "";
            vm.DataType = "Current: ";
        }

        private void btnResistance_Click(object sender, RoutedEventArgs e)
        {
            vm.Unit = "\x03A9";
            vm.Value = "";
            vm.DataType = "Resistance: ";
        }

        private void btnVoltage_Click(object sender, RoutedEventArgs e)
        {
            vm.DataType = "Voltage: ";
            vm.Value = "";
            vm.Unit = "V";
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            vm.SaveData();
        }

        private void btnOpenFile_Click(object sender, RoutedEventArgs e)
        {
            vm.LoadFile();
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("Are you sure?", "Confirmation", MessageBoxButton.YesNo, MessageBoxImage.Question)
                == MessageBoxResult.Yes)
            {
                vm.IsReading = false;
                btnPlay.Content = "\xE768";
                vm.DeleteGraphData();
            }
        }
    }
}
