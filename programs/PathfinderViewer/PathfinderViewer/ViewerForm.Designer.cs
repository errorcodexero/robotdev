namespace PathfinderViewer
{
    partial class ViewerForm
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
            this.m_panel = new System.Windows.Forms.Panel();
            this.m_splitter = new System.Windows.Forms.SplitContainer();
            this.m_pathview = new PathfinderViewer.PathViewControl();
            this.m_posvelview = new PathfinderViewer.PositionVelocityViewControl();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.doubleSlider1 = new PathfinderViewer.DoubleSlider();
            this.m_panel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_splitter)).BeginInit();
            this.m_splitter.Panel1.SuspendLayout();
            this.m_splitter.Panel2.SuspendLayout();
            this.m_splitter.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_panel
            // 
            this.m_panel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_panel.Controls.Add(this.m_splitter);
            this.m_panel.Location = new System.Drawing.Point(12, 36);
            this.m_panel.Name = "m_panel";
            this.m_panel.Size = new System.Drawing.Size(1539, 658);
            this.m_panel.TabIndex = 0;
            // 
            // m_splitter
            // 
            this.m_splitter.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_splitter.Location = new System.Drawing.Point(0, 0);
            this.m_splitter.Name = "m_splitter";
            // 
            // m_splitter.Panel1
            // 
            this.m_splitter.Panel1.Controls.Add(this.m_pathview);
            // 
            // m_splitter.Panel2
            // 
            this.m_splitter.Panel2.Controls.Add(this.m_posvelview);
            this.m_splitter.Size = new System.Drawing.Size(1539, 658);
            this.m_splitter.SplitterDistance = 770;
            this.m_splitter.TabIndex = 0;
            // 
            // m_pathview
            // 
            this.m_pathview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_pathview.Data = null;
            this.m_pathview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_pathview.Location = new System.Drawing.Point(0, 0);
            this.m_pathview.Name = "m_pathview";
            this.m_pathview.Size = new System.Drawing.Size(770, 658);
            this.m_pathview.TabIndex = 0;
            // 
            // m_posvelview
            // 
            this.m_posvelview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_posvelview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_posvelview.Location = new System.Drawing.Point(0, 0);
            this.m_posvelview.Name = "m_posvelview";
            this.m_posvelview.Size = new System.Drawing.Size(765, 658);
            this.m_posvelview.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1563, 33);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(50, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(135, 30);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadFileCommand);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(135, 30);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.ExitCommand);
            // 
            // doubleSlider1
            // 
            this.doubleSlider1.LeftValue = 25D;
            this.doubleSlider1.Location = new System.Drawing.Point(12, 700);
            this.doubleSlider1.Margin = new System.Windows.Forms.Padding(6, 3, 6, 3);
            this.doubleSlider1.Maximum = 100D;
            this.doubleSlider1.Minimum = 0D;
            this.doubleSlider1.Name = "doubleSlider1";
            this.doubleSlider1.RightValue = 75D;
            this.doubleSlider1.Size = new System.Drawing.Size(1539, 60);
            this.doubleSlider1.TabIndex = 3;
            // 
            // ViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1563, 776);
            this.Controls.Add(this.doubleSlider1);
            this.Controls.Add(this.m_panel);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ViewerForm";
            this.Text = "Pathfinder Viewer";
            this.m_panel.ResumeLayout(false);
            this.m_splitter.Panel1.ResumeLayout(false);
            this.m_splitter.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.m_splitter)).EndInit();
            this.m_splitter.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel m_panel;
        private System.Windows.Forms.SplitContainer m_splitter;
        private PathViewControl m_pathview;
        private PositionVelocityViewControl m_posvelview;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private DoubleSlider doubleSlider1;
    }
}

