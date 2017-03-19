using ENGG4810_Multimeter.Model;
using GalaSoft.MvvmLight;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;

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
        private ObservableCollection<DataModel> _ActiveValues;

        public ObservableCollection<DataModel> ActiveValues
        {
            get { return _ActiveValues; }
            set {
                _ActiveValues = value;
                RaisePropertyChanged("ActiveValues");
            }
        }

        private ObservableCollection<double> _tempPlot;

        public ObservableCollection<double> TempPlot
        {
            get { return _tempPlot; }
            set {
                _tempPlot = value;
                RaisePropertyChanged("TempPlot");
            }
        }

        //public List<DataModel> ActiveValues;

        public List<DataModel> AllValues;

        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            _ActiveValues = new ObservableCollection<DataModel>();
            ActiveValues = new ObservableCollection<DataModel>();

            TempPlot = new ObservableCollection<double>();

            StartPlot();
        }

        private void StartPlot()
        {
            var random = new Random();
            for (int i = 0; i < 10; i++)
            {
                TempPlot.Add(random.Next(0, 20));
            }
            Debug.Write(TempPlot.ToString());
        }
    }
}