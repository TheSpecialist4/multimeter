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
        private static List<MaskValue> Values = new List<MaskValue>();

        public static void AddToMask(double start, double end)
        {
            //Debug.Write("values: ");
            //foreach (var value in Values)
            //{
            //    Debug.Write($"({value.Start}, {value.End}) ");
            //}
            //Debug.WriteLine("");
            //Debug.WriteLine($"checking for {start}, {end}");
            //bool found = false;
            //foreach (var value in Values)
            //{
            //   if (value.End <= start)
            //    {
            //        value.End = end;
            //        found = true;
            //        break;
            //    }
            //}
            //if (!found)
            //{
                Values.Add(new MaskValue { Start = start, End = end });
            //}
        }

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

        public static void ClearMasks()
        {
            Values.Clear();
        }
    }

    class MaskValue
    {
        public double Start { get; set; }
        public double End { get; set; }
    }
}
