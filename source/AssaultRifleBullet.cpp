#include "AssaultRifleBullet.h"


AssaultRifleBullet::AssaultRifleBullet()
{
	SetSprite(AnimationManager::GetInstance()->GetSprite("bullet"));
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "bullet";
}


AssaultRifleBullet::~AssaultRifleBullet()
{
}

int AssaultRifleBullet::GetType() const
{
	return ENT_BULLET_ASSAULT;
}
