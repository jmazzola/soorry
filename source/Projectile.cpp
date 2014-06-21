#include "Projectile.h"
#include "GameplayState.h"
#include "Enemy.h"
#include "Entity.h"
#include "EntityManager.h"
#include "DestroyEntityMessage.h"
#include "WorldManager.h"
#include "CreateExplosionMessage.h"
#include "CreateParticleMessage.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Projectile::Projectile()
{
	m_fLifeTime = 0;
	m_fCanAccel = 0.0f;
}


Projectile::~Projectile()
{

}


/**********************************************************/
// Interface Methods

void Projectile::Update(float dt)
{
	Entity::Update(dt);

	m_fLifeTime += dt;
	m_fCanAccel += dt;

	if (this->GetType() == ENT_BULLET_ROCKET && m_fCanAccel >= 0.05f)
	{
		// Update speed
		SGD::Vector toAdd = m_vtVelocity;
		toAdd.Normalize();
		toAdd *= 100.0f;
		m_vtVelocity += toAdd;

		m_fCanAccel = 0.0f;

		CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle2", { m_ptPosition.x, m_ptPosition.y }, 16, 16);
		msg->QueueMessage();
		msg = nullptr;
	}

	if (WorldManager::GetInstance()->CheckCollision(GetRect(), true) || m_fLifeTime > 10)
	{
		if (this->GetType() == ENT_BULLET_ROCKET)
		{
			CreateExplosionMessage* msg = new CreateExplosionMessage(m_ptPosition.x + 8, m_ptPosition.y + 8, (float)m_nDamage, m_fRadius);
			msg->QueueMessage();
		}
		
		// Destroy the proj
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;

		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hHit))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hHit);
	}
}

int Projectile::GetType() const
{
	return ENT_PROJECTILE;
}

/*virtual*/ void Projectile::HandleCollision(const IEntity* pOther) /*override*/
{
	if (pOther->GetType() == ENT_ZOMBIE_BEAVER || pOther->GetType() == ENT_ZOMBIE_FAST ||
		pOther->GetType() == ENT_ZOMBIE_SLOW)
	{
		if (this->GetType() == ENT_BULLET_ROCKET)
		{
			CreateExplosionMessage* msg = new CreateExplosionMessage(m_ptPosition.x + 8, m_ptPosition.y + 8, (float)m_nDamage, m_fRadius);
			msg->QueueMessage();
		}

		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hImpact) == false)
			SGD::AudioManager::GetInstance()->PlayAudio(m_hImpact);

		// Destroy the proj
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
		SGD::AudioManager::GetInstance()->PlayAudio(m_hHit);
	}
}

/**********************************************************/
// Accessors

int Projectile::GetDamage() const
{
	return m_nDamage;
}

float Projectile::GetRadius() const
{
	return m_fRadius;
}

float Projectile::GetSpeed() const
{
	return m_fSpeed;
}

float Projectile::GetLifeTime() const
{
	return m_fLifeTime;
}

/**********************************************************/
// Mutators

void Projectile::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void Projectile::SetRadius(float _radius)
{
	m_fRadius = _radius;
}

void Projectile::SetSpeed(float _speed)
{
	m_fSpeed = _speed;
}

void Projectile::SetLifeTime(float _lifeTime)
{
	m_fLifeTime = _lifeTime;
}

void Projectile::SetHitSound(SGD::HAudio _hitSound)
{
	m_hHit = _hitSound;
}

void Projectile::SetImpactSound(SGD::HAudio _impactSound)
{
	m_hImpact = _impactSound;
}