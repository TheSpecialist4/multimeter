using ENGG4810_Multimeter.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter
{
    public class SerialFacade : ISerialFacade
    {
        /// <summary>
        /// Single instance of the class used throughout the application
        /// </summary>
        private static SerialFacade INSTANCE;
        /// <summary>
        /// The port connected to the hardware device
        /// </summary>
        private SerialPort port;
        /// <summary>
        /// True if the port is connected
        /// </summary>
        private bool isConnected = false;

        private MainViewModel vm;

        /// <summary>
        /// Data received on the port
        /// </summary>
        public ObservableCollection<string> IncomingData { get; set; }

        private SerialFacade()
        {
            IncomingData = new ObservableCollection<string>();
        }

        public void SetVM(MainViewModel vm)
        {
            this.vm = vm;
        }

        /// <summary>
        /// Returns the single instance of this class used by the entire application
        /// </summary>
        /// <returns></returns>
        public static SerialFacade GetInstance()
        {
            if (INSTANCE == null)
            {
                INSTANCE = new SerialFacade();
            }
            return INSTANCE;
        }

        /// <summary>
        /// Gets the data received
        /// </summary>
        /// <returns></returns>
        public int GetData()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Send data to the port
        /// </summary>
        /// <returns></returns>
        public bool SendData()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Sets up the serial port.
        /// </summary>
        /// <returns>True if connection was established. False otherwise</returns>
        public bool SetupConnection()
        {
            foreach (var portName in GetPortNames())
            {
                port = new SerialPort(portName);
                port.Open();
                System.Timers.Timer timer = new System.Timers.Timer(5000);
                timer.Elapsed += Timer_Elapsed;
                timer.Start();
                try
                {
                    if (port.ReadChar() != 0 && port.BytesToRead > 0)
                    {
                        isConnected = true;
                        Debug.Print($"Port connected successfully on {portName}");
                        byte[] ack = new byte[1];
                        ack[0] = 0xFF;
                        port.Write(ack, 0, 1);
                        Task.Factory.StartNew(() =>
                        {
                            bool isFirst = true;
                            while (this.isConnected && port.IsOpen)
                            {
                                try
                                {
                                    timer.Start();
                                    int firstByte = 0;
                                    firstByte = port.ReadByte();
                                    if (firstByte <= 3)
                                    {
                                        switch (firstByte)
                                        {
                                            case 0:
                                                //brightness
                                                vm.BrightnessValue = port.ReadByte().ToString();
                                                break;
                                            case 1:
                                                //sample mode
                                                vm.SwitchDataTypeMode(port.ReadByte());
                                                IncomingData.Clear();
                                                break;
                                            case 2:
                                                //sample rate
                                                //1 byte
                                                break;
                                            case 3:
                                                //value
                                                port.ReadByte();
                                                byte[] buffer = new byte[4];
                                                port.Read(buffer, 0, 4);
                                                IncomingData.Add(BitConverter.ToSingle(buffer, 0).ToString());
                                                if (isFirst)
                                                {
                                                    port.Write(ack, 0, 1);
                                                    isFirst = false;
                                                }
                                                break;
                                        }
                                    }
                                }
                                catch (Exception) { }
                            }
                        });
                        timer.Close();
                        return true;
                    }
                } catch (Exception e)
                {
                    Debug.Print("couldnt connect to port");
                    return false;
                }
                port.Close();
            }
            Debug.Print("couldnt connect to any port");
            return false;
        }

        /// <summary>
        /// timer to quit trying to connect to port
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (!isConnected)
            {
                ClosePort();
            }
        }

        /// <summary>
        /// Close the serial port
        /// </summary>
        public void ClosePort()
        {
            if (port != null)
            {
                port.Close();
            }
        }

        /// <summary>
        /// Open the port
        /// </summary>
        /// <returns>True if port was opened</returns>
        public bool OpenPort()
        {
            if (port != null && port.IsOpen)
            {
                port.Open();
                return true;
            }
            return SetupConnection();
        }

        /// <summary>
        /// Data received on the port is appropriately interpreted
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //var value = port.ReadExisting();
            //port.ReadByte();
            byte[] buffer = new byte[4];
            port.Read(buffer, 0, 4);
            float f = BitConverter.ToSingle(buffer, 0);
            
            Debug.WriteLine("buffer " + BitConverter.ToString(buffer));
            Debug.WriteLine($"{f} read");
            //Debug.WriteLine(buffer[0] + " read");
            //}
            //IncomingData.Add(calculatedValue.ToString());
            //Debug.Print($"{IncomingData[IncomingData.Count - 1]} received and added to graph\n");

            byte[] ack = new byte[1];
            ack[0] = 0xff;
            port.Write(ack, 0, 1);
        }

        /// <summary>
        /// Returns a list of all the port names on the machine
        /// </summary>
        /// <returns></returns>
        private List<string> GetPortNames()
        {
            List<string> ports = new List<string>();
            foreach (var port in SerialPort.GetPortNames())
            {
                ports.Add(port);
            }
            return ports;
        }

        /// <summary>
        /// Sends zero to the port
        /// </summary>
        public void SendZero()
        {
            byte[] buffer = new byte[1];
            buffer[0] = 0;
            if (port != null)
            {
                port.Write(buffer, 0, 1);
            }
        }

        public bool SendBrightnessValue(int value)
        {
            try
            {
                byte[] buffer = new byte[2];
                buffer[0] = 0;
                buffer[1] = (byte)value;
                port.Write(buffer, 0, 2);
                return true;
            } catch(Exception)
            {
                return false;
            }
        }

        public bool SendMode(int value)
        {
            try
            {
                byte[] buffer = new byte[2];
                buffer[0] = 1;
                buffer[1] = (byte)value;
                port.Write(buffer, 0, 2);
                return true;
            }
            catch (Exception) { return false; }
        }
    }
}
