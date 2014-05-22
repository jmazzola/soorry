#include "BeaverZombie.h"

#include "AnimationManager.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

BeaverZombie::BeaverZombie()
{
	m_pSprite = AnimationManager::GetInstance()->GetSprite("beaver");
}


BeaverZombie::~BeaverZombie()
{
}


void BeaverZombie::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x, m_ptPosition.y);

	Entity::Render();
}


/**********************************************************/
// Interface Methods

int BeaverZombie::GetType() const
{
	return ENT_ZOMBIE_BEAVER;
}