#include "FastZombie.h"

#include "AnimationManager.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

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


void FastZombie::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x, m_ptPosition.y);

	Entity::Render();
}


/**********************************************************/
// Interface Methods

int FastZombie::GetType() const
{
	return ENT_ZOMBIE_FAST;
}