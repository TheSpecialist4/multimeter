using ENGG4810_Multimeter.Model;
using GalaSoft.MvvmLight;
using LiveCharts;
using LiveCharts.Wpf;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

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

        public bool IsSerialWorking { get; set; }

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

        private double _xAxisMax;
        public double XAxisMax
        {
            get { return _xAxisMax; }
            set {
                if (_xAxisMax == value) return;

                _xAxisMax = value;
                RaisePropertyChanged("XAxisMax");
            }
        }

        public string[] Labels { get; set; }
        public Func<double, string> YFormatter { get; set; }

        private Random random = new Random();

        public SerialFacade SerialHandler { get; set; }

        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            IsModeConnected = true;
            IsSerialWorking = false;

            Value = "0";
            Unit = "V";
            DataType = "Voltage: ";

            XAxisMin = 0;
            XAxisMax = 10;

            DataFileLocation = "";

            YFormatter = value => value.ToString();

            //modifying the series collection will animate and update the chart
            SeriesCollection = new SeriesCollection
            {
                new LineSeries
                {
                    Title = "Data",
                    Values = new ChartValues<double>(),
                    LineSmoothness = 0.5 //straight lines, 1 really smooth lines
                },
                new LineSeries
                {
                    Title = "Low",
                    Values = new ChartValues<double>(),
                    LineSmoothness = 0.5,
                    StrokeThickness = 0.5
                }
            };

            //SetUpSerial();
        }

        public void SetUpSerial()
        {
            SerialHandler = SerialFacade.GetInstance();
            SerialHandler.IncomingData.CollectionChanged += SerialIncomingData_CollectionChanged;

            IsSerialWorking = SerialHandler.SetupConnection();
            //IsSerialWorking = true;
        }

        private void SerialIncomingData_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && IsReading)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(() =>
                {
                    double value = double.Parse(SerialHandler.IncomingData[SerialHandler.IncomingData.Count - 1]);
                    SeriesCollection[0].Values.Add(value);
                    if (SeriesCollection[0].Values.Count > 10)
                    {
                        XAxisMin++;
                        XAxisMax++;
                    }
                    Value = value.ToString();
                    Debug.WriteLine(value + " added to graph");
                }));
            }
        }

        public void StartContinuousGraph()
        {
            //while (IsReading && IsSerialWorking)
            //{
            //    var next = random.Next(0, 20);

            //    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(() =>
            //    {
            //        SerialHandler.IncomingData.Add(next.ToString());
            //        Value = next.ToString();
            //        if (SeriesCollection[0].Values.Count > 10)
            //        {
            //            XAxisMin++;
            //            XAxisMax++;
            //        }
            //        Thread.Sleep(500);
            //    }));
            //}
        }

        public void SwitchMode()
        {
            SeriesCollection[0].Values.Clear();
            Value = "0";
            if (IsModeConnected)
            {
                SerialHandler.ClosePort();
            }
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
        }

        public void LoadFile()
        {
            var openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "CSV File (*.csv)|*.csv";
            openFileDialog.FileOk += (s, e) =>
            {
                DataFileLocation = openFileDialog.FileName;
                LoadFileData();
            };
            openFileDialog.ShowDialog();
        }

        private void OpenFileDialog_FileOk(object sender, System.ComponentModel.CancelEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void LoadFileData()
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
                SeriesCollection[0].Values.Add(double.Parse(data[i]));
                Value = data[i];
            }
        }

        public void DeleteGraphData()
        {
            SeriesCollection[0].Values.Clear();
            Value = "0";
        }

        public void AddToLow(ChartPoint value)
        {
            SeriesCollection[1].Values.Add(value);
        }
    }
}