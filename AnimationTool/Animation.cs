using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace AnimationTool
{
    class Frame
    {
        decimal frameTop;

        public decimal FrameTop
        {
            get { return frameTop; }
            set { frameTop = value; }
        }

        decimal frameBottom;
        
        public decimal FrameBottom
        {
            get { return frameBottom; }
            set { frameBottom = value; }
        }
        
        decimal frameLeft;

        public decimal FrameLeft
        {
            get { return frameLeft; }
            set { frameLeft = value; }
        }

        decimal frameRight;

        public decimal FrameRight
        {
            get { return frameRight; }
            set { frameRight = value; }
        }

        decimal hitTop;

        public decimal HitTop
        {
            get { return hitTop; }
            set { hitTop = value; }
        }

        decimal hitBottom;

        public decimal HitBottom
        {
            get { return hitBottom; }
            set { hitBottom = value; }
        }

        decimal hitLeft;

        public decimal HitLeft
        {
            get { return hitLeft; }
            set { hitLeft = value; }
        }

        decimal hitRight;

        public decimal HitRight
        {
            get { return hitRight; }
            set { hitRight = value; }
        }

       public Frame()
       {

       }
       decimal anchorX;

       public decimal AnchorX
       {
           get { return anchorX; }
           set { anchorX = value; }
       }

       decimal anchorY;

       public decimal AnchorY
       {
           get { return anchorY; }
           set { anchorY = value; }
       }

       decimal duration;

       public decimal Duration
       {
           get { return duration; }
           set { duration = value; }
       }


       string triggerID;

       public string TriggerID
       {
           get { return triggerID; }
           set { triggerID = value; }
       }
    }

    class Sprite : Object
    {
        private string imagePath;

        public string ImagePath
        {
            get { return imagePath; }
            set { imagePath = value; }
        }

        private string spriteID;

        public string SpriteID
        {
            get { return spriteID; }
            set { spriteID = value; }
        }

        private int loop;

        public int Loop
        {
            get { return loop; }
            set { loop = value; }
        }

        private List<Frame> frames;

        public List<Frame> Frames
        {
            get { return frames; }
            set { frames = value; }
        }

        public Sprite()
        {
            frames = new List<Frame>();
        }
        public override string ToString()
        {
            return spriteID + ' ';

        }
    }

    class Animation : Object
    {
        private List<Sprite> sprites;
        public List<Sprite> Sprites
        {
            get { return sprites; }
            set { sprites = value; }
        }

        public Animation()
        {
            sprites = new List<Sprite>();
        }
      
    }
 
}
