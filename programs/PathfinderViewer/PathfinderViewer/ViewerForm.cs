using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathfinderViewer
{
    public partial class ViewerForm : Form
    {
        ConfigData m_config = new ConfigData();
        DataFile m_file = new DataFile();

        public ViewerForm()
        {
            m_file = new DataFile();
            InitializeComponent();

        }

        private void TrackerValueChanged(object sender, EventArgs e)
        {
        }

        private void LoadFileCommand(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "csv files (*.csv)|*.csv|All files (*.*)|*.*";
            dialog.FilterIndex = 0;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_file.Clear();
                try
                {
                    m_file.readFile(dialog.FileName);
                }
                catch
                {
                    m_file.Clear();
                }
            }

            updateWindows();
        }

        private void ExitCommand(object sender, EventArgs e)
        {
            Close();
        }

        private void updateWindows()
        {
            m_pathview.Data = m_file;
            if (m_file != null && m_file.Count > 0)
            {
                DataFile.DataFileElement elem = m_file["t"];
                m_pathview.SetPathDescriptors(m_config.Paths);
            }
        }
    }
}
