#include "AnimationManager.h"
#include "Sprite.h"
#include "Frame.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_Geometry.h"



// LoadSprites
//	- reading all the sprites from the XML file

bool AnimationManager::LoadSprites(std::string fileName)
{
	TiXmlDocument animationDoc;

	// Check to see if there is anything to load from the file
	if (animationDoc.LoadFile(fileName.c_str()) == false);
		return false;

		TiXmlElement* root = animationDoc.RootElement();

		// Check to see if there is actually anything to read in the XML
		if (root == nullptr)
			return false;
		// Getting rid of any past data
		m_vSprites.clear();

		TiXmlElement* spriteImg = root->FirstChildElement("sprite");
		while (spriteImg != nullptr)
		{
			std::string file = spriteImg->GetText();
			if (file.c_str() != nullptr)
				m_vSprites[0].SetImage(file.c_str());
		}
		spriteImg = spriteImg->NextSiblingElement("spriteID");
		while (spriteImg != nullptr)
		{
			std::string id = spriteImg->GetText();
			if (id.c_str() != nullptr)
				m_vSprites[0].SetSpriteID(id);
		}
		spriteImg = spriteImg->NextSiblingElement("isLooping");
		while (spriteImg != nullptr)
		{
			int boolNum;
			spriteImg->Attribute("isIt", &boolNum);
			m_vSprites[0].SetLooping(boolNum);
		}
		TiXmlElement* frames = spriteImg->FirstChildElement("frame");
		while (frames != nullptr)
		{
			SGD::Rectangle collTemp;
			int l, t, r, b;
			frames->Attribute("left", &l);
			collTemp.left = l;
			frames->Attribute("right", &r);
			collTemp.right = r;
			frames->Attribute("top", &t);
			collTemp.top = t;
			frames->Attribute("bottom", &b);
			collTemp.bottom = b;

			m_vSprites[0].GetFrame(0).SetCollisionRect(collTemp);
		}

		frames = frames->NextSiblingElement("drawRect");
		while (frames != nullptr)
		{
			SGD::Rectangle drawTemp;
			int l, t, r, b;
			frames->Attribute("left", &l);
			drawTemp.left = l;
			frames->Attribute("right", &r);
			drawTemp.right = r;
			frames->Attribute("top", &t);
			drawTemp.top = t;
			frames->Attribute("bottom", &b);
			drawTemp.bottom = b;

			m_vSprites[0].GetFrame(0).SetFrameRect(drawTemp);
		}

		frames = frames->NextSiblingElement("duration");
		while (frames != nullptr)
		{
			int durtemp;
			frames->Attribute("time", &durtemp);
			m_vSprites[0].GetFrame(0).SetDuration(durtemp);

		}

		frames = frames->NextSiblingElement("anchorPoint");
		while (frames != nullptr)
		{
			SGD::Point pointTemp;
			int x, y;
			frames->Attribute("X", &x);
			pointTemp.x = x;
			frames->Attribute("Y", &y);
			pointTemp.y = y;
			m_vSprites[0].GetFrame(0).SetAnchorPoint(pointTemp);
		}

		frames = frames->NextSiblingElement("triggerID");
		while (frames != nullptr)
		{
			std::string id = spriteImg->GetText();
			if (id.c_str() != nullptr)
				m_vSprites[0].GetFrame(0).SetTriggerID(id);
		}
		return (m_vSprites.size() > 0);
}

void AnimationManager::Update(AnimationTimestamp& ants, float dt)
{

}

void AnimationManager::Render(AnimationTimestamp& ants, int x, int y)
{

}


//Sprite AnimationManager::GetSprite(std::string id)
//{
//	for (int i = 0; i < m_vSprites.size(); i++)
//	{
//		return m_vSprites[i];
//		
//	}
//}