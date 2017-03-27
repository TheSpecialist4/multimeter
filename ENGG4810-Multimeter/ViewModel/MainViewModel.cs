using ENGG4810_Multimeter.Model;
using GalaSoft.MvvmLight;
using LiveCharts;
using LiveCharts.Wpf;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace ENGG4810_Multimeter.ViewModel
{
    /// <summary>
    /// This class contains properties that the main View can data bind to.
    /// <para>
    /// Use the <strong>mvvminpc</strong> snippet to add bindable properties to this ViewModel.
    /// </para>
    /// <para>
    /// You can also use Blend to data bind with the tool's support.
    /// </para>
    /// <para>
    /// See http://www.galasoft.ch/mvvm
    /// </para>
    /// </summary>
    public class MainViewModel : ViewModelBase
    {
        private ObservableCollection<DataModel> _ActiveValues;

        public ObservableCollection<DataModel> ActiveValues
        {
            get { return _ActiveValues; }
            set {
                _ActiveValues = value;
                RaisePropertyChanged("ActiveValues");
            }
        }

        private ObservableCollection<double> _tempPlot;

        public ObservableCollection<double> TempPlot
        {
            get { return _tempPlot; }
            set {
                _tempPlot = value;
                RaisePropertyChanged("TempPlot");
            }
        }

        public bool IsModeConnected;

        //public List<DataModel> ActiveValues;

        public string Value { get; set; }

        public List<DataModel> AllValues;

        public SeriesCollection SeriesCollection { get; set; }
        public string[] Labels { get; set; }
        public Func<double, string> YFormatter { get; set; }

        private Random random = new Random();

        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            _ActiveValues = new ObservableCollection<DataModel>();
            ActiveValues = new ObservableCollection<DataModel>();

            TempPlot = new ObservableCollection<double>();

            Value = "";

            SeriesCollection = new SeriesCollection();

            //Labels = new[] { "Jan", "Feb", "Mar", "Apr", "May" };
            YFormatter = value => value.ToString();

            //modifying the series collection will animate and update the chart
            SeriesCollection.Add(new LineSeries
            {
                Title = "Data",
                Values = new ChartValues<double> { },
                LineSmoothness = 0.5 //straight lines, 1 really smooth lines
            });

            ////modifying any series values will also animate and update the chart
            //SeriesCollection[2].Values.Add(5d);

            //StartPlot();
        }

        private void StartPlot()
        {
            var random = new Random();
            for (int i = 0; i < 10; i++)
            {
                TempPlot.Add(random.Next(0, 20));
            }
            Debug.Write(TempPlot.ToString());
        }

        public void AddPointsToPlot()
        {
            for (int i = 0; i < 15; i++)
            {
                var next = random.NextDouble() * 7;
                SeriesCollection[0].Values.Add(next);
            }
        }
    }
}