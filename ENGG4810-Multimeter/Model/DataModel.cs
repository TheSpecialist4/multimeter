using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter.Model
{
    /// <summary>
    /// The model of the data in the chart
    /// </summary>
    public class DataModel
    {
        /// <summary>
        /// X axis value
        /// </summary>
        public string X { get; set; }
        /// <summary>
        /// Y axis value
        /// </summary>
        public string Y { get; set; }
        /// <summary>
        /// The timestamp of the data
        /// </summary>
        public string TimeStamp { get; set; }
    }
}
