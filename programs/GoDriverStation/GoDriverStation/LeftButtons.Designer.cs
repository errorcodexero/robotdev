namespace GoDriverStation
{
    partial class LeftButtons
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
            this.m_operation = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // m_operation
            // 
            this.m_operation.Image = global::GoDriverStation.Properties.Resources.Operation;
            this.m_operation.Location = new System.Drawing.Point(3, 3);
            this.m_operation.Name = "m_operation";
            this.m_operation.Size = new System.Drawing.Size(64, 64);
            this.m_operation.TabIndex = 0;
            this.m_operation.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Image = global::GoDriverStation.Properties.Resources.Diagnostics;
            this.button1.Location = new System.Drawing.Point(3, 73);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(64, 64);
            this.button1.TabIndex = 1;
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Image = global::GoDriverStation.Properties.Resources.Setup;
            this.button2.Location = new System.Drawing.Point(3, 143);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(64, 64);
            this.button2.TabIndex = 2;
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Image = global::GoDriverStation.Properties.Resources.USBDevices;
            this.button3.Location = new System.Drawing.Point(3, 213);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(64, 64);
            this.button3.TabIndex = 3;
            this.button3.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Image = global::GoDriverStation.Properties.Resources.PowerCan;
            this.button4.Location = new System.Drawing.Point(3, 283);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(64, 64);
            this.button4.TabIndex = 4;
            this.button4.UseVisualStyleBackColor = true;
            // 
            // LeftButtons
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.m_operation);
            this.Name = "LeftButtons";
            this.Size = new System.Drawing.Size(73, 351);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button m_operation;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
    }
}
