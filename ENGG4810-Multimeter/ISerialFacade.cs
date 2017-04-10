using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter
{
    public interface ISerialFacade
    {
        bool SetupConnection();

        int GetData();

        bool SendData();
    }
}
