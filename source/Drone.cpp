#include "Drone.h"
#include "Player.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "CreateMachineGunBulletMessage.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "DestroyEntityMessage.h"

Drone::Drone()
{
	//NOTE: hardcoded should change for upgrades
	m_nDamage = 20;
	m_fNextShotTimer = 1;
	m_fFireRate = 1;
	m_fRange = 100.0f;
	SetSprite(AnimationManager::GetInstance()->GetSprite("drone"));
	SetCurrFrame(0);
	SetCurrAnimation("drone");
	m_fBulletSpeed = 1000;
	
}

Drone::~Drone()
{
	
}

void Drone::Update(float dt)
{
	if (m_nHealth <= 0)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

	m_fHitTimer -= dt;
	
	SGD::Point tempPoint = { m_pPlayer->GetPosition().x - 32, m_pPlayer->GetPosition().y + 32 };
	
	SetPosition(tempPoint);

	m_fNextShotTimer -= dt;

	Enemy* enemy = dynamic_cast<Enemy*>(m_pEntityManager->CheckCollision(SGD::Point(m_ptPosition.x + 16.0f, m_ptPosition.y + 16.0f), m_fRange, 1));
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

void Drone::Render()
{
	int frame = GetSprite()->GetCurrFrame();

	SGD::Vector center;
	center.x = GetSprite()->GetFrame(frame).GetFrameRect().right - GetSprite()->GetFrame(frame).GetFrameRect().left;
	center.y = GetSprite()->GetFrame(frame).GetFrameRect().bottom - GetSprite()->GetFrame(frame).GetFrameRect().top;
	center.x /= 2;
	center.y /= 2;

	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y, m_fRotation, center);

}

int  Drone::GetType() const
{
	return ENT_DRONE;
}

void Drone::HandleCollision(const IEntity* pOther)
{
	if ((pOther->GetType() == ENT_ZOMBIE_BEAVER || pOther->GetType() == ENT_ZOMBIE_FAST || pOther->GetType() == ENT_ZOMBIE_SLOW) && m_fHitTimer <= 0)
	{
		m_nHealth -= 5;
		m_fHitTimer = 1.0f;
	}
}

void			Drone::SetDamage(int _Damage)
{
	m_nDamage = _Damage;
}
void			Drone::SetNumberID(int _ID)
{
	m_nNumberID = _ID;
}
void			Drone::SetHealth(int _Health)
{
	m_nHealth = _Health;
}
void			Drone::SetNextShotTimer(float _Timer)
{
	m_fNextShotTimer = _Timer;
}
void			Drone::SetFireRate(float _FireRate)
{
	m_fFireRate = _FireRate;
}
void			Drone::SetBulletSpeed(float _BulletSpeed)
{
	m_fBulletSpeed = _BulletSpeed;
}
void			Drone::SetRotation(float _Rotation)
{
	m_fRotation = _Rotation;
}
void			Drone::SetRange(float _Range)
{
	m_fRange = _Range;
}
void			Drone::SetPlayer(Player* _player)
{
	m_pPlayer = _player;
}
void			Drone::SetEntityManager(EntityManager* _EntityManager)
{
	m_pEntityManager = _EntityManager;
}
void			Drone::SetDroneImage(SGD::HTexture _DroneImage)
{
	m_hDroneImage = _DroneImage;
}
int				Drone::GetDamage() const
{
	return m_nDamage;
}
int				Drone::GetNumberID() const
{
	return m_nNumberID;
}
float			Drone::GetNextShotTimer() const
{
	return m_fNextShotTimer;
}
float			Drone::GetFireRate() const
{
	return m_fFireRate;
}
float			Drone::GetBulletSpeed() const
{
	return m_fBulletSpeed;
}
float			Drone::GetRotation() const
{
	return m_fRotation;
}
float			Drone::GetRange()const
{ 
	return m_fRange;
}
Player*			Drone::GetPlayer()const
{
	return m_pPlayer;
}
EntityManager*	Drone::GetEntityManager()const
{
	return m_pEntityManager;
}
SGD::HTexture	Drone::GetDroneImage()const
{
	return m_hDroneImage;
}