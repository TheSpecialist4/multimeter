using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter
{
    public class SerialFacade : ISerialFacade
    {
        private static SerialFacade INSTANCE;
        private SerialPort port;

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
                var port = new SerialPort(portName);
                port.Open();
                if (port.ReadChar() != 0)
                {
                    this.port = port;
                    this.port.DataReceived += Port_DataReceived;
                    Debug.Print($"Port connected successfully on {portName}");
                    return true;
                }
                port.Close();
            }
            Debug.Print("couldnt connect to any port");
            return false;
        }

        private void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            IncomingData.Add(port.ReadExisting());
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
    }
}
