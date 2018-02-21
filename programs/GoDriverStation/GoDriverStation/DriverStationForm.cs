using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GoDriverStation
{
    public partial class DriverStationForm : Form
    {
        private JoystickManager m_joysticks;

        public DriverStationForm()
        {
            InitializeComponent();

            m_joysticks = new JoystickManager();
        }

        protected override void OnLoad(EventArgs e)
        {
            Left = 0;
            Top = Screen.PrimaryScreen.Bounds.Height - Height;
            Width = Screen.PrimaryScreen.Bounds.Width;
            base.OnLoad(e);
        }
    }
}
