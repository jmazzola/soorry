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
    public partial class PlayBack : Form
    {
        public PlayBack()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Starting/ Resuming the Animation
            Form1 form = (Form1)this.Owner;
            form.Animation = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Pausing the Animation
            Form1 form = (Form1)this.Owner;
            form.Animation = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // Restarting the Animation
            Form1 form = (Form1)this.Owner;
            form.Animation = false;
            form.CurrFrame = 0;

        }

        private void showDrawRect_CheckedChanged(object sender, EventArgs e)
        {
            Form1 form = (Form1)this.Owner;

            if (showDrawRect.Checked == true)
                form.ShowDrectangle = true;
            else
                form.ShowDrectangle = false;
        }

        private void showCollideRect_CheckedChanged(object sender, EventArgs e)
        {
            Form1 form = (Form1)this.Owner;

            if (showCollideRect.Checked == true)
                form.ShowCrectangle = true;
            else
                form.ShowCrectangle = false;

        }

     
    }
}
