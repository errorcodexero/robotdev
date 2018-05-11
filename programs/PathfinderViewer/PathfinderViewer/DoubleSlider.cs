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
    public partial class DoubleSlider : UserControl
    {
        #region private constants
        private const int BarHeight = 10;
        #endregion

        #region private member variables
        private double m_minimum;
        private double m_maximum;
        private double m_left;
        private double m_right;
        private int m_labels;
        #endregion

        #region public constructors
        public DoubleSlider()
        {
            m_minimum = 0.0;
            m_maximum = 100.0;
            m_left = 0.0;
            m_right = 100.0;
            m_labels = 10;

            SetBoundsCore(0, 0, 0, 60, BoundsSpecified.Height);
            InitializeComponent();
        }
        #endregion

        #region public properties
        public double Maximum
        {
            get { return m_maximum; }
            set
            {
                if (value <= m_minimum)
                    throw new Exception("invalid value for maximum, less than minimum");
                m_maximum = value;
                Invalidate();
            }
        }

        public double Minimum
        {
            get { return m_minimum; }
            set
            {
                if (value >= m_maximum)
                    throw new Exception("invalid value for minimum, greater than maximum");
                m_minimum = value;
                Invalidate();
            }
        }

        public double LeftValue
        {
            get { return m_left; }
            set
            {
                if (value > m_right)
                    throw new Exception("invalid value for left, greater than right");
                if (value < m_minimum)
                    m_left = m_minimum;
                else if (value > m_maximum)
                    m_left = m_maximum;
                else
                    m_left = value;

                Invalidate();
            }
        }

        public double RightValue
        {
            get { return m_right; }
            set
            {
                if (value < m_left)
                    throw new Exception("invalid value for right, less than left");
                if (value < m_minimum)
                    m_right = m_minimum;
                else if (value > m_maximum)
                    m_right = m_maximum;
                else
                    m_right = value;

                Invalidate();
            }
        }
        #endregion

        #region public methods
        #endregion

        #region protected methods
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            DrawBar(e.Graphics);
            DrawLabels(e.Graphics);
            DrawMarker(e.Graphics, m_left, Color.Red);
            DrawMarker(e.Graphics, m_right, Color.Blue);
        }
        #endregion

        #region private methods
        private void DrawBar(Graphics g)
        {
            using (Brush b = new SolidBrush(Color.DarkGray))
            {
                Rectangle r = new Rectangle(Margin.Left, Margin.Top, Width - Margin.Left - Margin.Right, BarHeight);
                g.FillRectangle(b, r);
            }
        }

        private void DrawLabels(Graphics g)
        {
            int width = Width - Margin.Left - Margin.Right;
            int delta = width / (m_labels - 1);

            using (Brush b = new SolidBrush(Color.Black))
            {
                for (int i = 0; i < m_labels; i++)
                {
                    int xpos = 0;
                    double value = m_minimum + (m_maximum - m_minimum) / (m_labels - 1)  * i;
                    string label = String.Format("{0:F2}", value);
                    SizeF size = g.MeasureString(label, Font);

                    if (i == 0)
                    {
                        xpos = Margin.Left;
                    }
                    else if (i == m_labels - 1)
                    {
                        xpos = Width - Margin.Right - (int)size.Width;
                    }
                    else
                    {
                        xpos = Margin.Left + i * delta - (int)size.Width / 2;
                    }

                    g.DrawString(label, Font, b, xpos, Height - Margin.Bottom - size.Height);
                }
            }
        }

        private void DrawMarker(Graphics g, double value, Color c)
        {
            int markerwidth = 12;
            int markermid = 12;
            int markerheight = 24;

            Point[] points = new Point[5];

            double pcnt = (value - m_minimum) / (m_maximum - m_minimum);
            int xpos = (int)(pcnt * (Width - Margin.Left - Margin.Right)) + Margin.Left;

            points[0].X = xpos;
            points[0].Y = Margin.Top + BarHeight;

            points[1].X = xpos + markerwidth / 2;
            points[1].Y = Margin.Top + markermid + BarHeight;

            points[2].X = xpos + markerwidth / 2;
            points[2].Y = Margin.Top + markerheight + BarHeight;

            points[3].X = xpos - markerwidth / 2;
            points[3].Y = Margin.Top + markerheight + BarHeight;

            points[4].X = xpos - markerwidth / 2;
            points[4].Y = Margin.Top + markermid + BarHeight;

            using (Brush b = new SolidBrush(c))
            {
                g.FillPolygon(b, points);
            }
        }
        #endregion
    }
}
