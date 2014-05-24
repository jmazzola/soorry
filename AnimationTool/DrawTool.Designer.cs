namespace AnimationTool
{
    partial class DrawTool
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
            this.UpDownDrawX = new System.Windows.Forms.NumericUpDown();
            this.UpDownDrawY = new System.Windows.Forms.NumericUpDown();
            this.UpDownDrawWidth = new System.Windows.Forms.NumericUpDown();
            this.UpDownDrawHeight = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.numericUpDown2 = new System.Windows.Forms.NumericUpDown();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.anchorY = new System.Windows.Forms.Label();
            this.anchorX = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawHeight)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // UpDownDrawX
            // 
            this.UpDownDrawX.Location = new System.Drawing.Point(84, 38);
            this.UpDownDrawX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownDrawX.Name = "UpDownDrawX";
            this.UpDownDrawX.Size = new System.Drawing.Size(63, 22);
            this.UpDownDrawX.TabIndex = 0;
            // 
            // UpDownDrawY
            // 
            this.UpDownDrawY.Location = new System.Drawing.Point(84, 69);
            this.UpDownDrawY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownDrawY.Name = "UpDownDrawY";
            this.UpDownDrawY.Size = new System.Drawing.Size(63, 22);
            this.UpDownDrawY.TabIndex = 1;
            // 
            // UpDownDrawWidth
            // 
            this.UpDownDrawWidth.Location = new System.Drawing.Point(84, 102);
            this.UpDownDrawWidth.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownDrawWidth.Name = "UpDownDrawWidth";
            this.UpDownDrawWidth.Size = new System.Drawing.Size(63, 22);
            this.UpDownDrawWidth.TabIndex = 2;
            // 
            // UpDownDrawHeight
            // 
            this.UpDownDrawHeight.Location = new System.Drawing.Point(84, 130);
            this.UpDownDrawHeight.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.UpDownDrawHeight.Name = "UpDownDrawHeight";
            this.UpDownDrawHeight.Size = new System.Drawing.Size(63, 22);
            this.UpDownDrawHeight.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(28, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 17);
            this.label1.TabIndex = 4;
            this.label1.Text = "X";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(28, 74);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 17);
            this.label2.TabIndex = 5;
            this.label2.Text = "Y";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(16, 107);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 17);
            this.label3.TabIndex = 6;
            this.label3.Text = "Width";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(16, 135);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 17);
            this.label4.TabIndex = 7;
            this.label4.Text = "Height";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.UpDownDrawWidth);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.UpDownDrawX);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.UpDownDrawY);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.UpDownDrawHeight);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(185, 220);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Drawing Rectangle";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.numericUpDown2);
            this.groupBox2.Controls.Add(this.numericUpDown1);
            this.groupBox2.Controls.Add(this.anchorY);
            this.groupBox2.Controls.Add(this.anchorX);
            this.groupBox2.Location = new System.Drawing.Point(204, 24);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(137, 208);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Anchor Point";
            // 
            // numericUpDown2
            // 
            this.numericUpDown2.Location = new System.Drawing.Point(45, 62);
            this.numericUpDown2.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.numericUpDown2.Name = "numericUpDown2";
            this.numericUpDown2.Size = new System.Drawing.Size(56, 22);
            this.numericUpDown2.TabIndex = 3;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(45, 28);
            this.numericUpDown1.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(56, 22);
            this.numericUpDown1.TabIndex = 2;
            // 
            // anchorY
            // 
            this.anchorY.AutoSize = true;
            this.anchorY.Location = new System.Drawing.Point(6, 62);
            this.anchorY.Name = "anchorY";
            this.anchorY.Size = new System.Drawing.Size(17, 17);
            this.anchorY.TabIndex = 1;
            this.anchorY.Text = "Y";
            // 
            // anchorX
            // 
            this.anchorX.AutoSize = true;
            this.anchorX.Location = new System.Drawing.Point(7, 28);
            this.anchorX.Name = "anchorX";
            this.anchorX.Size = new System.Drawing.Size(17, 17);
            this.anchorX.TabIndex = 0;
            this.anchorX.Text = "X";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(113, 238);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(128, 24);
            this.button1.TabIndex = 17;
            this.button1.Text = "Add";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // DrawTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(362, 300);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.Name = "DrawTool";
            this.Text = "DrawTool";
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.UpDownDrawHeight)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NumericUpDown UpDownDrawX;
        private System.Windows.Forms.NumericUpDown UpDownDrawY;
        private System.Windows.Forms.NumericUpDown UpDownDrawWidth;
        private System.Windows.Forms.NumericUpDown UpDownDrawHeight;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label anchorY;
        private System.Windows.Forms.Label anchorX;
        private System.Windows.Forms.NumericUpDown numericUpDown2;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.Button button1;
    }
}