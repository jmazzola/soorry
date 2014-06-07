#include "MachineGunTower.h"

#include "Camera.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "CreateMachineGunBulletMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


MachineGunTower::MachineGunTower()
{
	m_nDamage = 25;
	m_fFireRate = 0.5f;
	m_fBulletSpeed = 750.0f;

	m_fNextShotTimer = 0.0f;
}


MachineGunTower::~MachineGunTower()
{
}


/**********************************************************/
// Interface Methods

void MachineGunTower::Update(float dt)
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

			CreateMachineGunBulletMessage* msg = new CreateMachineGunBulletMessage((int)(m_ptPosition.x + 16 + offset.x), (int)(m_ptPosition.y + 16 + offset.y), bulletVelocity, m_nDamage);
			msg->QueueMessage();

			// Reset timer
			m_fNextShotTimer = m_fFireRate;
		}
	}
}

void MachineGunTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void MachineGunTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));

	Tower::PostRender();
}

int MachineGunTower::GetType() const
{
	return ENT_TOWER_MACHINE_GUN;
}

/**********************************************************/
// Accessors

int MachineGunTower::GetDamage() const
{
	return m_nDamage;
}

float MachineGunTower::GetFireRate() const
{
	return m_fFireRate;
}

/**********************************************************/
// Mutators

void MachineGunTower::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void MachineGunTower::SetFireRate(float _fireRate)
{
	m_fFireRate = _fireRate;
}