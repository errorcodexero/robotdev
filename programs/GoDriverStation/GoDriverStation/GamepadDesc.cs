using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoDriverStation
{
    class GamepadDesc
    {
        #region private member variables
        private Guid m_guid;
        private string m_name;
        #endregion

        #region public contstructors
        public GamepadDesc(string name, Guid g)
        {
            m_name = name;
            m_guid = g;
        }
        #endregion

        #region public properties
        public string Name
        {
            get { return m_name; }
        }

        public Guid GUID
        {
            get { return m_guid; }
        }
        #endregion
    }
}
