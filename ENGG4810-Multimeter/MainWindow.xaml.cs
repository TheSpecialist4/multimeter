using ENGG4810_Multimeter.ViewModel;
using LiveCharts;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ENGG4810_Multimeter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainViewModel vm;

        private bool isPlaying;

        public MainWindow()
        {
            vm = new MainViewModel();
            DataContext = vm;

            InitializeComponent();
            isPlaying = false;
        }

        private void btnConnected_Click(object sender, RoutedEventArgs e)
        {
            btnDisconnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnConnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnOpenFile.Visibility = Visibility.Collapsed;
            btnSave.Visibility = Visibility.Visible;

            BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);
        }

        private void btnDisconnected_Click(object sender, RoutedEventArgs e)
        {
            btnConnected.BorderBrush = new SolidColorBrush(Colors.Transparent);
            btnDisconnected.BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#9017a5"));

            btnOpenFile.Visibility = Visibility.Visible;
            btnSave.Visibility = Visibility.Collapsed;

            BeginStoryboard(this.FindResource("ModeLoadStoryboard") as Storyboard);
        }

        private void btnPlay_Click(object sender, RoutedEventArgs e)
        {
            // if is playing == false, change icon to pause -> E769
            btnPlay.Content = ((!isPlaying) ? "\xE769" : "\xE768");
            btnPlay.ToolTip = ((!isPlaying) ? "Pause Plotting" : "Start Plotting");
            //if (!isPlaying)
            //{
            //    BeginStoryboard(FindResource("SimulatePlayStoryboard") as Storyboard);
            //}
            //else
            //{
            //    (FindResource("SimulatePlayStoryboard") as Storyboard).RepeatBehavior = new RepeatBehavior(1.0);
            //}
            vm.AddPointsToPlot();

            isPlaying = !isPlaying;
        }

        private void btnMultimeterHide_Click(object sender, RoutedEventArgs e)
        {
            columnMultimeter.Width = new GridLength(0.0);
        }

        private void btnCurrent_Click(object sender, RoutedEventArgs e)
        {
            txtBlockMultiUnit.Text = "A";
            txtBlockDataHeading.Text = "Current: ";
        }

        private void btnResistance_Click(object sender, RoutedEventArgs e)
        {
            txtBlockMultiUnit.Text = "\x03A9";
            txtBlockDataHeading.Text = "Resistance: ";
        }

        private void btnVoltage_Click(object sender, RoutedEventArgs e)
        {
            txtBlockDataHeading.Text = "Voltage: ";
            txtBlockMultiUnit.Text = "V";
        }
    }
}
