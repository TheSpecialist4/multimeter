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
        private static SerialFacade INSTANCE;
        private SerialPort port;
        private bool isConnected = false;

        //public bool NewDataReceived { get; set; }
        //public int NewDataCount { get; set; }

        public ObservableCollection<string> IncomingData { get; set; }

        private SerialFacade()
        {
            IncomingData = new ObservableCollection<string>();
        }

        public static SerialFacade GetInstance()
        {
            if (INSTANCE == null)
            {
                INSTANCE = new SerialFacade();
            }
            return INSTANCE;
        }

        public int GetData()
        {
            throw new NotImplementedException();
        }

        public bool SendData()
        {
            throw new NotImplementedException();
        }

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
                            while (this.isConnected && port.IsOpen)
                            {
                                int counter = 0;
                                byte checker = 0;
                                byte[] buffer = new byte[4];
                                
                                checker = (byte)port.ReadByte();
                                while (checker != 0)
                                {
                                    buffer[counter++] = checker;
                                    checker = (byte)port.ReadByte();
                                }
                                if (counter == 4)
                                {
                                    Debug.WriteLine("sending ack for zero");
                                    buffer[0] = 0xFF;
                                    port.Write(buffer, 0, 1);
                                }
                                counter = 0;
                                Debug.Write("buffer: " + BitConverter.ToString(buffer));
                                Debug.WriteLine(" float " + BitConverter.ToSingle(buffer, 0));
                                IncomingData.Add(BitConverter.ToSingle(buffer, 0).ToString());
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

        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (!isConnected)
            {
                ClosePort();
            }
        }

        public void ClosePort()
        {
            if (port != null)
            {
                port.Close();
            }
        }

        public bool OpenPort()
        {
            if (port != null && port.IsOpen)
            {
                port.Open();
                return true;
            }
            return SetupConnection();
        }

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

        private List<string> GetPortNames()
        {
            List<string> ports = new List<string>();
            foreach (var port in SerialPort.GetPortNames())
            {
                ports.Add(port);
            }
            return ports;
        }

        public void SendZero()
        {
            byte[] buffer = new byte[1];
            buffer[0] = 0;
            port.Write(buffer, 0, 1);
        }
    }
}
