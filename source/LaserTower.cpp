#include "LaserTower.h"

#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


LaserTower::LaserTower() : Listener(this)
{
}


LaserTower::~LaserTower()
{
}


/**********************************************************/
// Interface Methods

void LaserTower::Update(float dt)
{

}

void LaserTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void LaserTower::PostRender()
{

}

int LaserTower::GetType() const
{
	return ENT_TOWER_LASER;
}