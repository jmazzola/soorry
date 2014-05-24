using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SGP;
using System.Xml.Linq;

namespace AnimationTool
{
    public partial class Form1 : Form
    {
        CSGP_Direct3D DX = CSGP_Direct3D.GetInstance();
        CSGP_TextureManager TM = CSGP_TextureManager.GetInstance();

        FolderBrowserDialog folder = new FolderBrowserDialog();
        bool loadImageOnce = false;
        DrawTool draw = null;
        CollideTool collide = null;
        PlayBack playback = null;

        Animation animations = new Animation();

        Rectangle selectedFrame;
        Rectangle selectedFrameC;
        Rectangle selectedFrameD;
        Rectangle workSelectedFrameC;
        Rectangle workSelectedFrameD;

        Point selectedFrameAnchor;
        string currentPicture;

        int loadID = -1;
        bool animation = false;

        public bool Animation
        {
            get { return animation; }
            set { animation = value; }
        }
        float toolTimer;
        float animationTimer = 0;
        int currFrame;

        public int CurrFrame
        {
            get { return currFrame; }
            set { currFrame = value; }
        }


        bool animationLoop = false;

        bool showDrectangle;

        public bool ShowDrectangle
        {
            get { return showDrectangle; }
            set { showDrectangle = value; }
        }
        bool showCrectangle;

        public bool ShowCrectangle
        {
            get { return showCrectangle; }
            set { showCrectangle = value; }
        }


        private bool looping = true;
        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public decimal UpDownCollX
        {
            get
            { return upDownCollX.Value; }
            set
            { upDownCollX.Value = value; }
        }
        public decimal UpDownCollY
        {
            get
            { return upDownCollY.Value; }
            set
            { upDownCollY.Value = value; }
        }
        public decimal UpDownCollW
        {
            get
            { return upDownCollW.Value; }
            set
            { upDownCollW.Value = value; }
        }
        public decimal UpDownCollH
        {
            get
            { return upDownCollH.Value; }
            set
            { upDownCollH.Value = value; }
        }
        public decimal UpDownSpriteX
        {
            get
            { return upDownSpriteX.Value; }
            set
            { upDownSpriteX.Value = value; }
        }
        public decimal UpDownSpriteY
        {
            get
            { return upDownSpriteY.Value; }
            set
            { upDownSpriteY.Value = value; }
        }
        public decimal UpDownSpriteW
        {
            get
            { return upDownSpriteW.Value; }
            set
            { upDownSpriteW.Value = value; }
        }
        public decimal UpDownSpriteH
        {
            get
            { return upDownSpriteH.Value; }
            set
            { upDownSpriteH.Value = value; }
        }
        public Form1()
        {
            InitializeComponent();

            DX.Initialize(splitContainer1.Panel2, true);
            DX.AddRenderTarget(tabPage2);

            TM.Initialize(DX.Device, DX.Sprite);
            currFrame = 0;
            upDownLocX.Value = 100;
            upDownLocY.Value = 100;
            toolTimer = Environment.TickCount;
            
        }

        public new void Update()
        {
            float now = Environment.TickCount;
            float elapsedTime = (now - toolTimer) / 1000;
            toolTimer = now;

	        // Cap the elapsed time to 1/8th of a second
            if (elapsedTime > 0.125f)
                elapsedTime = 0.125f;


            if (animation == true)
            {
                animationTimer += elapsedTime;
                if (Sprites.SelectedIndex != -1 && 
                    (decimal)animationTimer >= animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].Duration)
                {
                    animationTimer = 0;
                    Frame f = animations.Sprites[Sprites.SelectedIndex].Frames[currFrame];
                    selectedFrame.X = (int)f.FrameLeft;
                    selectedFrame.Y = (int)f.FrameTop;
                    selectedFrame.Width = (int)f.FrameRight;
                    selectedFrame.Height = (int)f.FrameBottom;
                    selectedFrameAnchor.X = (int)f.AnchorX;
                    selectedFrameAnchor.Y = (int)f.AnchorY;

                    ++currFrame;
                    if (Frames.SelectedIndex + 1 < Frames.Items.Count)
                        ++Frames.SelectedIndex;

                    if (currFrame == animations.Sprites[Sprites.SelectedIndex].Frames.Count)
                    {
                        Frames.SelectedIndex = 0;
                        if (animationLoop == true)
                            currFrame = 0;
                        else
                        {
                            --currFrame;
                            animation = false;
                        }

                    }
                }

            }
        }
        public void Render()
        {
            // Will render the sprite to the Sprite tab on the left
            DX.Clear(tabPage2, Color.White);
            DX.DeviceBegin();
            DX.SpriteBegin();
            if (loadID != -1)
            {
                Point offset = new Point(0, 0);
                offset.X = tabPage2.AutoScrollPosition.X;
                offset.Y = tabPage2.AutoScrollPosition.Y;


                TM.Draw(loadID, offset.X, offset.Y);

                // If they want to see the Hollow Rect it will display
                if (showDrectangle == true)
                    DX.DrawHollowRect(workSelectedFrameD, Color.Blue, 2);

                if (showCrectangle == true)
                    DX.DrawHollowRect(workSelectedFrameC, Color.Red, 2);

            }
            DX.SpriteEnd();
            DX.DeviceEnd();
            DX.Present();

            // Check to see if it wants to play the animation
            if (animation == false)
            {
                // If the user does not want to do animation it just render the 
                // selected index in the Frames listbox
                if (Sprites.SelectedIndex != -1
               && animations.Sprites[Sprites.SelectedIndex].Frames.Count != 0)
                {
                    DX.Clear(splitContainer1.Panel2, Color.White);
                    DX.DeviceBegin();
                    DX.SpriteBegin();
                    if (loadID != -1)
                    {

                        if (selectedFrame != null)
                            TM.Draw(loadID, (int)upDownLocX.Value - selectedFrameAnchor.X,
                                (int)upDownLocY.Value - selectedFrameAnchor.Y, 1.0f, 1.0f, selectedFrame);


                        DX.DrawLine((int)upDownLocX.Value, (int)upDownLocY.Value,
                            (int)upDownLocX.Value + 1, (int)upDownLocY.Value + 1, Color.Black, 1);

                    }
                    DX.SpriteEnd();
                    DX.DeviceEnd();
                    DX.Present();


                }
            }
            else if (animation == true)
            {
                // If the user does want to do animation it render all 
                // selected indexs in the Frames listbox
                DX.Clear(splitContainer1.Panel2, Color.White);
                DX.DeviceBegin();
                DX.SpriteBegin();
                if (loadID != -1)
                {
                    Point newPoint = new Point();
                    newPoint.X = (int)upDownLocX.Value - (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].AnchorX;
                    newPoint.Y = (int)upDownLocY.Value - (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].AnchorY;

                    if (selectedFrame != null)
                        TM.Draw(loadID, (int)upDownLocX.Value - (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].AnchorX,
                            (int)upDownLocY.Value - (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].AnchorY, 1.0f, 1.0f, selectedFrame);

                    DX.DrawLine((int)upDownLocX.Value, (int)upDownLocY.Value,
                        (int)upDownLocX.Value + 1, (int)upDownLocY.Value + 1, Color.Black, 5);


                    // Offsetting the Draw and Collission Hollow Rectangle
                    selectedFrameC.X = newPoint.X;
                    selectedFrameC.Y = newPoint.Y;

                    selectedFrameC.Width = (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].HitRight;
                    selectedFrameC.Height = (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].HitBottom;

                    selectedFrameD.X = newPoint.X;
                    selectedFrameD.Y = newPoint.Y;
                    selectedFrameD.Width = (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].HitRight;
                    selectedFrameD.Height = (int)animations.Sprites[Sprites.SelectedIndex].Frames[currFrame].HitBottom;

                    // If they want to see the Hollow Rect it will display
                    if (showDrectangle == true)
                        DX.DrawHollowRect(selectedFrameD, Color.Blue, 2);

                    if (showCrectangle == true)
                        DX.DrawHollowRect(selectedFrameC, Color.Red, 2);

                }
                DX.SpriteEnd();
                DX.DeviceEnd();
                DX.Present();
            }



        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
        }

        private void drawRectangleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (draw == null)
            {
                draw = new DrawTool();
                draw.FormClosed += draw_FormClosed;
                draw.Show(this);
            }
        }

        void draw_FormClosed(object sender, FormClosedEventArgs e)
        {
            draw = null;
        }

        private void collisionRectangleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (collide == null)
            {
                collide = new CollideTool();
                collide.FormClosed += collide_FormClosed;
                collide.Show(this);
            }
        }
        void collide_FormClosed(object sender, FormClosedEventArgs e)
        {
            collide = null;
        }

        private void animationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (playback == null)
            {
                playback = new PlayBack();
                playback.FormClosed += playback_FormClosed;
                playback.Show(this);
            }
        }

        void playback_FormClosed(object sender, FormClosedEventArgs e)
        {
            playback = null;
        }

        private void spriteSheetToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "All Files(*.*)|*.*|My Files(*.png)|*.png";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                loadID = TM.LoadTexture(dlg.FileName);
                currentPicture = dlg.SafeFileName;
                tabPage2.AutoScrollMinSize = new Size(TM.GetTextureWidth(loadID), TM.GetTextureHeight(loadID));
               
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int track = 0;
            Frames.Items.Clear();
            Sprites.Items.Clear();
            animations.Sprites.Clear();
            DX.Clear(splitContainer1.Panel2, Color.White);
            DX.Present();

            // Loading XML files to use for the tool
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement xRoot = XElement.Load(dlg.FileName);
                // Loading in the image from the file
                int xNumSprites = (int)xRoot.Attribute("Sprites");

                folder.ShowDialog();
                if (folder.SelectedPath == "")
                    return;

                string xSprite = folder.SelectedPath;
                xSprite += '\\';
                currentPicture = (string)xRoot.Element("sprite");
                xSprite += currentPicture;
                loadID = TM.LoadTexture(xSprite);
                tabPage2.AutoScrollMinSize = new Size(TM.GetTextureWidth(loadID), TM.GetTextureHeight(loadID));

                for (int i = 0; i < xNumSprites; i++)
                {
                    Sprite cycle = new Sprite();
                    
                    // Loading the SpriteID
                    string xSpriteID = (string)xRoot.Element("spriteID");
                    cycle.SpriteID = xSpriteID;
                    textSpriteID.Text = xSpriteID;

                    animations.Sprites.Add(cycle);
                    Sprites.Items.Add(animations.Sprites.Count + cycle.SpriteID);


                    // Checking to see if its looping
                    XElement xLooping = xRoot.Element("isLooping");
                    XAttribute XLoopBool = xLooping.Attribute("isIt");
                    int nLoopbool = Convert.ToInt32(XLoopBool.Value);

                    if (nLoopbool == 0)
                    {
                        radioButtonFalse.Checked = true;
                        radioButtonTrue.Checked = false;
                    }
                    else
                    {
                        radioButtonFalse.Checked = false;
                        radioButtonTrue.Checked = true;
                    }

                    // Checking How many frames for this sprite
                    XElement xFrames = xRoot.Element("Frames");
                    int xnumFrames = (int)xFrames.Attribute("numbers");
                    for (int j = 0; j < xnumFrames; j++)
                    {
                        XElement xFrame = xRoot.Element("frame");
                        // Create a frame for the sprite
                        Frame f = new Frame();

                        // Loading frame Anchor Point
                        XElement xAnchor = xFrame.Element("anchorPoint");
                        XAttribute xAnchorX = xAnchor.Attribute("X");
                        int nAnchorX = Convert.ToInt32(xAnchorX.Value);
                        f.AnchorX = nAnchorX;
                        XAttribute xAnchorY = xAnchor.Attribute("Y");
                        int nAnchorY = Convert.ToInt32(xAnchorY.Value);
                        f.AnchorY = nAnchorY;
                        xAnchor = null;

                        //Loading in the Collsion Rectangle
                        XElement xCollide = xFrame.Element("collisionRect");
                        XAttribute xColLeft = xCollide.Attribute("left");
                        int nCollLeft = Convert.ToInt32(xColLeft.Value);
                        f.HitLeft = nCollLeft + nAnchorX;
                        XAttribute xColRight = xCollide.Attribute("right");
                        int nCollRight = Convert.ToInt32(xColRight.Value);
                        f.HitRight = nCollRight + nAnchorX;
                        XAttribute xColTop = xCollide.Attribute("top");
                        int nCollTop = Convert.ToInt32(xColTop.Value);
                        f.HitTop = nCollTop + nAnchorY;
                        XAttribute xColBottom = xCollide.Attribute("bottom");
                        int nCollBottom = Convert.ToInt32(xColBottom.Value);
                        f.HitBottom = nCollBottom + nAnchorY;

                        //Loading in the Draw Rectangle
                        XElement xDraw = xFrame.Element("drawRect");
                        XAttribute xDrawLeft = xDraw.Attribute("left");
                        int nDrawLeft = Convert.ToInt32(xDrawLeft.Value);
                        f.FrameLeft = nDrawLeft;
                        XAttribute xDrawRight = xDraw.Attribute("right");
                        int nDrawRight = Convert.ToInt32(xDrawRight.Value);
                        f.FrameRight = nDrawRight;
                        XAttribute xDrawTop = xDraw.Attribute("top");
                        int nDrawTop = Convert.ToInt32(xDrawTop.Value);
                        f.FrameTop = nDrawTop;
                        XAttribute xDrawBottom = xDraw.Attribute("bottom");
                        int nDrawBottom = Convert.ToInt32(xDrawBottom.Value);
                        f.FrameBottom = nDrawBottom;

                        // Loading frame Duration
                        XElement xDur = xFrame.Element("duration");
                        XAttribute xTime = xDur.Attribute("time");
                        //double nTime = Convert.ToDouble(xTime.Value);
                        decimal nTime = Convert.ToDecimal(xTime.Value);
                        f.Duration = (decimal)nTime;

                        // Loading the SpriteID
                        string xTriggerID = (string)xFrame.Element("triggerID");
                        f.TriggerID = xTriggerID;
                        textTriggerID.Text = xTriggerID;

                        animations.Sprites[track].Frames.Add(f);
                        Frames.Items.Add(animations.Sprites[track].Frames.Count);
                        //xFrame = null;
                        f = null;
                        xFrame.Remove();
                    }
                    ++track;
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                if (animations.Sprites != null)
                {
                    XElement xRoot = new XElement("animation");
                    XAttribute xAnimations = new XAttribute("Sprites", animations.Sprites.Count);
                    xRoot.Add(xAnimations);

                    if (currentPicture != null)
                    {
                        XElement xLink = new XElement("sprite", currentPicture);
                        xRoot.Add(xLink);

                    }

                    for (int i = 0; i < animations.Sprites.Count; i++)
                    {

                        if (animations.Sprites[i].SpriteID != null && animations.Sprites[i].SpriteID != "")
                        {
                            XElement xSpriteID = new XElement("spriteID", animations.Sprites[i].SpriteID);
                            xRoot.Add(xSpriteID);
                        }

                        XElement xLooping = new XElement("isLooping");
                        xRoot.Add(xLooping);

                        if (animationLoop == true)
                        {
                            XAttribute xisIt = new XAttribute("isIt", 1);
                            xLooping.Add(xisIt);
                        }
                        else
                        {
                            XAttribute xisIt = new XAttribute("isIt", 0);
                            xLooping.Add(xisIt);
                        }

                        XElement xNumFrames = new XElement("Frames");
                        xRoot.Add(xNumFrames);
                        XAttribute xNumbers = new XAttribute("numbers", animations.Sprites[i].Frames.Count);
                        xNumFrames.Add(xNumbers);

                        for (int j = 0; j < animations.Sprites[i].Frames.Count; j++)
                        {
                            XElement xFrame = new XElement("frame");
                            xRoot.Add(xFrame);

                            XElement xAnchor = new XElement("anchorPoint");
                            xFrame.Add(xAnchor);

                            XAttribute xAnchorX = new XAttribute("X", animations.Sprites[i].Frames[j].AnchorX);
                            xAnchor.Add(xAnchorX);

                            XAttribute xAnchorY = new XAttribute("Y", animations.Sprites[i].Frames[j].AnchorY);
                            xAnchor.Add(xAnchorY);


                            XElement xCollide = new XElement("collisionRect");
                            xFrame.Add(xCollide);

                            XAttribute xCLeft = new XAttribute("left", animations.Sprites[i].Frames[j].HitLeft - animations.Sprites[i].Frames[j].AnchorX);
                            xCollide.Add(xCLeft);

                            XAttribute xCRight = new XAttribute("right", animations.Sprites[i].Frames[j].HitRight - animations.Sprites[i].Frames[j].AnchorX);
                            xCollide.Add(xCRight);

                            XAttribute xCTop = new XAttribute("top", animations.Sprites[i].Frames[j].HitTop - animations.Sprites[i].Frames[j].AnchorY);
                            xCollide.Add(xCTop);

                            XAttribute xCBottom = new XAttribute("bottom", animations.Sprites[i].Frames[j].HitBottom - animations.Sprites[i].Frames[j].AnchorY);
                            xCollide.Add(xCBottom);


                            XElement xDraw = new XElement("drawRect");
                            xFrame.Add(xDraw);


                            XAttribute xDLeft = new XAttribute("left", animations.Sprites[i].Frames[j].FrameLeft);
                            xDraw.Add(xDLeft);

                            XAttribute xDRight = new XAttribute("right", animations.Sprites[i].Frames[j].FrameRight);
                            xDraw.Add(xDRight);

                            XAttribute xDTop = new XAttribute("top", animations.Sprites[i].Frames[j].FrameTop);
                            xDraw.Add(xDTop);

                            XAttribute xDBottom = new XAttribute("bottom", animations.Sprites[i].Frames[j].FrameBottom);
                            xDraw.Add(xDBottom);

                            XElement xDuration = new XElement("duration");
                            xFrame.Add(xDuration);

                            XAttribute xTime = new XAttribute("time", animations.Sprites[i].Frames[j].Duration);
                            xDuration.Add(xTime);

                            //XElement xAnchor = new XElement("anchorPoint");
                            //xFrame.Add(xAnchor);

                            //XAttribute xAnchorX = new XAttribute("X", animations.Sprites[i].Frames[j].AnchorX);
                            //xAnchor.Add(xAnchorX);

                            //XAttribute xAnchorY = new XAttribute("Y", animations.Sprites[i].Frames[j].AnchorY);
                            //xAnchor.Add(xAnchorY);

                            if (animations.Sprites[i].Frames[j].TriggerID != null && animations.Sprites[i].Frames[j].TriggerID != "")
                            {
                                XElement xTrigger = new XElement("triggerID", animations.Sprites[i].Frames[j].TriggerID);
                                xFrame.Add(xTrigger);
                            }
                        }
                    }
                    xRoot.Save(dlg.FileName);
                }
            }
        }

        private void Frames_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Frames.SelectedIndex >= 0)
            {
                if (Sprites.SelectedIndex != -1)
                {
                    Frame f = animations.Sprites[Sprites.SelectedIndex].Frames[Frames.SelectedIndex];

                    selectedFrameAnchor.X = (int)f.AnchorX;
                    selectedFrameAnchor.Y = (int)f.AnchorY;


                    selectedFrame.X = (int)f.FrameLeft;
                    selectedFrame.Y = (int)f.FrameTop;
                    selectedFrame.Width = (int)f.FrameRight;
                    selectedFrame.Height = (int)f.FrameBottom;



                    selectedFrameC.X = (int)f.HitLeft;
                    selectedFrameC.Y = (int)f.HitTop;
                    selectedFrameC.Width = (int)f.HitRight;
                    selectedFrameC.Height = (int)f.HitBottom;

                    selectedFrameD.X = (int)f.FrameLeft;
                    selectedFrameD.Y = (int)f.FrameTop;
                    selectedFrameD.Width = (int)f.FrameRight;
                    selectedFrameD.Height = (int)f.FrameBottom;


                    workSelectedFrameC.X = (int)f.HitLeft;
                    workSelectedFrameC.Y = (int)f.HitTop;
                    workSelectedFrameC.Width = (int)f.HitRight;
                    workSelectedFrameC.Height = (int)f.HitBottom;

                    workSelectedFrameD.X = (int)f.FrameLeft;
                    workSelectedFrameD.Y = (int)f.FrameTop;
                    workSelectedFrameD.Width = (int)f.FrameRight;
                    workSelectedFrameD.Height = (int)f.FrameBottom;


                    upDownSpriteX.Value = (int)f.FrameLeft;
                    upDownSpriteY.Value = (int)f.FrameTop;
                    upDownSpriteW.Value = (int)f.FrameRight;
                    upDownSpriteH.Value = (int)f.FrameBottom;

                    upDownCollX.Value = (int)f.HitLeft;
                    upDownCollY.Value = (int)f.HitTop;
                    upDownCollW.Value = (int)f.HitRight;
                    upDownCollH.Value = (int)f.HitBottom;

                    upDownAnchorX.Value = f.AnchorX;
                    upDownAnchorY.Value = f.AnchorY;

                    upDownDuration.Value = f.Duration;
                }
                else
                {
                    Frame f = animations.Sprites[0].Frames[Frames.SelectedIndex];

                    selectedFrameAnchor.X = (int)f.AnchorX;
                    selectedFrameAnchor.Y = (int)f.AnchorY;


                    selectedFrame.X = (int)f.FrameLeft;
                    selectedFrame.Y = (int)f.FrameTop;
                    selectedFrame.Width = (int)f.FrameRight;
                    selectedFrame.Height = (int)f.FrameBottom;



                    selectedFrameC.X = (int)f.HitLeft;
                    selectedFrameC.Y = (int)f.HitTop;
                    selectedFrameC.Width = (int)f.HitRight;
                    selectedFrameC.Height = (int)f.HitBottom;

                    selectedFrameD.X = (int)f.FrameLeft;
                    selectedFrameD.Y = (int)f.FrameTop;
                    selectedFrameD.Width = (int)f.FrameRight;
                    selectedFrameD.Height = (int)f.FrameBottom;

                    upDownSpriteX.Value = (int)f.FrameLeft;
                    upDownSpriteY.Value = (int)f.FrameTop;
                    upDownSpriteW.Value = (int)f.FrameRight;
                    upDownSpriteH.Value = (int)f.FrameBottom;

                    upDownCollX.Value = (int)f.HitLeft;
                    upDownCollY.Value = (int)f.HitTop;
                    upDownCollW.Value = (int)f.HitRight;
                    upDownCollH.Value = (int)f.HitBottom;

                    upDownAnchorX.Value = f.AnchorX;
                    upDownAnchorY.Value = f.AnchorY;

                    upDownDuration.Value = f.Duration;
                }
            }
        }

        private void radioButtonTrue_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonTrue.Checked == true)
            {
                animationLoop = true;
            }
        }

        private void radioButtonFalse_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonFalse.Checked == true)
            {
                animationLoop = false;
            }
        }

        private void removeFrameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Frames.SelectedIndex >= 0 && Sprites.SelectedIndex != 0)
            {
                animations.Sprites[Sprites.SelectedIndex].Frames.RemoveAt(Frames.SelectedIndex);
                Frames.Items.RemoveAt(Frames.SelectedIndex);

                selectedFrame.X = 0;
                selectedFrame.Y = 0;
                selectedFrame.Width = 0;
                selectedFrame.Height = 0;

                selectedFrameC.X = 0;
                selectedFrameC.Y = 0;
                selectedFrameC.Width = 0;
                selectedFrameC.Height = 0;

                selectedFrameD.X = 0;
                selectedFrameD.Y = 0;
                selectedFrameD.Width = 0;
                selectedFrameD.Height = 0;

                selectedFrameAnchor.X = 0;
                selectedFrameAnchor.Y = 0;

                upDownSpriteX.Value = 0;
                upDownSpriteY.Value = 0;
                upDownSpriteW.Value = 0;
                upDownSpriteH.Value = 0;

                upDownCollX.Value = 0;
                upDownCollY.Value = 0;
                upDownCollW.Value = 0;
                upDownCollH.Value = 0;

                upDownAnchorX.Value = 0;
                upDownAnchorY.Value = 0;

                upDownDuration.Value = 0;

            }
        }

        private void updateFrameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Frames.SelectedIndex >= 0 && animations.Sprites.Count != 0 && Sprites.SelectedIndex != -1)
            {
                Frame f = new Frame();

                f.FrameLeft = upDownSpriteX.Value;
                f.FrameTop = upDownSpriteY.Value;
                f.FrameRight = upDownSpriteW.Value;
                f.FrameBottom = upDownSpriteH.Value;


                f.HitLeft = upDownCollX.Value;
                f.HitTop = upDownCollY.Value;
                f.HitRight = upDownCollW.Value;
                f.HitBottom = upDownCollH.Value;

                f.AnchorX = upDownAnchorX.Value;
                f.AnchorY = upDownAnchorY.Value;

                f.Duration = upDownDuration.Value;

                f.TriggerID = textTriggerID.Text;

                animations.Sprites[Sprites.SelectedIndex].Frames[Frames.SelectedIndex] = f;
                Frames.Items[Frames.SelectedIndex] = f;
            }
        }

        private void upDownAnchorX_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownAnchorY_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownSpriteX_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownSpriteY_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownSpriteW_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownSpriteH_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownCollX_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownCollY_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownCollW_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void upDownCollH_ValueChanged(object sender, EventArgs e)
        {
            updateFrameToolStripMenuItem_Click(sender, e);
        }

        private void deselectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Frames.SelectedIndex >= 0)
            {
                Frames.SelectedIndex = -1;
            }
            if (Sprites.SelectedIndex >= 0)
            {
                Sprites.SelectedIndex = -1;
            }
        }

        private void tabPage1_MouseDown(object sender, MouseEventArgs e)
        {
            Point offset = new Point(0, 0);
            offset.X = tabPage2.AutoScrollPosition.X;
            offset.Y = tabPage2.AutoScrollPosition.Y;

            if (e.X >= 0 && e.Y >= 0)
            {
                upDownCollX.Value = e.X;
                upDownCollY.Value = e.Y;
            }
        }

        private void tabPage1_MouseUp(object sender, MouseEventArgs e)
        {
            Point offset = new Point(0, 0);
            offset.X = tabPage2.AutoScrollPosition.X;
            offset.Y = tabPage2.AutoScrollPosition.Y;

            if (e.X >= 0 && e.Y >= 0)
            {
                if (e.X - upDownCollX.Value >= 0 && e.Y - upDownCollY.Value >= 0)
                {
                    upDownCollW.Value = e.X - upDownCollX.Value;
                    upDownCollH.Value = e.Y - upDownCollY.Value;

                }
            }

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            if (Frames.SelectedIndex - 1 >= 0)
                Frames.SelectedIndex -= 1;
            Frames_SelectedIndexChanged(sender, e);

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            if (Sprites.SelectedIndex != -1 && Frames.SelectedIndex + 1 < animations.Sprites[Sprites.SelectedIndex].Frames.Count)
                Frames.SelectedIndex += 1;
            Frames_SelectedIndexChanged(sender, e);

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Frame f = new Frame();

            f.FrameLeft = upDownSpriteX.Value;
            f.FrameTop = upDownSpriteY.Value;
            f.FrameRight = upDownSpriteW.Value;
            f.FrameBottom = upDownSpriteH.Value;


            f.AnchorX = upDownAnchorX.Value;
            f.AnchorY = upDownAnchorY.Value;


            f.HitLeft = upDownCollX.Value;
            f.HitTop = upDownCollY.Value;
            f.HitRight = upDownCollW.Value;
            f.HitBottom = upDownCollH.Value;

            f.Duration = upDownDuration.Value;

            f.TriggerID = textTriggerID.Text;

            if (animations.Sprites.Count == 0)
            {
                button4_Click(sender, e);
                animations.Sprites[0].Frames.Add(f);
                Frames.Items.Add(animations.Sprites[0].Frames.Count);

            }
            else
            {
                if (Sprites.SelectedIndex != -1)
                {
                    animations.Sprites[Sprites.SelectedIndex].Frames.Add(f);
                    Frames.Items.Add(animations.Sprites[Sprites.SelectedIndex].Frames.Count);
                }
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Sprite s = new Sprite();
            s.SpriteID = textSpriteID.Text;

            animations.Sprites.Add(s);

            Sprites.Items.Add(animations.Sprites.Count + s.SpriteID);
        }
        private void Sprites_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Sprites.SelectedIndex >= 0)
            {
                Frames.Items.Clear();

                for (int i = 0; i < animations.Sprites[Sprites.SelectedIndex].Frames.Count; i++)
                {
                    Frames.Items.Add(animations.Sprites[Sprites.SelectedIndex].Frames[i]);
                }
            }

        }
    }
}
