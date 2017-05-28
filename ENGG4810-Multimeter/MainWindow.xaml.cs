using ENGG4810_Multimeter.ViewModel;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace ENGG4810_Multimeter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Main view model of the class
        /// </summary>
        public MainViewModel vm;

        private Color buttonGreen;
        private Color buttonGray;

        public MainWindow()
        {
            InitializeComponent();

            buttonGray = (Color)ColorConverter.ConvertFromString("#FFDDDDDD");
            buttonGreen = (Color)ColorConverter.ConvertFromString("#85e263");

            vm = (MainViewModel)this.DataContext;
       }

        /// <summary>
        /// Handler for connected button click
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnConnected_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.SwitchToConnected())
            {
                mainGrid.Opacity = 0.3;
                MessageBox.Show("Couldn't connect to any port. Switching to disconnected mode");
                SwitchToDisconnectedMode();
                return;
            }

            columnMultimeter.Width = new GridLength(325.0);

            btnDisconnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnConnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnPlay.Content = "\xE768";

            btnOpenFile.Visibility = Visibility.Collapsed;
            btnSave.Visibility = Visibility.Visible;

            stackPanelMasks.Visibility = Visibility.Collapsed;

            //BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);
        }

        /// <summary>
        /// Handler for disconnected button click
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

        /// <summary>
        /// Change visibility and appearance of various GUI elements to switch to disconnected mode
        /// </summary>
        private void SwitchToDisconnectedMode()
        {
            vm.SwitchToDisconnected();
            columnMultimeter.Width = new GridLength(0.0);
            btnConnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnDisconnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnOpenFile.Visibility = Visibility.Visible;
            btnSave.Visibility = Visibility.Collapsed;

            btnPlay.Content = "\xE768";

            BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);

            stackPanelMasks.Visibility = Visibility.Visible;
        }

        /// <summary>
        /// Event handler for the play button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnPlay_Click(object sender, RoutedEventArgs e)
        {
            vm.IsReading = !vm.IsReading;
            
            btnPlay.Content = ((vm.IsReading) ? "\xE769" : "\xE768");
            btnPlay.ToolTip = ((vm.IsReading) ? "Pause Plotting" : "Start Plotting");

            if (vm.IsReading && vm.IsModeConnected)
            {
                //Task.Factory.StartNew(vm.StartContinuousGraph);
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

        /// <summary>
        /// Event handler for Multimeter hide button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnMultimeterHide_Click(object sender, RoutedEventArgs e)
        {
            columnMultimeter.Width = new GridLength(0.0);

            //BeginStoryboard(this.FindResource("ShowFileErrorStoryboard") as Storyboard);

        }

        /// <summary>
        /// Event handler when current button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCurrent_Click(object sender, RoutedEventArgs e)
        {
            vm.Unit = "A";
            vm.Value = "0";
            vm.DataType = "Current: ";

            changeMultiBtnBorderToGreen("C");
        }

        /// <summary>
        /// Event handler when resistance button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnResistance_Click(object sender, RoutedEventArgs e)
        {
            vm.Unit = "\x03A9";
            vm.Value = "0";
            vm.DataType = "Resistance: ";

            changeMultiBtnBorderToGreen("R");
        }

        /// <summary>
        /// event handler when voltage button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnVoltage_Click(object sender, RoutedEventArgs e)
        {
            vm.DataType = "Voltage: ";
            vm.Value = "0";
            vm.Unit = "V";

            changeMultiBtnBorderToGreen("V");
        }

        /// <summary>
        /// Change the colour of the multimeter buttons to green
        /// </summary>
        /// <param name="buttonType"></param>
        private void changeMultiBtnBorderToGreen(string buttonType)
        {
            
        }

        /// <summary>
        /// event handler when save button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            vm.SaveData();
        }

        /// <summary>
        /// event handler when open file button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOpenFile_Click(object sender, RoutedEventArgs e)
        {
            vm.LoadFile();
        }

        /// <summary>
        /// event handler when reset button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
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

        /// <summary>
        /// event handler when window is loaded
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (!vm.SetUpSerial())
            {
                mainGrid.Opacity = 0.3;
                MessageBox.Show("No serial connection found! Switching to disconnected mode");
                SwitchToDisconnectedMode();
            }
        }

        /// <summary>
        /// event handler when a point on the chart is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="chartPoint"></param>
        private void chart_DataClick(object sender, LiveCharts.ChartPoint chartPoint)
        {
            var chartType = vm.HandlePointClick(chartPoint);

            btnLowDelete.IsEnabled = false;
            btnHighDelete.IsEnabled = false;

            switch(chartType)
            {
                case "low":
                    btnLowDelete.IsEnabled = true;
                    btnLowAdd.Visibility = Visibility.Collapsed;
                    btnLowEdit.Visibility = Visibility.Visible;

                    btnHighAdd.Content = "\xE710";
                    break;
                case "high":
                    btnHighDelete.IsEnabled = true;
                    btnHighAdd.Visibility = Visibility.Collapsed;
                    btnHighEdit.Visibility = Visibility.Visible;

                    btnLowAdd.Content = "\xE710";
                    break;
            }
        }

        /// <summary>
        /// event handler when Add button is clicked for low mask series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLowAdd_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.AddToLow(txtBoxLowX.Text, txtBoxLowY.Text))
            {
                txtBoxLowY.BorderBrush = new SolidColorBrush(Colors.Red);
            }
            else
            {
                txtBoxLowY.BorderBrush = new SolidColorBrush(Colors.LightGray);
            }
        }

        /// <summary>
        /// Event handler when edit button is clicked for low mask series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLowEdit_Click(object sender, RoutedEventArgs e)
        {
            vm.EditLow(txtBoxLowX.Text, txtBoxLowY.Text);
            btnLowEdit.Visibility = Visibility.Collapsed;
            btnLowAdd.Visibility = Visibility.Visible;
        }

        /// <summary>
        /// Event handler when delete button is clicked for a point on low mask series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLowDelete_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.DeleteFromLow(txtBoxLowX.Text))
            {
                txtBoxLowX.BorderBrush = new SolidColorBrush(Colors.Red);
            }
            else
            {
                txtBoxLowX.BorderBrush = new SolidColorBrush(Colors.LightGray);
            }
        }

        /// <summary>
        /// event handler when add button is clicked for high mask testing series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHighAdd_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.AddToHigh(txtBoxHighX.Text, txtBoxHighY.Text))
            {
                txtBoxHighY.BorderBrush = new SolidColorBrush(Colors.Red);
            }
            else
            {
                txtBoxHighY.BorderBrush = new SolidColorBrush(Colors.LightGray);
            }
        }

        /// <summary>
        /// event handler when delete button is clicked for high mask testing series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHighDelete_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.DeleteFromHigh(txtBoxHighX.Text))
            {
                txtBoxHighX.BorderBrush = new SolidColorBrush(Colors.Red);
            }
            else
            {
                txtBoxHighX.BorderBrush = new SolidColorBrush(Colors.LightGray);
            }
        }

        /// <summary>
        /// Event handler when edit button is clicked for high mask testing series
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHighEdit_Click(object sender, RoutedEventArgs e)
        {
            vm.EditHigh(txtBoxHighX.Text, txtBoxHighY.Text);
            btnHighAdd.Visibility = Visibility.Visible;
            btnHighEdit.Visibility = Visibility.Collapsed;
        }

        /// <summary>
        /// event handler when a key is pressed
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Window_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Escape)
            {
                btnLowAdd.Visibility = Visibility.Visible;
                btnHighAdd.Visibility = Visibility.Visible;

                btnLowEdit.Visibility = Visibility.Collapsed;
                btnHighEdit.Visibility = Visibility.Collapsed;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            vm.SendMode();
        }

        /// <summary>
        /// event handler when load mask file button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLoadMaskFile_Click(object sender, RoutedEventArgs e)
        {
            if (!vm.LoadMaskFile())
            {
                BeginStoryboard(this.FindResource("ShowFileErrorStoryboard") as Storyboard);
            }
        }

        /// <summary>
        /// event handler when refresh button is clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRefreshMask_Click(object sender, RoutedEventArgs e)
        {
            vm.TestMask();
        }
    }
}
