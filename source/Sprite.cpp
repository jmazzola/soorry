#include "Sprite.h"
#include "Frame.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	if (m_htImage != SGD::INVALID_HANDLE)
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_htImage);

	for (size_t i = 0; i < m_vFrames.size(); i++)
	{
		delete m_vFrames[i];
		m_vFrames[i] = nullptr;
	}
}

Sprite::Sprite(const Sprite& cpy)
{
	m_bIsLooping = cpy.m_bIsLooping;
	m_htImage = cpy.m_htImage;
	m_strSpriteID = cpy.m_strSpriteID;
	for (unsigned int i = 0; i < m_vFrames.size(); i++)
		m_vFrames[i] = cpy.m_vFrames[i];
}

Sprite& Sprite::operator = (const Sprite& assign)
{
	m_bIsLooping = assign.m_bIsLooping;
	m_htImage = assign.m_htImage;
	m_strSpriteID = assign.m_strSpriteID;
	for (unsigned int i = 0; i < m_vFrames.size(); i++)
		m_vFrames[i] = assign.m_vFrames[i];
	return *this;
}



void Sprite::SetImage(const char * fileName)
{
	m_htImage = SGD::GraphicsManager::GetInstance()->LoadTexture(fileName);
}

void Sprite::NextFrame( void )
{
	m_nCurrFrame++;
	if(m_nCurrFrame == m_vFrames.size())
		m_nCurrFrame = 0;

}
