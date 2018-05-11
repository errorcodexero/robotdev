using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PathfinderViewer
{
    partial class PathViewControl : UserControl
    {
        public class Path
        {
            #region private variables
            private PathViewControl m_parent;
            private ConfigData.PathDescriptor m_desc;

            private DataFile.DataFileElement m_time;
            private DataFile.DataFileElement m_x;
            private DataFile.DataFileElement m_y;
            private DataFile.DataFileElement m_theta;

            #endregion

            public Path(PathViewControl parent, ConfigData.PathDescriptor desc)
            {
                m_parent = parent;
                m_desc = desc;

                m_time = parent.Data["t"];
                m_x = parent.Data[desc.XVarName];
                m_y = parent.Data[desc.YVarName];
                m_theta = parent.Data[desc.ThetaName];
            }

            public DataFile.DataFileElement X
            {
                get { return m_x; }
            }

            public DataFile.DataFileElement Y
            {
                get { return m_y; }
            }

            public DataFile.DataFileElement Theta
            {
                get { return m_theta; }
            }
        }

        #region private variables
        private List<Path> m_paths;
        private DataFile m_file;
        int m_start_sample;
        int m_end_sample;
        bool m_auto_boundaries;

        private double m_minx;
        private double m_maxx;
        private double m_miny;
        private double m_maxy;

        private int m_left_boundary;
        private int m_right_boundary;
        private int m_top_boundary;
        private int m_bottom_boundary;
        #endregion

        #region public constructors
        public PathViewControl()
        {
            m_paths = new List<Path>();
            m_left_boundary = 3;
            m_right_boundary = 3;
            m_top_boundary = 3;
            m_bottom_boundary = 3;

            m_auto_boundaries = true;
            InitializeComponent();
        }
        #endregion

        #region public properties
        public DataFile Data
        {
            get { return m_file; }
            set 
            {
                m_start_sample = 0;
                m_end_sample = 0;

                m_file = value;
                if (m_file != null)
                {
                    DataFile.DataFileElement elem = m_file["t"];
                    if (elem != null)
                    {
                        m_start_sample = 0;
                        m_end_sample = elem.Values.Length;
                    }
                }
            }
        }
        #endregion

        #region public methods
        public void SetPathDescriptors(IList<ConfigData.PathDescriptor> list)
        {
            m_paths.Clear();

            foreach (ConfigData.PathDescriptor desc in list)
                m_paths.Add(new Path(this, desc));

            if (m_auto_boundaries)
                FindBounds();

            Invalidate();
        }

        public void Clear()
        {
            m_paths.Clear();
        }

        public void AddPath(Path p)
        {
            m_paths.Add(p);
            if (m_auto_boundaries)
                FindBounds();
        }
        #endregion

        #region protected methods
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (m_paths.Count > 0)
                DrawPlot(e.Graphics);
        }
        #endregion

        #region private methods
        private void DrawPlot(Graphics g)
        {
            double scalex = (Size.Width - m_left_boundary - m_right_boundary) / (m_maxx - m_minx);
            double scaley = (Size.Height - m_top_boundary - m_bottom_boundary) / (m_maxy - m_miny);
            double scale = (scalex < scaley) ? scalex : scaley;

            DrawPlot(g, scale);
        }

        private void DrawPlot(Graphics g, double scale)
        {
            using (Pen pen = new Pen(Color.Blue))
            {
                foreach (Path p in m_paths)
                {
                    DrawPath(g, scale, p, pen);
                }
            }
        }

        private void DrawPath(Graphics g, double scale, Path p, Pen pen)
        {
            Point prev = new Point(0, 0);

            for (int i = m_start_sample; i < m_end_sample; i++)
            {
                int px = (int)((p.X.Values[i] - m_minx) * scale) + m_left_boundary;
                int py = Size.Height - (int)((p.Y.Values[i] - m_miny) * scale) - m_bottom_boundary;
                Point curr = new Point(px, py);

                if (i != m_start_sample)
                {
                    g.DrawLine(pen, prev, curr);
                }

                prev = curr;
            }
        }

        private void FindBounds()
        {
            m_minx = Double.MaxValue;
            m_maxx = Double.MinValue;
            m_miny = Double.MaxValue;
            m_maxy = Double.MinValue;

            foreach(Path p in m_paths)
            {
                if (p.X.Minimum < m_minx)
                    m_minx = p.X.Minimum;

                if (p.X.Maximum > m_maxx)
                    m_maxx = p.X.Maximum;

                if (p.Y.Minimum < m_miny)
                    m_miny = p.Y.Minimum;

                if (p.Y.Maximum > m_maxy)
                    m_maxy = p.Y.Maximum;
            }

            if (Math.Abs(m_minx - m_maxx) < 0.1)
            {
                m_minx = 0.8 * m_minx;
                m_maxx = 1.2 * m_maxx;
            }

            if (Math.Abs(m_miny - m_maxy) < 0.1)
            {
                m_miny = 0.8 * m_miny;
                m_maxy = 1.2 * m_maxy;
            }
        }
        #endregion
    }
}
