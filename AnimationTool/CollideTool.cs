using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AnimationTool
{
    public partial class CollideTool : Form
    {
        public CollideTool()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form1 form = (Form1)this.Owner;
            form.UpDownCollX = UpDownCollideX.Value;
            form.UpDownCollY = UpDownCollideY.Value;
            form.UpDownCollW = UpDownCollideWidth.Value;
            form.UpDownCollH = UpDownCollideHeight.Value;

            this.Close();
        }
    }
}
