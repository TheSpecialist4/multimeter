using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ENGG4810_Multimeter
{
    public class MaskValues
    {
        /// <summary>
        /// list of all the intervals where the data fails
        /// </summary>
        private static List<MaskValue> Values = new List<MaskValue>();

        /// <summary>
        /// Add an interval to mask
        /// </summary>
        /// <param name="start"></param>
        /// <param name="end"></param>
        public static void AddToMask(double start, double end)
        {
            Values.Add(new MaskValue { Start = start, End = end });
        }

        /// <summary>
        /// returns the formatted text of all intervals
        /// </summary>
        /// <returns>(interval 1)\n(interval 2)...</returns>
        public static string GetMaskText()
        {
            StringBuilder text = new StringBuilder();
            foreach (var value in Values)
            {
                text.Append($"({Math.Round(value.Start, 2)}, {Math.Round(value.End, 2)})");
                text.Append(Environment.NewLine);
            }
            return text.ToString();
        }

        /// <summary>
        /// clears all the interval values
        /// </summary>
        public static void ClearMasks()
        {
            Values.Clear();
        }
    }

    /// <summary>
    /// Defines an interval
    /// </summary>
    class MaskValue
    {
        /// <summary>
        /// the starting point of the interval
        /// </summary>
        public double Start { get; set; }
        /// <summary>
        /// ending point of the interval
        /// </summary>
        public double End { get; set; }
    }
}
