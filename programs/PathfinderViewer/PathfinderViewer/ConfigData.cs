using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PathfinderViewer
{
    class ConfigData
    {
        public class PathDescriptor
        {
            #region private member variables
            private string m_name;
            private string m_x_name;
            private string m_y_name;
            private string m_theta_name;
            #endregion

            #region public constructors
            public PathDescriptor(string name, string x, string y, string theta)
            {
                m_name = name;
                m_x_name = x;
                m_y_name = y;
                m_theta_name = theta;
            }
            #endregion

            #region public properties
            public string Name
            {
                get { return m_name; }
            }

            public string XVarName
            {
                get { return m_x_name; }
            }

            public string YVarName
            {
                get { return m_y_name; }
            }

            public string ThetaName
            {
                get { return m_theta_name; }
            }
            #endregion
        }

        public class PosVelDesc
        {
            #region public enums
            public enum VarType
            {
                Position,
                Speed
            };
            #endregion

            #region private members
            private string m_name;
            private string m_variable;
            private VarType m_type;
            #endregion

            public PosVelDesc(string name, string var, VarType type)
            {
                m_name = name;
                m_variable = var;
                m_type = type;
            }

            public string Name
            {
                get { return m_name; }
            }

            public string Variable
            {
                get { return m_variable; }
            }

            public VarType VariableType
            {
                get { return m_type; }
            }
        };

        private IList<PathDescriptor> m_paths;
        private IList<PosVelDesc> m_posvels;

        public ConfigData()
        {
            m_paths = new List<PathDescriptor>();
            m_posvels = new List<PosVelDesc>();

            m_paths.Add(new PathDescriptor("Position", "pose_x", "pose_y", "pose_theta"));
            m_posvels.Add(new PosVelDesc("ActualPosition", "linear_disp", PosVelDesc.VarType.Position));
            m_posvels.Add(new PosVelDesc("ProfilePosition", "profile_disp", PosVelDesc.VarType.Position));
            m_posvels.Add(new PosVelDesc("ActualSpeed", "linear_vel", PosVelDesc.VarType.Speed));
            m_posvels.Add(new PosVelDesc("ProfileSpeed", "profile_vel", PosVelDesc.VarType.Speed));
        }

        public IList<PathDescriptor> Paths
        {
            get { return m_paths; }
        }

        public IList<PosVelDesc> PosVels
        {
            get { return m_posvels; }
        }
    }
}
