#include "AnimationManager.h"
#include "Sprite.h"
#include "Frame.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

/*static*/ AnimationManager* AnimationManager::s_pInstance = nullptr;

/*static*/ AnimationManager* AnimationManager::GetInstance()
{
	if (s_pInstance == nullptr)
		s_pInstance = new AnimationManager;
	return s_pInstance;
}

/*static*/ void AnimationManager::DeleteInstance()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

// LoadSprites
//	- reading all the sprites from the XML file
std::string AnimationManager::LoadSprites(std::string fileName)
{
	TiXmlDocument animationDoc;

	// Check to see if there is anything to load from the file
	if (animationDoc.LoadFile(fileName.c_str()) == false)
		return false;

	Sprite* newSprite = new Sprite();
	TiXmlElement* root = animationDoc.RootElement();
	int numSprites;
	root->Attribute("Sprites", &numSprites);
	// Check to see if there is actually anything to read in the XML
	if (root == nullptr)
		return false;
	// Getting rid of any past data
	if (made == false)
	{
		m_mSprites.clear();
		made = true;
	}

	std::string ID;
	for (int i = 0; i < numSprites; i++)
	{
		TiXmlElement* spriteImg = root->FirstChildElement("sprite");
		while (spriteImg != nullptr)
		{
			if (spriteImg != nullptr)
			{
				std::string file = "resource/animation/";
				file += spriteImg->GetText();

				if (file.c_str() != nullptr)
					newSprite->SetImage(file.c_str());
			}
			spriteImg = spriteImg->NextSiblingElement("spriteID");
			if (spriteImg != nullptr)
			{
				std::string id = spriteImg->GetText();
				if (id.c_str() != nullptr)
				{
					newSprite->SetSpriteID(id);
					ID = id;
				}
			}
			spriteImg = spriteImg->NextSiblingElement("isLooping");
			if (spriteImg != nullptr)
			{
				int boolNum;
				spriteImg->Attribute("isIt", &boolNum);
				if (boolNum == 0)
					newSprite->SetLooping(false);
				else
					newSprite->SetLooping(true);

			}
			spriteImg = spriteImg->NextSiblingElement("Frames");
			int numFrames;
			if (spriteImg != nullptr)
			{
				spriteImg->Attribute("numbers", &numFrames);
			}
			TiXmlElement* frames = spriteImg->NextSiblingElement("frame");
			for (int i = 0; i < numFrames; i++)
			{
				if (frames != nullptr)
				{
					TiXmlElement* info = frames->FirstChildElement("anchorPoint");
					Frame* newFrame = new Frame();
					if (info != nullptr)
					{
						SGD::Point pointTemp;
						double x, y;
						info->Attribute("X", &x);
						pointTemp.x = (float)x;
						info->Attribute("Y", &y);
						pointTemp.y = (float)y;
						newFrame->SetAnchorPoint(pointTemp);
					}
					info = info->NextSiblingElement("collisionRect");

					if (info != nullptr)
					{
						SGD::Rectangle collTemp;
						double l, t, r, b;
						info->Attribute("left", &l);
						collTemp.left = (float)l;
						info->Attribute("right", &r);
						collTemp.right = (float)r;
						info->Attribute("top", &t);
						collTemp.top = (float)t;
						info->Attribute("bottom", &b);
						collTemp.bottom = (float)b;

						newFrame->SetCollisionRect(collTemp);
					}

					info = info->NextSiblingElement("drawRect");
					if (info != nullptr)
					{
						SGD::Rectangle drawTemp;
						double l, t, r, b;
						info->Attribute("left", &l);
						drawTemp.left = (float)l;
						info->Attribute("right", &r);
						drawTemp.right = (float)r;
						info->Attribute("top", &t);
						drawTemp.top = (float)t;
						info->Attribute("bottom", &b);
						drawTemp.bottom = (float)b;

						newFrame->SetFrameRect(drawTemp);
					}

					info = info->NextSiblingElement("duration");
					if (info != nullptr)
					{
						double durtemp;
						info->Attribute("time", &durtemp);
						newFrame->SetDuration((float)durtemp);

					}

					info = info->NextSiblingElement("triggerID");
					if (info != nullptr)
					{
						std::string trigid = info->GetText();
						if (trigid.c_str() != nullptr)
							newFrame->SetTriggerID(trigid);
					}
					newSprite->AddFrame(newFrame);
					//frames->
					frames = frames->NextSiblingElement("frame");
				}
			}
			m_mSprites[newSprite->GetSpriteID()] = newSprite;
			m_vSpriteNames.push_back(newSprite->GetSpriteID());
			spriteImg = spriteImg->NextSiblingElement("sprite");
		}
	}
	if (m_mSprites.size() > 0)
		return ID;
	else
		return nullptr;
}

void AnimationManager::UnloadSprites()
{
	for (unsigned int i = 0; i < m_vSpriteNames.size(); i++)
	{
		delete m_mSprites[m_vSpriteNames[i]];
	}
}

void AnimationManager::Update(AnimationTimestamp& ants, float dt)
{
	ants.m_fTimeOnFrame += dt;
	// check to see if the frame duration is over
	if (ants.m_fTimeOnFrame > m_mSprites[ants.m_nCurrAnimation]->GetFrame(ants.m_nCurrFrame).GetDuration())
	{
		ants.m_fTimeOnFrame -= m_mSprites[ants.m_nCurrAnimation]->GetFrame(ants.m_nCurrFrame).GetDuration();

		// change to next frame
		++ants.m_nCurrFrame;

		// check to see if its the last frame
		if (ants.m_nCurrFrame == m_mSprites[ants.m_nCurrAnimation]->GetFrameSize())
		{
			// if they are looping they will go back to the first frame
			if (m_mSprites[ants.m_nCurrAnimation]->IsLooping())
				ants.m_nCurrFrame = 0;
			else
			{
				// if not stop at the last frame
				--ants.m_nCurrFrame;
			}
		}
	}
}

void AnimationManager::Render(AnimationTimestamp& ants, float x, float y, float rotation, SGD::Vector center, SGD::Color col)
{
	SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
	//g->DrawLine({ (float)x, (float)y }, { (float)x + 2, (float)y + 2 });

	x -= m_mSprites[ants.m_nCurrAnimation]->GetFrame(ants.m_nCurrFrame).GetAnchorPoint().x;
	y -= m_mSprites[ants.m_nCurrAnimation]->GetFrame(ants.m_nCurrFrame).GetAnchorPoint().y;

	SGD::Rectangle r = m_mSprites[ants.m_nCurrAnimation]->GetFrame(ants.m_nCurrFrame).GetFrameRect();

	// Getting the specific frame
	//r.right += r.left;
	//r.bottom += r.top;


	g->DrawTextureSection(m_mSprites[ants.m_nCurrAnimation]->GetImage(),
	{ (float)x, (float)y },
	r, rotation, center, col);

	SGD::Rectangle dr = r;
	dr.top += y;
	dr.bottom += y;
	dr.left += x;
	dr.right += x;

	//g->DrawRectangle(dr, { 128, 255, 255, 0 });

}

Sprite* AnimationManager::GetSprite(std::string nameID)
{
	if (m_mSprites[nameID] != nullptr)
		return m_mSprites[nameID];
	else
		return nullptr;
}

void AnimationManager::LoadAll()
{
	LoadSprites("resource/animation/player.xml");
	LoadSprites("resource/animation/beaver.xml");
	LoadSprites("resource/animation/fastZombie.xml");
	LoadSprites("resource/animation/slowZombie.xml");
	LoadSprites("resource/animation/rocket.xml");
	LoadSprites("resource/animation/Crab.xml");
	LoadSprites("resource/animation/Wall.xml");
	LoadSprites("resource/animation/Window.xml");
	LoadSprites("resource/animation/bullet.xml");
	LoadSprites("resource/animation/mine.xml");
	LoadSprites("resource/animation/superpickup.xml");
	LoadSprites("resource/animation/ammopickup.xml");
	LoadSprites("resource/animation/healthpickup.xml");
	LoadSprites("resource/animation/drone.xml");
	LoadSprites("resource/animation/grenade.xml");
	LoadSprites("resource/animation/trickshot.xml");
	LoadSprites("resource/animation/RocketCarry.xml");
	LoadSprites("resource/animation/AssultCarry.xml");
	LoadSprites("resource/animation/HatTrickCarry.xml");
}
