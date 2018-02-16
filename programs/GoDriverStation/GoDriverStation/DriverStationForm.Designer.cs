namespace GoDriverStation
{
    partial class DriverStationForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_right_buttons = new GoDriverStation.RightButtons();
            this.m_right_window = new GoDriverStation.RightWindow();
            this.m_team_window = new GoDriverStation.TeamWindow();
            this.m_left_window = new GoDriverStation.LeftWindow();
            this.m_left_buttons = new GoDriverStation.LeftButtons();
            this.SuspendLayout();
            // 
            // m_right_buttons
            // 
            this.m_right_buttons.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_right_buttons.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_right_buttons.Location = new System.Drawing.Point(1852, 2);
            this.m_right_buttons.Name = "m_right_buttons";
            this.m_right_buttons.Size = new System.Drawing.Size(74, 350);
            this.m_right_buttons.TabIndex = 4;
            // 
            // m_right_window
            // 
            this.m_right_window.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_right_window.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_right_window.Location = new System.Drawing.Point(676, 2);
            this.m_right_window.Name = "m_right_window";
            this.m_right_window.Size = new System.Drawing.Size(1170, 350);
            this.m_right_window.TabIndex = 3;
            // 
            // m_team_window
            // 
            this.m_team_window.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_team_window.Location = new System.Drawing.Point(340, 2);
            this.m_team_window.Name = "m_team_window";
            this.m_team_window.Size = new System.Drawing.Size(330, 350);
            this.m_team_window.TabIndex = 2;
            // 
            // m_left_window
            // 
            this.m_left_window.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_left_window.Location = new System.Drawing.Point(80, 2);
            this.m_left_window.Name = "m_left_window";
            this.m_left_window.Size = new System.Drawing.Size(256, 350);
            this.m_left_window.TabIndex = 1;
            // 
            // m_left_buttons
            // 
            this.m_left_buttons.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_left_buttons.Location = new System.Drawing.Point(2, 2);
            this.m_left_buttons.Name = "m_left_buttons";
            this.m_left_buttons.Size = new System.Drawing.Size(74, 350);
            this.m_left_buttons.TabIndex = 0;
            // 
            // DriverStationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1931, 355);
            this.Controls.Add(this.m_right_buttons);
            this.Controls.Add(this.m_right_window);
            this.Controls.Add(this.m_team_window);
            this.Controls.Add(this.m_left_window);
            this.Controls.Add(this.m_left_buttons);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "DriverStationForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "DriverStations";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private LeftButtons m_left_buttons;
        private LeftWindow m_left_window;
        private TeamWindow m_team_window;
        private RightWindow m_right_window;
        private RightButtons m_right_buttons;
    }
}

