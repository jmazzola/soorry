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
    public partial class DrawTool : Form
    {
        public DrawTool()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form1 form = (Form1)this.Owner;
            form.UpDownSpriteX = UpDownDrawX.Value;
            form.UpDownSpriteY = UpDownDrawY.Value;
            form.UpDownSpriteW=  UpDownDrawWidth.Value;
            form.UpDownSpriteH = UpDownDrawHeight.Value;

            this.Close();
        }

    }
}
