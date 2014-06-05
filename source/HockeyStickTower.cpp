#include "HockeyStickTower.h"

#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


HockeyStickTower::HockeyStickTower()
{
	m_fRotationRate = 12.0f;
}


HockeyStickTower::~HockeyStickTower()
{
}


/**********************************************************/
// Interface Methods

void HockeyStickTower::Update(float dt)
{
	// Spin the stick
	m_fRotation += m_fRotationRate * dt;

	// Update hit list
	for (unsigned int i = 0; i < m_vEnemiesHit.size(); i++)
	{
		m_vEnemiesHit[i].angleRemaining -= m_fRotationRate * dt;
		if (m_vEnemiesHit[i].angleRemaining <= 0.0f)
		{
			m_vEnemiesHit.erase(m_vEnemiesHit.begin() + i);
			i--;
		}
	}


}

void HockeyStickTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void HockeyStickTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));
}

int HockeyStickTower::GetType() const
{
	return ENT_TOWER_HOCKEY_STICK;
}