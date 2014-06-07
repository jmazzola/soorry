#include "HockeyStickTower.h"

#include "Camera.h"
#include "EntityManager.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


HockeyStickTower::HockeyStickTower()
{
	m_fRotationRate = 12.0f;
	m_fRadius = 48.0f;
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

	SGD::Vector heading = SGD::Vector(-1, 0);
	heading.Rotate(m_fRotation);

	// Attack enemies
	const vector<IEntity*> enemies = m_pEntityManager->GetBucket(2);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		// Make sure it's not on the hit list
		if (CheckHitList(enemies[i]))
			continue;

		// Compute vector
		SGD::Vector distanceVector = enemies[i]->GetRect().ComputeCenter() - (m_ptPosition + SGD::Vector(16, 16));
		float distance = distanceVector.ComputeLength();

		// Check if within radius
		if (distance < m_fRadius)
		{
			// Check if we hit it
			distanceVector.Normalize();

			if (heading.ComputeAngle(distanceVector) < 0.2 || heading.ComputeAngle(distanceVector) > -0.2)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(enemies[i]);

				// Add to the hit list
				m_vEnemiesHit.push_back(EnemyHit(enemy));

				enemy->SetCurrHealth(enemy->GetCurrHealth() - 5);
			}
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

/**********************************************************/
// Helper functions

bool HockeyStickTower::CheckHitList(IEntity* _enemy) const
{
	for (unsigned int enemy = 0; enemy < m_vEnemiesHit.size(); enemy++)
	{
		if (m_vEnemiesHit[enemy].enemy == _enemy)
			return true;
	}

	return false;
}