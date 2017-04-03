using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter
{
    public class SerialFacade
    {
        public static SerialPort GetConnectedPort()
        {
            var allPorts = GetAllPorts();

            var connectedPort = GetRequiredPort(allPorts);

            return connectedPort;
        }

        private static List<string> GetAllPorts()
        {
            var allPorts = new List<string>();
            foreach (var port in SerialPort.GetPortNames())
            {
                allPorts.Add(port);
            }
            return allPorts;
        }

        private static SerialPort GetRequiredPort(List<string> allPorts)
        {
            for (int i = 0; i < allPorts.Count; i++)
            {
                char[] inBuffer = new char[5]; 
                SerialPort tempPort = new SerialPort(allPorts[i]);
                tempPort.Open();
                tempPort.Read(inBuffer, 0, 5);
                if (char.IsDigit(inBuffer[0]))
                {
                    return tempPort;
                }
                tempPort.Close();
            }
            return null;
        }

        public static void StartSerialReceive(SerialPort port)
        {
            char[] inBuffer = new char[5];

            int index = 0;
            while (index < 5 && inBuffer[index] != '0')
            {
                Debug.Print(inBuffer[index++].ToString());
                Debug.Print("\n");
            }
        }
    }
}
