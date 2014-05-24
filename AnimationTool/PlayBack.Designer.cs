namespace AnimationTool
{
    partial class PlayBack
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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.showDrawRect = new System.Windows.Forms.CheckBox();
            this.showCollideRect = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 189);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Play";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(103, 189);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 1;
            this.button2.Text = "Pause";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(195, 189);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 2;
            this.button3.Text = "Restart";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // showDrawRect
            // 
            this.showDrawRect.AutoSize = true;
            this.showDrawRect.Location = new System.Drawing.Point(26, 49);
            this.showDrawRect.Name = "showDrawRect";
            this.showDrawRect.Size = new System.Drawing.Size(152, 21);
            this.showDrawRect.TabIndex = 3;
            this.showDrawRect.Text = "Show Drawing Rect";
            this.showDrawRect.UseVisualStyleBackColor = true;
            this.showDrawRect.CheckedChanged += new System.EventHandler(this.showDrawRect_CheckedChanged);
            // 
            // showCollideRect
            // 
            this.showCollideRect.AutoSize = true;
            this.showCollideRect.Location = new System.Drawing.Point(26, 85);
            this.showCollideRect.Name = "showCollideRect";
            this.showCollideRect.Size = new System.Drawing.Size(153, 21);
            this.showCollideRect.TabIndex = 4;
            this.showCollideRect.Text = "Show Collision Rect";
            this.showCollideRect.UseVisualStyleBackColor = true;
            this.showCollideRect.CheckedChanged += new System.EventHandler(this.showCollideRect_CheckedChanged);
            // 
            // PlayBack
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(282, 255);
            this.Controls.Add(this.showCollideRect);
            this.Controls.Add(this.showDrawRect);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "PlayBack";
            this.Text = "PlayBack";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.CheckBox showDrawRect;
        private System.Windows.Forms.CheckBox showCollideRect;
    }
}