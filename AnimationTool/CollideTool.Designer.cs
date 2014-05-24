namespace AnimationTool
{
    partial class CollideTool
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
            this.cHeight = new System.Windows.Forms.Label();
            this.cWidth = new System.Windows.Forms.Label();
            this.cY = new System.Windows.Forms.Label();
            this.cX = new System.Windows.Forms.Label();
            this.UpDownCollideHeight = new System.Windows.Forms.NumericUpDown();
            this.UpDownCollideWidth = new System.Windows.Forms.NumericUpDown();
            this.UpDownCollideY = new System.Windows.Forms.NumericUpDown();
            this.UpDownCollideX = new System.Windows.Forms.NumericUpDown();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideX)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cHeight
            // 
            this.cHeight.AutoSize = true;
            this.cHeight.Location = new System.Drawing.Point(11, 125);
            this.cHeight.Name = "cHeight";
            this.cHeight.Size = new System.Drawing.Size(49, 17);
            this.cHeight.TabIndex = 15;
            this.cHeight.Text = "Height";
            // 
            // cWidth
            // 
            this.cWidth.AutoSize = true;
            this.cWidth.Location = new System.Drawing.Point(11, 97);
            this.cWidth.Name = "cWidth";
            this.cWidth.Size = new System.Drawing.Size(44, 17);
            this.cWidth.TabIndex = 14;
            this.cWidth.Text = "Width";
            // 
            // cY
            // 
            this.cY.AutoSize = true;
            this.cY.Location = new System.Drawing.Point(23, 64);
            this.cY.Name = "cY";
            this.cY.Size = new System.Drawing.Size(17, 17);
            this.cY.TabIndex = 13;
            this.cY.Text = "Y";
            // 
            // cX
            // 
            this.cX.AutoSize = true;
            this.cX.Location = new System.Drawing.Point(23, 30);
            this.cX.Name = "cX";
            this.cX.Size = new System.Drawing.Size(17, 17);
            this.cX.TabIndex = 12;
            this.cX.Text = "X";
            // 
            // UpDownCollideHeight
            // 
            this.UpDownCollideHeight.Location = new System.Drawing.Point(79, 120);
            this.UpDownCollideHeight.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownCollideHeight.Name = "UpDownCollideHeight";
            this.UpDownCollideHeight.Size = new System.Drawing.Size(63, 22);
            this.UpDownCollideHeight.TabIndex = 11;
            // 
            // UpDownCollideWidth
            // 
            this.UpDownCollideWidth.Location = new System.Drawing.Point(79, 92);
            this.UpDownCollideWidth.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownCollideWidth.Name = "UpDownCollideWidth";
            this.UpDownCollideWidth.Size = new System.Drawing.Size(63, 22);
            this.UpDownCollideWidth.TabIndex = 10;
            // 
            // UpDownCollideY
            // 
            this.UpDownCollideY.Location = new System.Drawing.Point(79, 59);
            this.UpDownCollideY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownCollideY.Name = "UpDownCollideY";
            this.UpDownCollideY.Size = new System.Drawing.Size(63, 22);
            this.UpDownCollideY.TabIndex = 9;
            // 
            // UpDownCollideX
            // 
            this.UpDownCollideX.Location = new System.Drawing.Point(79, 28);
            this.UpDownCollideX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownCollideX.Name = "UpDownCollideX";
            this.UpDownCollideX.Size = new System.Drawing.Size(63, 22);
            this.UpDownCollideX.TabIndex = 8;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(14, 172);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(128, 24);
            this.button1.TabIndex = 16;
            this.button1.Text = "Add";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.UpDownCollideY);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.UpDownCollideX);
            this.groupBox1.Controls.Add(this.cHeight);
            this.groupBox1.Controls.Add(this.UpDownCollideWidth);
            this.groupBox1.Controls.Add(this.cWidth);
            this.groupBox1.Controls.Add(this.UpDownCollideHeight);
            this.groupBox1.Controls.Add(this.cY);
            this.groupBox1.Controls.Add(this.cX);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(180, 202);
            this.groupBox1.TabIndex = 17;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Collision Rectangle";
            // 
            // CollideTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(361, 298);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.Name = "CollideTool";
            this.Text = "CollideTool";
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownCollideX)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label cHeight;
        private System.Windows.Forms.Label cWidth;
        private System.Windows.Forms.Label cY;
        private System.Windows.Forms.Label cX;
        private System.Windows.Forms.NumericUpDown UpDownCollideHeight;
        private System.Windows.Forms.NumericUpDown UpDownCollideWidth;
        private System.Windows.Forms.NumericUpDown UpDownCollideY;
        private System.Windows.Forms.NumericUpDown UpDownCollideX;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}