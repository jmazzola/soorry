#include "AssaultRifleBullet.h"


AssaultRifleBullet::AssaultRifleBullet()
{
	SetSprite(AnimationManager::GetInstance()->GetSprite("rocket"));
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "rocket";
}


AssaultRifleBullet::~AssaultRifleBullet()
{
}
