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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.doubleSlider1 = new PathfinderViewer.DoubleSlider();
            this.graphToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createPositionalToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createPositionVelocityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.graphToolStripMenuItem});
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
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(252, 30);
            this.loadToolStripMenuItem.Text = "Load Dataset ...";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.LoadFileCommand);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(252, 30);
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
            // graphToolStripMenuItem
            // 
            this.graphToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createPositionalToolStripMenuItem,
            this.createPositionVelocityToolStripMenuItem});
            this.graphToolStripMenuItem.Name = "graphToolStripMenuItem";
            this.graphToolStripMenuItem.Size = new System.Drawing.Size(72, 29);
            this.graphToolStripMenuItem.Text = "Graph";
            // 
            // createPositionalToolStripMenuItem
            // 
            this.createPositionalToolStripMenuItem.Name = "createPositionalToolStripMenuItem";
            this.createPositionalToolStripMenuItem.Size = new System.Drawing.Size(280, 30);
            this.createPositionalToolStripMenuItem.Text = "Create XY";
            // 
            // createPositionVelocityToolStripMenuItem
            // 
            this.createPositionVelocityToolStripMenuItem.Name = "createPositionVelocityToolStripMenuItem";
            this.createPositionVelocityToolStripMenuItem.Size = new System.Drawing.Size(280, 30);
            this.createPositionVelocityToolStripMenuItem.Text = "Create Position Velocity";
            // 
            // ViewerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1563, 776);
            this.Controls.Add(this.doubleSlider1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ViewerForm";
            this.Text = "Pathfinder Viewer";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private DoubleSlider doubleSlider1;
        private System.Windows.Forms.ToolStripMenuItem graphToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createPositionalToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createPositionVelocityToolStripMenuItem;
    }
}

