#include "MapleSyrupTower.h"

#include "Camera.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "CreateMapleSyrupBulletMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


MapleSyrupTower::MapleSyrupTower()
{
	m_fSlowTime = 3.0f;
	m_fFireRate = 1.2f;
	m_fBulletSpeed = 250.0f;

	m_fNextShotTimer = 0.0f;
}


MapleSyrupTower::~MapleSyrupTower()
{
}


/**********************************************************/
// Interface Methods

void MapleSyrupTower::Update(float dt)
{
	// Update timers
	m_fNextShotTimer -= dt;

	// Aim at enemy
	Enemy* enemy = dynamic_cast<Enemy*>(m_pEntityManager->CheckCollision(SGD::Point(m_ptPosition.x + 16.0f, m_ptPosition.y + 16.0f), 128.0f, 2));
	SGD::Vector toEnemy;
	if (enemy)
	{
		SGD::Vector orientation = { 0, -1 };
		toEnemy = enemy->GetPosition() - m_ptPosition;
		float distance = toEnemy.ComputeLength();
		toEnemy.Normalize();

		// Create bullet's velocity
		SGD::Vector bulletVelocity;

		// Get estimated bullet travel time
		float bulletTime = distance / m_fBulletSpeed;

		// Get estimated enemy position
		SGD::Point estimatedPos = enemy->GetPosition() + enemy->GetVelocity() * bulletTime;

		bulletVelocity = estimatedPos - m_ptPosition;
		bulletVelocity.Normalize();

		if (orientation.ComputeSteering(bulletVelocity) > 0)
			m_fRotation = orientation.ComputeAngle(bulletVelocity);
		else
			m_fRotation = -orientation.ComputeAngle(bulletVelocity);

		// Shoot, if time
		if (m_fNextShotTimer <= 0.0f)
		{
			bulletVelocity *= m_fBulletSpeed;
			SGD::Vector offset = bulletVelocity.ComputeNormalized() * 16;

			CreateMapleSyrupBulletMessage* msg = new CreateMapleSyrupBulletMessage((int)(m_ptPosition.x + 8 + offset.x), (int)(m_ptPosition.y + 8 + offset.y), bulletVelocity, m_fSlowTime);
			msg->QueueMessage();

			// Reset timer
			m_fNextShotTimer = m_fFireRate;
		}
	}
}

void MapleSyrupTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void MapleSyrupTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));

	Tower::PostRender();
}

int MapleSyrupTower::GetType() const
{
	return ENT_TOWER_MAPLE_SYRUP;
}

/**********************************************************/
// Accessors

float MapleSyrupTower::GetSlowTime() const
{
	return m_fSlowTime;
}

float MapleSyrupTower::GetFireRate() const
{
	return m_fFireRate;
}

/**********************************************************/
// Mutators

void MapleSyrupTower::SetSlowTime(float _slowTime)
{
	m_fSlowTime = _slowTime;
}

void MapleSyrupTower::SetFireRate(float _fireRate)
{
	m_fFireRate = _fireRate;
}