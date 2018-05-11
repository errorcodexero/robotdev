using System;
using System.IO;
using System.Collections.Generic;

namespace PathfinderViewer
{
    class DataFile
    {
        #region public classes
        public class DataFileElement
        {
            #region private members
            private string m_name;
            private double[] m_values;
            private double m_max;
            private double m_min;
            #endregion

            public DataFileElement(string name)
            {
                m_name = name;
                m_values = new double[0];
                m_max = Double.MinValue;
                m_min = Double.MaxValue;
            }

            public double[] Values
            {
                get { return m_values; }
            }

            public double Maximum
            {
                get { return m_max; }
            }

            public double Minimum
            {
                get { return m_min; }
            }

            public void addValue(double value)
            {
                int newsize = m_values.Length + 1;
                Array.Resize<double>(ref m_values, newsize);
                m_values[newsize - 1] = value;

                if (value > m_max)
                    m_max = value;

                if (value < m_min)
                    m_min = value;
            }
        }
        #endregion

        #region private members
        IDictionary<string, DataFileElement> m_elements;
        IList<DataFileElement> m_element_array;
        #endregion

        #region public constructors
        public DataFile()
        {
            m_elements = new Dictionary<string, DataFileElement>();
            m_element_array = new List<DataFileElement>();
        }
        #endregion

        #region public properties
        public DataFileElement this[string name]
        {
            get
            {
                if (m_elements.ContainsKey(name))
                    return m_elements[name];

                return null;
            }
        }

        public int Count
        {
            get { return m_elements.Count; }
        }
        #endregion

        #region public methods
        public void Clear()
        {
            m_element_array.Clear();
            m_elements.Clear();
        }

        public void readFile(string filename)
        {
            string line;
            StreamReader rdr = new StreamReader(filename);
            int which = 0;
            string[] fields;

            while ((line = rdr.ReadLine()) != null)
            {
                fields = line.Split(',');

                if (which == 0)
                {
                    createFields(fields);
                }
                else
                {
                    putData(fields);
                }

                which++;
            }
        }
        #endregion

        #region private members
        private void createFields(string [] names)
        {
            foreach(string name in names)
            {
                DataFileElement elem = new DataFileElement(name);
                m_elements[name] = elem;
                m_element_array.Add(elem);
            }
        }

        private void putData(string [] data)
        {
            if (data.Length != m_element_array.Count)
            {
                m_elements.Clear();
                m_element_array.Clear();
                throw new Exception("Invalid data line in data file - wrong number of elements");
            }

            for(int i = 0; i < m_element_array.Count; i++)
            {
                double value;
                
                if (data[i].Length == 0)
                {
                    value = 0;
                }
                else if (data[i] == "inf")
                {
                    value = Double.PositiveInfinity;
                }
                else if (!Double.TryParse(data[i], out value))
                {
                    m_elements.Clear();
                    m_element_array.Clear();
                    throw new Exception("Invalid data line in data file - invalid value");
                }

                m_element_array[i].addValue(value);
            }
        }
        #endregion
    }
}
