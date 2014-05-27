#include "ShotgunPellet.h"


ShotgunPellet::ShotgunPellet()
{
	SetSprite(AnimationManager::GetInstance()->GetSprite("bullet"));
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "bullet";
}


ShotgunPellet::~ShotgunPellet()
{
}

int ShotgunPellet::GetType() const
{
	return ENT_BULLET_SHOTGUN;
}
