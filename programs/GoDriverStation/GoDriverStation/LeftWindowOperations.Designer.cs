namespace GoDriverStation
{
    partial class LeftWindowOperations
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
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.m_disable = new System.Windows.Forms.Button();
            this.m_enable = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.m_team = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(20, 28);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(131, 24);
            this.radioButton1.TabIndex = 0;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "TeleOperated";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(20, 58);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(125, 24);
            this.radioButton2.TabIndex = 1;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "Autonomous";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(20, 88);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(91, 24);
            this.radioButton3.TabIndex = 2;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "Practice";
            this.radioButton3.UseVisualStyleBackColor = true;
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Location = new System.Drawing.Point(20, 118);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(65, 24);
            this.radioButton4.TabIndex = 3;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "Test";
            this.radioButton4.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.m_disable);
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Controls.Add(this.m_enable);
            this.groupBox1.Controls.Add(this.radioButton2);
            this.groupBox1.Controls.Add(this.radioButton4);
            this.groupBox1.Controls.Add(this.radioButton3);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(238, 261);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Mode";
            // 
            // m_disable
            // 
            this.m_disable.ForeColor = System.Drawing.Color.Red;
            this.m_disable.Location = new System.Drawing.Point(136, 170);
            this.m_disable.Name = "m_disable";
            this.m_disable.Size = new System.Drawing.Size(96, 78);
            this.m_disable.TabIndex = 3;
            this.m_disable.Text = "Disable";
            this.m_disable.UseVisualStyleBackColor = true;
            // 
            // m_enable
            // 
            this.m_enable.ForeColor = System.Drawing.Color.Green;
            this.m_enable.Location = new System.Drawing.Point(11, 170);
            this.m_enable.Name = "m_enable";
            this.m_enable.Size = new System.Drawing.Size(96, 78);
            this.m_enable.TabIndex = 2;
            this.m_enable.Text = "Enable";
            this.m_enable.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 287);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 20);
            this.label1.TabIndex = 2;
            this.label1.Text = "Team Station";
            // 
            // m_team
            // 
            this.m_team.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_team.FormattingEnabled = true;
            this.m_team.Items.AddRange(new object[] {
            "Red 1",
            "Red 2",
            "Red 3",
            "Blue 1",
            "Blue 2",
            "Blue 3"});
            this.m_team.Location = new System.Drawing.Point(120, 284);
            this.m_team.Name = "m_team";
            this.m_team.Size = new System.Drawing.Size(121, 28);
            this.m_team.TabIndex = 3;
            // 
            // LeftWindowOperations
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_team);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox1);
            this.Name = "LeftWindowOperations";
            this.Size = new System.Drawing.Size(256, 346);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button m_disable;
        private System.Windows.Forms.Button m_enable;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox m_team;
    }
}
