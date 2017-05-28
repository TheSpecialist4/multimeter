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
        /// <summary>
        /// The current mode of software
        /// </summary>
        public bool IsModeConnected;
        /// <summary>
        /// Whether or not acquisition is paused
        /// </summary>
        public bool IsReading { get; set; }

        /// <summary>
        /// Connection to serial port established
        /// </summary>
        public bool IsSerialWorking { get; set; }

        /// <summary>
        /// Value of the data sample
        /// </summary>
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

        /// <summary>
        /// Unit of the sample
        /// </summary>
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

        /// <summary>
        /// The type of data (Current/Resistance/Voltage)
        /// </summary>
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

        /// <summary>
        /// Data file
        /// </summary>
        private string DataFileLocation;

        /// <summary>
        /// Chart series
        /// </summary>
        public SeriesCollection SeriesCollection { get; set; }

        /// <summary>
        /// Minimum x axis value in chart
        /// </summary>
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

        /// <summary>
        /// Maximum x axis value displayed on chart
        /// </summary>
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

        /// <summary>
        /// The x coordinate of currently selected Low mask point
        /// </summary>
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

        /// <summary>
        /// The y coordinate of the currently selected low mask point
        /// </summary>
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

        /// <summary>
        /// The x coordinate of the currently selected High mask point
        /// </summary>
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

        /// <summary>
        /// The y coordinate of the currently selected High mask point
        /// </summary>
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

        /// <summary>
        /// Labels on the chart
        /// </summary>
        public string[] Labels { get; set; }

        /// <summary>
        /// formatter for the Y values on the chart
        /// </summary>
        public Func<double, string> YFormatter { get; set; }

        /// <summary>
        /// Serial communication handler
        /// </summary>
        public SerialFacade SerialHandler { get; set; }

        /// <summary>
        /// The failure descriptions of mask test
        /// </summary>
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

        /// <summary>
        /// The timestamp in ISO 8601 format
        /// </summary>
        private string _timestamp;
        public string Timestamp
        {
            get { return _timestamp; }
            set {
                if (_timestamp == value) return;
                _timestamp = value;
                RaisePropertyChanged("Timestamp");
            }
        }

        private string startTimeString = "";
        private string endTime = "";

        private DateTime startTime;

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
        }

        /// <summary>
        /// Sets up serial port connection
        /// </summary>
        /// <returns>True if connection was successfully established</returns>
        public bool SetUpSerial()
        {
            SerialHandler = SerialFacade.GetInstance();
            SerialHandler.IncomingData.CollectionChanged += SerialIncomingData_CollectionChanged;

            IsSerialWorking = SerialHandler.SetupConnection();
            return IsSerialWorking;
        }

        private void SerialIncomingData_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add && IsReading)
            {
                // first value
                if (SeriesCollection[0].Values.Count == 0)
                {
                    startTime = DateTime.UtcNow;
                    startTimeString = startTime.ToString("yyyy-MM-ddTHH\\:mm\\:ss.fffffffzzz");
                }
                endTime = DateTime.UtcNow.ToString("yyyy-MM-ddTHH\\:mm\\:ss.fffffffzzz");
                Timestamp = startTimeString + "/" + endTime;
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(() =>
                {
                    for (int i = SeriesCollection[0].Values.Count; i < SerialHandler.IncomingData.Count; i++)
                    {
                        double value = double.Parse(SerialHandler.IncomingData[i]);
                        SeriesCollection[0].Values.Add(value);
                        if (SeriesCollection[0].Values.Count > 10)
                        {
                            XAxisMin++;
                            XAxisMax++;
                        }
                        Value = value.ToString();
                        Debug.WriteLine(value + " added to graph");
                    }
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

        /// <summary>
        /// Switch mode to disconnected
        /// </summary>
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
        }

        /// <summary>
        /// Switch mode to connected
        /// </summary>
        /// <returns></returns>
        public bool SwitchToConnected()
        {
            if (!SerialHandler.OpenPort())
            {
                Debug.WriteLine("couldnt open port");
                return false;
            }
            while(SeriesCollection.Count > 1)
            {
                SeriesCollection.RemoveAt(SeriesCollection.Count - 1);
            }
            IsModeConnected = true;
            return true;
        }

        /// <summary>
        /// Save the data currently in the graph
        /// </summary>
        public void SaveData()
        {
            if (IsReading)
            {
                MessageBox.Show("Cannot save data while still acquiring data");
                return;
            }
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

        /// <summary>
        /// Start graphing disconnected data
        /// </summary>
        public void StartGraphingDisconnected()
        {
            if (String.IsNullOrEmpty(DataFileLocation))
            {
                LoadFile();
            }
        }

        /// <summary>
        /// Load data file
        /// </summary>
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

        /// <summary>
        /// Load mask testing file
        /// </summary>
        /// <returns>True if the file was of correct format. False otherwise</returns>
        public bool LoadMaskFile()
        {
            var openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "CSV File (*.csv)|*.csv";
            var fileName = "";
            openFileDialog.FileOk += (s, e) =>
            {
                fileName = openFileDialog.FileName;
            };
            openFileDialog.ShowDialog();
            if (!fileName.Equals(""))
            {
                return LoadMaskFileData(fileName);
            }
            return true;
        }

        /// <summary>
        /// Load data from the mask file
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns>True if the data is in correct format</returns>
        private bool LoadMaskFileData(string fileName)
        {
            SeriesCollection[1].Values.Clear();
            SeriesCollection[2].Values.Clear();
            string[] lines = File.ReadAllText(fileName).Split('\n');
            bool isValid = true;
            foreach (var line in lines)
            {
                Debug.WriteLine("line: " + line);
                string[] tokens = line.Split(',');
                if (tokens.Length != 4) { Debug.WriteLine("length error"); isValid = false; }
                tokens[3] = tokens[3].Replace('\n', ' ');
                tokens[3] = tokens[3].Trim();
                if (Unit.ToLower() != tokens[3].ToLower()) {
                    Debug.WriteLine("unit error" + tokens[3] + Unit); isValid = false; }
                if (tokens[0].ToLower().Equals("high"))
                {
                    if (!AddToHigh(tokens[1], tokens[2]))
                    {
                        isValid = false;
                    }
                } else if (tokens[0].ToLower().Equals("low"))
                {
                    if (!AddToLow(tokens[1], tokens[2])) { isValid = false; }
                } else { Debug.WriteLine("high/low error"); isValid = false; }
            }
            if (!isValid)
            {
                SeriesCollection[1].Values.Clear();
                SeriesCollection[2].Values.Clear();
                return false;
            }
            TestMask();
            return true;
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

        /// <summary>
        /// Delete all the currently acquired graph data
        /// </summary>
        public void DeleteGraphData()
        {
            SeriesCollection[0].Values.Clear();
            Value = "0";
        }

        /// <summary>
        /// Add point to low
        /// </summary>
        /// <param name="xstring">X coordinate</param>
        /// <param name="ystring">Y coordinate</param>
        /// <returns>True if point was added</returns>
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
                } else
                {
                    SeriesCollection[1].Values.Add(new ObservablePoint { X = x, Y = y });
                }
            }
            if (SeriesCollection[0].Values.Count > 0)
            {
                double lastDataXValue = ((ObservablePoint)SeriesCollection[0]
                        .Values[SeriesCollection[0].Values.Count - 1]).X;
                bool isPresent = false;
                foreach (ObservablePoint point in SeriesCollection[1].Values)
                {
                    if (point.X >= lastDataXValue)
                    {
                        Debug.WriteLine("found higher x value in low");
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

        /// <summary>
        /// Edit currently selected low mask point
        /// </summary>
        /// <param name="xstring"></param>
        /// <param name="ystring"></param>
        /// <returns>True if point was edited</returns>
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

        /// <summary>
        /// Delete point from low mask series
        /// </summary>
        /// <param name="xValue"></param>
        /// <returns>True if the point was deleted</returns>
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

        /// <summary>
        /// Add point to High mask
        /// </summary>
        /// <param name="xstring">X coordinate</param>
        /// <param name="ystring">Y coordinate</param>
        /// <returns>True if point was added</returns>
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
                } else
                {
                    SeriesCollection[2].Values.Add(new ObservablePoint { X = x, Y = y });
                }
            }
            if (SeriesCollection[0].Values.Count > 0)
            {
                double lastDataXValue = ((ObservablePoint)SeriesCollection[0]
                        .Values[SeriesCollection[0].Values.Count - 1]).X;
                bool isPresent = false;
                foreach (ObservablePoint point in SeriesCollection[2].Values)
                {
                    if (point.X >= lastDataXValue)
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

        /// <summary>
        /// Delete point from high mask
        /// </summary>
        /// <param name="xValue"></param>
        /// <returns>True if point was deleted</returns>
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

        /// <summary>
        /// Edit currently selected high mask point
        /// </summary>
        /// <param name="xstring"></param>
        /// <param name="ystring"></param>
        /// <returns></returns>
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

        /// <summary>
        /// Determine the action when a mask value point is clicked
        /// </summary>
        /// <param name="point"></param>
        /// <returns></returns>
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

        /// <summary>
        /// Test the masking of the data
        /// </summary>
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
                if (i == SeriesCollection[0].Values.Count - 1 && !isStart)
                {
                    end = point.X;
                    MaskValues.AddToMask(start, end);
                }
            }
            MaskText = MaskValues.GetMaskText();
        }
    }
}