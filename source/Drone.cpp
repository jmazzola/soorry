#include "Drone.h"
#include "Player.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "CreateMachineGunBulletMessage.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "DestroyEntityMessage.h"
#include <math.h>
#include "GameplayState.h"

Drone::Drone()
{
	m_nDamage = 10;
	m_fNextShotTimer = 1;
	m_fFireRate = 1;
	m_fRange = 75.0f;
	SetSprite(AnimationManager::GetInstance()->GetSprite("drone"));
	SetCurrFrame(0);
	SetCurrAnimation("drone");
	m_fBulletSpeed = 1000;
	m_fAngle = 0;
}

Drone::~Drone()
{
	GameplayState::GetInstance()->droneCount--;
}

void Drone::Update(float dt)
{
	//update timers
	m_fHitTimer -= dt;
	m_fNextShotTimer -= dt;

	//if its dead kill it
	if (m_nHealth <= 0)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

	//radius from the center(player pos)
	float radius = 36;
	// center x and y
	float x = m_pPlayer->GetPosition().x + 8;
	float y = m_pPlayer->GetPosition().y + 8;
	//center point
	SGD::Vector center = SGD::Vector(x, y);

	//for (unsigned int i = 0; i < drones.size(); i++)
	//{
	//	//run check to see if there is a drone behind
	//	for (unsigned int j = 0; j < drones.size(); j++)
	//	{
	//		if (drones[j]->m_nNumberID == drones[j]->m_nNumberID - 1 && drones[j] != nullptr)
	//		{
	//			drones[i]->m_fAngle = drones[j]->m_fAngle - 72;
	//		}
	//		if (drones[j]->m_nNumberID == drones[j]->m_nNumberID + 1 && drones[j] != nullptr)
	//		{
	//			drones[i]->m_fAngle = drones[j]->m_fAngle + 72;
	//		}
	//	}
	//}
	//Angle that will incriment to spin it around the player
	//m_fAngle += 2 * dt;
	m_fAngle = (m_pPlayer->m_fDroneRotation + (6.28f / GameplayState::GetInstance()->droneCount)*m_nNumberID) ;
	//If the angle is over 360 reset to 0
	/*if (m_fAngle > 360)
		m_fAngle -= 360;*/
	//set position to the center plus the angle spot 
	SGD::Vector newVec = center.Lerp({ m_ptPosition.x, m_ptPosition.y }, { center.x + (radius*cosf(m_fAngle)), center.y + (radius*sinf(m_fAngle)) }, 0.15f);
	m_ptPosition.x = newVec.x;
	m_ptPosition.y = newVec.y;
	//Find enemy and shoot at it
	Enemy* enemy = dynamic_cast<Enemy*>(m_pEntityManager->CheckCollision(SGD::Point(m_ptPosition.x + 16.0f, m_ptPosition.y + 16.0f), m_fRange, 2));
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
void			Drone::SetAngle(float _Angle)
{
	m_fAngle = _Angle;
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