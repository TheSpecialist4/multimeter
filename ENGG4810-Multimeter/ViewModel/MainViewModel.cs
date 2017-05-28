using ENGG4810_Multimeter.Model;
using GalaSoft.MvvmLight;
using LiveCharts;
using LiveCharts.Defaults;
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

        private string _maskLowX;
        public string MaskLowX
        {
            get { return _maskLowX; }
            set {
                if (_maskLowX == value) return;
                _maskLowX = value;
                RaisePropertyChanged("MaskLowX");
            }
        }

        private string _maskLowY;
        public string MaskLowY
        {
            get { return _maskLowY; }
            set {
                if (_maskLowY == value) return;
                _maskLowY = value;
                RaisePropertyChanged("MaskLowY");
            }
        }

        private string _maskHighX;
        public string MaskHighX
        {
            get { return _maskHighX; }
            set {
                if (_maskHighX == value) return;
                _maskHighX = value;
                RaisePropertyChanged("MaskHighX");
            }
        }

        private string _maskHighY;
        public string MaskHighY
        {
            get { return _maskHighY; }
            set {
                if (_maskHighY == value) return;
                _maskHighY = value;
                RaisePropertyChanged("MaskHighY");
            }
        }

        private double maskLowXValue;
        private double maskHighIndex;

        private double lowHighest = -1;
        private double highLowest = -1;

        public string[] Labels { get; set; }
        public Func<double, string> YFormatter { get; set; }

        private Random random = new Random();

        public SerialFacade SerialHandler { get; set; }

        private string _maskText;
        public string MaskText
        {
            get { return _maskText; }
            set {
                if (_maskText == value) return;
                _maskText = value;
                RaisePropertyChanged("MaskText");
            }
        }


        //temp variables
        private double dataX = 0;

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
                    Values = new ChartValues<ObservablePoint>(),
                    LineSmoothness = 0.5 //straight lines, 1 really smooth lines
                },
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

        //public void SwitchMode()
        //{
        //    SeriesCollection[0].Values.Clear();
        //    Value = "0";
        //    if (IsModeConnected)
        //    {
        //        SerialHandler.ClosePort();
        //    }
        //}

        public void SwitchToDisconnected()
        {
            IsModeConnected = false;
            SerialHandler.ClosePort();
            if (SeriesCollection.Count == 1)
            {
                SeriesCollection.Add(
                    new LineSeries
                    {
                        Title = "Low",
                        Values = new ChartValues<ObservablePoint>(),
                        LineSmoothness = 0.1,
                        StrokeThickness = 0.5
                    });
                SeriesCollection.Add(
                    new LineSeries
                    {
                        Title = "High",
                        Values = new ChartValues<ObservablePoint>(),
                        LineSmoothness = 0.1,
                        StrokeThickness = 0.5
                    });
            }
            //SeriesCollection[1].Values.Add(2.0);
            //SeriesCollection[2].Values.Add(6.0);

            //SeriesCollection[1].Values.Add(3.0);
            //SeriesCollection[2].Values.Add(10.0);

            //SeriesCollection[1].Values.Add(1.0);
            //SeriesCollection[2].Values.Add(8.0);

            //SeriesCollection[1].Values.Add(2.0);
            //SeriesCollection[2].Values.Add(7.0);
        }

        public void SwitchToConnected()
        {
            IsModeConnected = true;
            if (!SerialHandler.OpenPort())
            {
                Debug.WriteLine("couldnt open port");
            }
            while(SeriesCollection.Count > 1)
            {
                SeriesCollection.RemoveAt(SeriesCollection.Count - 1);
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
                SeriesCollection[0].Values.Add(new ObservablePoint { X = dataX++, Y = double.Parse(data[i])});
                Value = data[i];
            }
        }

        public void DeleteGraphData()
        {
            SeriesCollection[0].Values.Clear();
            Value = "0";
        }

        public bool AddToLow(string xstring, string ystring)
        {
            double x, y;
            if (double.TryParse(xstring, out x) && double.TryParse(ystring, out y))
            {
                if (lowHighest < 0) { lowHighest = y; }
                if (y > lowHighest) { lowHighest = y; }

                if (y >= GetHighValueAt(x))
                {
                    return false;
                }
                if (SeriesCollection[1].Values.Count == 0)
                {
                    SeriesCollection[1].Values.Add(new ObservablePoint { X = 0, Y = y });
                }
                if (x < ((ObservablePoint)SeriesCollection[1].Values[SeriesCollection[1].Values.Count - 1]).X)
                {
                    int index = 0;
                    while (((ObservablePoint)SeriesCollection[1].Values[index]).X < x)
                    {
                        index++;
                    }
                    SeriesCollection[1].Values.Insert(index, new ObservablePoint { X = x, Y = y });
                    return true;
                }
                SeriesCollection[1].Values.Add(new ObservablePoint { X = x, Y = y});
            }
            if (SeriesCollection[0].Values.Count > 0)
            {
                double lastDataXValue = ((ObservablePoint)SeriesCollection[0]
                        .Values[SeriesCollection[0].Values.Count - 1]).X;
                bool isPresent = false;
                foreach (ObservablePoint point in SeriesCollection[1].Values)
                {
                    if (point.X == lastDataXValue)
                    {
                        isPresent = true;
                        break;
                    }
                }
                if (!isPresent)
                {
                    SeriesCollection[1].Values.Add(new ObservablePoint
                    {
                        X = lastDataXValue,
                        Y = ((ObservablePoint)SeriesCollection[1].Values[SeriesCollection[1].Values.Count - 1]).Y
                    });
                }
            }
            return true;
        }

        public bool EditLow(string xstring, string ystring)
        {
            double x;
            if (double.TryParse(xstring, out x))
            {
                foreach (ObservablePoint point in SeriesCollection[1].Values)
                {
                    if (point.X == maskLowXValue)
                    {
                        double y = 0;
                        if (double.TryParse(ystring, out y))
                        {
                            point.Y = y;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        private double GetHighValueAt(double x)
        {
            double value = 0;
            ObservablePoint low = new ObservablePoint { X = 0, Y = 0 };
            ObservablePoint high = new ObservablePoint { X = 0, Y = 0 };
            foreach (ObservablePoint point in SeriesCollection[2].Values)
            {
                if (point.X == x)
                {
                    return point.Y;
                }
                if (point.X < x)
                {
                    low = point;
                }
                if (point.X > x)
                {
                    high = point;
                }
            }
            value = low.Y + ((x - low.X) / (high.X - low.X)) * (high.Y - low.Y);
            return value;
        }

        private double GetLowValueAt(double x)
        {
            double value = 0;
            ObservablePoint low = new ObservablePoint { X = 0, Y = 0 };
            ObservablePoint high = new ObservablePoint { X = 0, Y = 0 };
            foreach (ObservablePoint point in SeriesCollection[1].Values)
            {
                if (point.X == x)
                {
                    return point.Y;
                }
                if (point.X < x)
                {
                    low = point;
                }
                if (point.X > x)
                {
                    high = point;
                }
            }
            value = low.Y + ((x - low.X) / (high.X - low.X)) * (high.Y - low.Y);
            return value;
        }

        public bool DeleteFromLow(string xValue)
        {
            double x;
            if (double.TryParse(xValue, out x))
            {
                foreach (ObservablePoint point in SeriesCollection[1].Values)
                {
                    if (point.X == x)
                    {
                        SeriesCollection[1].Values.Remove(point);
                        return true;
                    }
                }
            }
            return false;
        }

        public bool AddToHigh(string xstring, string ystring)
        {
            double x, y;
            if (double.TryParse(xstring, out x) && double.TryParse(ystring, out y))
            {
                if (highLowest < 0) { highLowest = y; }
                if (y < highLowest) { highLowest = y; }

                if (y <= GetLowValueAt(x))
                {
                    return false;
                }
                if (SeriesCollection[2].Values.Count == 0)
                {
                    SeriesCollection[2].Values.Add(new ObservablePoint { X = 0, Y = y });
                }
                if (x < ((ObservablePoint)SeriesCollection[2].Values[SeriesCollection[2].Values.Count - 1]).X)
                {
                    int index = 0;
                    while (((ObservablePoint)SeriesCollection[2].Values[index]).X < x)
                    {
                        index++;
                    }
                    SeriesCollection[2].Values.Insert(index, new ObservablePoint { X = x, Y = y });
                    return true;
                }
                SeriesCollection[2].Values.Add(new ObservablePoint { X = x, Y = y });
            }
            if (SeriesCollection[0].Values.Count > 0)
            {
                double lastDataXValue = ((ObservablePoint)SeriesCollection[0]
                        .Values[SeriesCollection[0].Values.Count - 1]).X;
                bool isPresent = false;
                foreach (ObservablePoint point in SeriesCollection[2].Values)
                {
                    if (point.X == lastDataXValue)
                    {
                        isPresent = true;
                        break;
                    }
                }
                if (!isPresent)
                {
                    SeriesCollection[2].Values.Add(new ObservablePoint
                    {
                        X = lastDataXValue,
                        Y = ((ObservablePoint)SeriesCollection[2].Values[SeriesCollection[2].Values.Count - 1]).Y
                    });
                }
            }
            return true;
        }

        public bool DeleteFromHigh(string xValue)
        {
            double x;
            if (double.TryParse(xValue, out x))
            {
                foreach (ObservablePoint point in SeriesCollection[2].Values)
                {
                    if (point.X == x)
                    {
                        SeriesCollection[2].Values.Remove(point);
                        return true;
                    }
                }
            }
            return false;
        }

        public bool EditHigh(string xstring, string ystring)
        {
            double x;
            if (double.TryParse(xstring, out x))
            {
                foreach (ObservablePoint point in SeriesCollection[2].Values)
                {
                    if (point.X == x)
                    {
                        double y = 0;
                        if (double.TryParse(ystring, out y))
                        {
                            point.Y = y;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        public string HandlePointClick(ChartPoint point)
        {
            if (SeriesCollection.IndexOf(point.SeriesView) == 1) //low
            {
                MaskLowX = point.X + "";
                MaskLowY = point.Y + "";

                MaskHighX = "";
                MaskHighY = "";

                maskLowXValue = point.X;

                return "low";
            } else if (SeriesCollection.IndexOf(point.SeriesView) == 2)
            {
                MaskHighX = point.X + "";
                MaskHighY = point.Y + "";

                MaskLowX = "";
                MaskLowY = "";

                maskHighIndex = point.X;

                return "high";
            }
            return "";
        }

        public void SendMode()
        {
            SerialHandler.SendZero();
        }

        public void TestMask()
        {
            MaskValues.ClearMasks();

            bool isStart = true;
            double start = -1;
            double end = -1;
            double prevValue = 0;
            for (int i = 0; i < SeriesCollection[0].Values.Count; i++)
            {
                ObservablePoint point = SeriesCollection[0].Values[i] as ObservablePoint;
                double highValue = GetHighValueAt(point.X);
                double lowValue = GetLowValueAt(point.X);

                Debug.WriteLine($"checking for ({point.X},{point.Y}) low: {lowValue} high: {highValue}");

                if (point.Y > highValue || point.Y < lowValue)
                {
                    prevValue = point.Y;
                    if (i == 0) {
                        start = 0;
                        isStart = false;
                    }
                    else
                    {
                        double intersection = 0;
                        ObservablePoint prevPoint = SeriesCollection[0].Values[i - 1] as ObservablePoint;
                        if (point.Y > highValue)
                        {
                            intersection = prevPoint.X + ((highValue - prevPoint.Y) / (point.Y - prevPoint.Y)) 
                                * (point.X - prevPoint.X);
                        } else
                        {
                            intersection = prevPoint.X + ((lowValue - prevPoint.Y) / (point.Y - prevPoint.Y))
                                * (point.X - prevPoint.X);
                        }

                        if (isStart)
                        {
                            start = intersection;
                            isStart = false;
                        }
                    }
                } else
                {
                    if (!isStart)
                    {
                        ObservablePoint prevPoint = SeriesCollection[0].Values[i - 1] as ObservablePoint;
                        if (prevValue > highValue)
                        {
                            end = prevPoint.X + ((highValue - prevPoint.Y) / (point.Y - prevPoint.Y))
                                * (point.X - prevPoint.X);
                            Debug.WriteLine("setting end from high");
                        } else
                        {
                            end = prevPoint.X + ((lowValue - prevPoint.Y) / (point.Y - prevPoint.Y))
                                * (point.X - prevPoint.X);
                            Debug.WriteLine("setting end from low");
                        }
                        Debug.WriteLine($"adding to mask: {start} {end}");
                        MaskValues.AddToMask(start, end);
                        isStart = true;
                    }
                }
            }
            MaskText = MaskValues.GetMaskText();
        }
    }
}