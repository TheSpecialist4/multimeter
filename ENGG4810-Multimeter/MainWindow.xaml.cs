using ENGG4810_Multimeter.ViewModel;
using System.Diagnostics;
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

        private Color buttonGreen;
        private Color buttonGray;

        public MainWindow()
        {
            InitializeComponent();

            buttonGray = (Color)ColorConverter.ConvertFromString("#FFDDDDDD");
            buttonGreen = (Color)ColorConverter.ConvertFromString("#85e263");

            vm = (MainViewModel)this.DataContext;

            changeMultiBtnBorderToGreen("V");
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
                //vm.StartContinuousGraph();
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
            vm.Value = "0";
            vm.DataType = "Current: ";

            changeMultiBtnBorderToGreen("C");
        }

        private void btnResistance_Click(object sender, RoutedEventArgs e)
        {
            vm.Unit = "\x03A9";
            vm.Value = "0";
            vm.DataType = "Resistance: ";

            changeMultiBtnBorderToGreen("R");
        }

        private void btnVoltage_Click(object sender, RoutedEventArgs e)
        {
            vm.DataType = "Voltage: ";
            vm.Value = "0";
            vm.Unit = "V";

            changeMultiBtnBorderToGreen("V");
        }

        private void changeMultiBtnBorderToGreen(string buttonType)
        {
            //btnResistance.Background = new SolidColorBrush(buttonGray);
            //btnCurrent.Background = new SolidColorBrush(buttonGray);
            //btnVoltage.Background = new SolidColorBrush(buttonGray);

            //switch (buttonType)
            //{
            //    case "V":
            //        btnVoltage.Background = new SolidColorBrush(buttonGreen);
            //        break;
            //    case "C":
            //        btnCurrent.Background = new SolidColorBrush(buttonGreen);
            //        break;
            //    case "R":
            //        btnResistance.Background = new SolidColorBrush(buttonGreen);
            //        break;
            //    default:
            //        break;
            //}
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

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            vm.SetUpSerial();
        }

        private void chart_DataClick(object sender, LiveCharts.ChartPoint chartPoint)
        {
            MessageBox.Show("X: " + chartPoint.X + " Y: " + chartPoint.Y);
        }

        private void chart_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            var pos = e.GetPosition(chart);
            double xstep = (chart.ActualWidth) / 10;
            double xsteps = 0;
            while ((xsteps * xstep + 40) < pos.X) { xsteps++; }

            double ystep = chart.ActualHeight / chart.AxisY.Count;
            Debug.WriteLine("ystep: "+ chart.AxisY[0].Name + 
                " step height: " + chart.AxisY[0].Labels[0]);
            double ysteps = 0;
            while ((ysteps * ystep) < pos.Y) { ysteps++; }
            ysteps = chart.AxisY.Count - ysteps;

            //MessageBox.Show("x: " + xsteps + " y: " + ysteps);

        }
    }
}
