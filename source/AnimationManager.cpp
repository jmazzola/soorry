#include "AnimationManager.h"
#include "Sprite.h"
#include "Frame.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


// LoadSprites
//	- reading all the sprites from the XML file
bool AnimationManager::LoadSprites(std::string fileName)
{
	TiXmlDocument animationDoc;

	// Check to see if there is anything to load from the file
	if (animationDoc.LoadFile(fileName.c_str()) == false)
		return false;

	Sprite newSprite;
		TiXmlElement* root = animationDoc.RootElement();
		
		// Check to see if there is actually anything to read in the XML
		if (root == nullptr)
			return false;
		// Getting rid of any past data
		m_mSprites.clear();

		TiXmlElement* spriteImg = root->FirstChildElement("sprite");
		while (spriteImg != nullptr)
		{
			if (spriteImg != nullptr)
			{
				//std::string file = spriteImg->GetText();
				const char * file = spriteImg->GetText();
				/*if (file.c_str() != nullptr)
					newSprite.SetImage(file.c_str());*/
				if (file != nullptr)
					newSprite.SetImage(file);
				
			}
			spriteImg = spriteImg->NextSiblingElement("spriteID");
			if (spriteImg != nullptr)
			{
				std::string id = spriteImg->GetText();
				if (id.c_str() != nullptr)
					newSprite.SetSpriteID(id);
			}
			spriteImg = spriteImg->NextSiblingElement("isLooping");
			if (spriteImg != nullptr)
			{
				int boolNum;
				spriteImg->Attribute("isIt", &boolNum);
				if (boolNum == 0)
					newSprite.SetLooping(false);
				else
					newSprite.SetLooping(true);

			}
			TiXmlElement* frames = spriteImg->NextSiblingElement("frame");
			while (frames != nullptr)
			{
				frames = frames->FirstChildElement("collisionRect");
				Frame newFrame;
				if (frames != nullptr)
				{
					SGD::Rectangle collTemp;
					double l, t, r, b;
					frames->Attribute("left", &l);
					collTemp.left = (float)l;
					frames->Attribute("right", &r);
					collTemp.right = (float)r;
					frames->Attribute("top", &t);
					collTemp.top = (float)t;
					frames->Attribute("bottom", &b);
					collTemp.bottom = (float)b;

					newFrame.SetCollisionRect(collTemp);
				}

				frames = frames->NextSiblingElement("drawRect");
				if (frames != nullptr)
				{
					SGD::Rectangle drawTemp;
					double l, t, r, b;
					frames->Attribute("left", &l);
					drawTemp.left = (float)l;
					frames->Attribute("right", &r);
					drawTemp.right = (float)r;
					frames->Attribute("top", &t);
					drawTemp.top = (float)t;
					frames->Attribute("bottom", &b);
					drawTemp.bottom = (float)b;

					newFrame.SetFrameRect(drawTemp);
				}

				frames = frames->NextSiblingElement("duration");
				if (frames != nullptr)
				{
					double durtemp;
					frames->Attribute("time", &durtemp);
					newFrame.SetDuration((float)durtemp);

				}

				frames = frames->NextSiblingElement("anchorPoint");
				if (frames != nullptr)
				{
					SGD::Point pointTemp;
					double x, y;
					frames->Attribute("X", &x);
					pointTemp.x = (float)x;
					frames->Attribute("Y", &y);
					pointTemp.y = (float)y;
					newFrame.SetAnchorPoint(pointTemp);
				}

				frames = frames->NextSiblingElement("triggerID");
				if (frames != nullptr)
				{
					std::string trigid = frames->GetText();
					if (trigid.c_str() != nullptr)
						newFrame.SetTriggerID(trigid);
				}
				newSprite.SetFrame(newFrame);
				frames = frames->FirstChildElement("frames");
			}
			m_mSprites[newSprite.GetSpriteID()] = newSprite;
			spriteImg = spriteImg->NextSiblingElement("sprite");
		}
		return (m_mSprites.size() > 0);
}

void AnimationManager::Update(AnimationTimestamp& ants, float dt)
{
}

void AnimationManager::Render(AnimationTimestamp& ants, float x, float y)
{
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_mSprites[ants.m_nCurrAnimation].GetImage(),
	{ (float)x, (float)y },
	m_mSprites[ants.m_nCurrAnimation].GetFrame(ants.m_nCurrFrame).GetFrameRect());

}


