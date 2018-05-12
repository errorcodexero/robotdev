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
        #region private member variables
        private double m_minimum;
        private double m_maximum;
        private double m_left;
        private double m_right;
        private int m_ticks;
        private bool m_labels;

        private Color m_range_color;
        private Color m_normal_color;
        private Color m_tick_color;
        private Color m_label_color;

        private int m_bar_height;
        private int m_bar_tick_spacing;
        private int m_tick_height;
        private int m_tick_label_spacing;
        #endregion

        #region public constructors
        public DoubleSlider()
        {
            m_minimum = 0.0;
            m_maximum = 100.0;
            m_left = 0.0;
            m_right = 100.0;
            m_ticks = 20;
            m_labels = true;

            m_bar_height = 12;
            m_bar_tick_spacing = 4;
            m_tick_height = 12;
            m_tick_label_spacing = 8;

            m_range_color = Color.LightGreen;
            m_normal_color = Color.LightBlue;
            m_tick_color = Color.DarkBlue;
            m_label_color = Color.Black;

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
            DrawTicks(e.Graphics);

            // DrawMarker(e.Graphics, m_left, Color.Red);
            // DrawMarker(e.Graphics, m_right, Color.Blue);
        }
        #endregion

        #region private methods
        private int GetPixel(double v)
        {
            int width = Width - Margin.Left - Margin.Right;
            double pos = (v - m_minimum) / (m_maximum - m_minimum);
            return (int)(pos * width);
        }

        private double GetValue(int pixel)
        {
            double pos = ((double)pixel - (double)Margin.Left) / (double)(Width - Margin.Left - Margin.Right);
            return pos * (m_maximum - m_minimum);
        }

        private void DrawBar(Graphics g)
        {
            using (Brush b = new SolidBrush(m_normal_color))
            {
                int width = GetPixel(m_left) - Margin.Left;
                Rectangle r = new Rectangle(Margin.Left, Margin.Top, width, m_bar_height);
                g.FillRectangle(b, r);

                width = Width - Margin.Right - GetPixel(m_right);
                r = new Rectangle(GetPixel(m_right), Margin.Top, width, m_bar_height);
                g.FillRectangle(b, r);
            }

            using (Brush b = new SolidBrush(m_range_color))
            {
                int width = GetPixel(m_right) - GetPixel(m_left);
                Rectangle r = new Rectangle(GetPixel(m_left), Margin.Top, width, m_bar_height);
                g.FillRectangle(b, r);
            }
        }

        private void DrawTicks(Graphics g)
        {
            if (m_ticks <= 1)
                return;

            using (Brush b = new SolidBrush(m_label_color))
            {
                using (Pen p = new Pen(m_tick_color))
                {
                    int each = (Width - Margin.Left - Margin.Right) / (m_ticks - 1);
                    int pos = Margin.Left;
                    int top = Margin.Top + m_bar_height + m_bar_tick_spacing;
                    for (int i = 0; i < m_ticks; i++)
                    {
                        g.DrawLine(p, pos, top, pos, top + m_tick_height);

                        if (m_labels)
                        {
                            string label = string.Format("{0:F1}", GetValue(pos));
                            SizeF size = g.MeasureString(label, Font);
                            float left = pos - size.Width / 2.0f;
                            float baseline = top + m_tick_height + m_tick_label_spacing + size.Height;
                            g.DrawString(label, Font, b, left, baseline);
                        }
                        pos += each;
                    }
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
            points[0].Y = Margin.Top + m_bar_height;

            points[1].X = xpos + markerwidth / 2;
            points[1].Y = Margin.Top + markermid + m_bar_height;

            points[2].X = xpos + markerwidth / 2;
            points[2].Y = Margin.Top + markerheight + m_bar_height;

            points[3].X = xpos - markerwidth / 2;
            points[3].Y = Margin.Top + markerheight + m_bar_height;

            points[4].X = xpos - markerwidth / 2;
            points[4].Y = Margin.Top + markermid + m_bar_height;

            using (Brush b = new SolidBrush(c))
            {
                g.FillPolygon(b, points);
            }
        }
        #endregion
    }
}
