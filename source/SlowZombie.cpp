#include "SlowZombie.h"


SlowZombie::SlowZombie()
{
	m_pSprite = AnimationManager::GetInstance()->GetSprite("fastZombie");
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "fastZombie";
}


SlowZombie::~SlowZombie()
{
}


/**********************************************************/
// Interface Methods

int SlowZombie::GetType() const
{
	return ENT_ZOMBIE_SLOW;
}