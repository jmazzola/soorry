#include "FastZombie.h"

#include "AnimationManager.h"


FastZombie::FastZombie()
{
	m_pSprite = AnimationManager::GetInstance()->GetSprite("fastZombie");
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "fastZombie";
}


FastZombie::~FastZombie()
{
}


/**********************************************************/
// Interface Methods

int FastZombie::GetType() const
{
	return ENT_ZOMBIE_FAST;
}