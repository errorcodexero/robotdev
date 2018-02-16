namespace GoDriverStation
{
    partial class LeftWindow
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_operations = new GoDriverStation.LeftWindowOperations();
            this.SuspendLayout();
            // 
            // m_operations
            // 
            this.m_operations.Location = new System.Drawing.Point(2, 2);
            this.m_operations.Name = "m_operations";
            this.m_operations.Size = new System.Drawing.Size(256, 346);
            this.m_operations.TabIndex = 0;
            // 
            // LeftWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.m_operations);
            this.Name = "LeftWindow";
            this.Size = new System.Drawing.Size(256, 350);
            this.ResumeLayout(false);

        }

        #endregion

        private LeftWindowOperations m_operations;
    }
}
