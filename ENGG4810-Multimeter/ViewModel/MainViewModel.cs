using ENGG4810_Multimeter.Model;
using GalaSoft.MvvmLight;
using LiveCharts;
using LiveCharts.Wpf;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

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
        public bool IsModeConnected;
        public bool IsReading { get; set; }


        private string _value;
        public string Value
        {
            get { return _value; }
            set {
                if (_value == value)
                {
                    return;
                }
                _value = value;
                RaisePropertyChanged("Value");
            }
        }

        private string _unit;
        public string Unit
        {
            get { return _unit; }
            set {
                if (_unit == value) return;
                _unit = value;
                RaisePropertyChanged("Unit");
            }
        }

        private string _dataType;
        public string DataType
        {
            get { return _dataType; }
            set {
                if (_dataType == value) return;
                _dataType = value;
                RaisePropertyChanged("DataType");
            }
        }

        private string DataFileLocation;

        public SeriesCollection SeriesCollection { get; set; }

        private double _xAxisMin;
        public double XAxisMin {
            get {
                return _xAxisMin;
            }
            set {
                if (_xAxisMin == value) return;

                _xAxisMin = value;
                RaisePropertyChanged("XAxisMin");
            }
        }

        public string[] Labels { get; set; }
        public Func<double, string> YFormatter { get; set; }

        private Random random = new Random();

        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            IsModeConnected = true;

            Value = "";
            Unit = "A";
            DataType = "Current: ";

            DataFileLocation = "";

            SeriesCollection = new SeriesCollection();

            YFormatter = value => value.ToString();

            //modifying the series collection will animate and update the chart
            SeriesCollection.Add(new LineSeries
            {
                Title = "Data",
                Values = new ChartValues<int>(),
                LineSmoothness = 0.5 //straight lines, 1 really smooth lines
            });
        }

        public void StartGraphingConnected()
        {
            for (int i = 0; i < 15; i++)
            {
                var next = random.Next(0, 20);
                Debug.Print($"added {next}\n");
                SeriesCollection[0].Values.Add(next);
                Value = next.ToString() + Unit;
            }
            XAxisMin = SeriesCollection[0].Values.Count - 14;
        }

        public void SwitchMode()
        {
            SeriesCollection[0].Values.Clear();
        }

        public void SaveData()
        {
            var saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "CSV File (*.csv)|*.csv";
            if (saveFileDialog.ShowDialog() == true)
            {
                File.WriteAllText(saveFileDialog.FileName, getCsvData().ToString());
            }
        }

        private StringBuilder getCsvData()
        {
            var csv = new StringBuilder();
            csv.Append(Unit);
            foreach(var value in SeriesCollection[0].Values)
            {
                csv.Append($",{value}");
            }
            return csv;
        }

        public void StartGraphingDisconnected()
        {
            if (String.IsNullOrEmpty(DataFileLocation))
            {
                LoadFile();
            }
            loadFileData();
        }

        public void LoadFile()
        {
            var openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "CSV FIle (*.csv)|*.csv";
            if (openFileDialog.ShowDialog() == true)
            {
                DataFileLocation = openFileDialog.FileName;
            }
        }

        private void loadFileData()
        {
            SeriesCollection[0].Values.Clear();
            string[] data = File.ReadAllText(DataFileLocation).Split(',');
            switch (data[0])
            {
                case "A":
                    DataType = "Current: ";
                    break;
                case "V":
                    DataType = "Voltage: ";
                    break;
                case "\x03A9":
                    DataType = "Resistance: ";
                    break;
                default:
                    DataType = "Value: ";
                    break;
            }
            Unit = data[0];
            for (int i = 1; i < data.Length; i++)
            {
                SeriesCollection[0].Values.Add(int.Parse(data[i]));
                Value = data[i] + Unit;
            }
        }

        public void DeleteGraphData()
        {
            SeriesCollection[0].Values.Clear();
        }
    }
}