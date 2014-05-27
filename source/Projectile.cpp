#include "Projectile.h"
#include "GameplayState.h"
#include "Enemy.h"
#include "Entity.h"
#include "EntityManager.h"
#include "DestroyEntityMessage.h"
#include "WorldManager.h"

#include "../SGD Wrappers/SGD_Event.h"


Projectile::Projectile()
{
}


Projectile::~Projectile()
{
}


/**********************************************************/
// Interface Methods

void Projectile::Update(float dt)
{
	Entity::Update(dt);

	if (WorldManager::GetInstance()->CheckCollision(GetRect(), true))
	{
		// Destroy the proj
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
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
		// Destroy the proj
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

/**********************************************************/
// Accessors

int Projectile::GetDamage() const
{
	return m_nDamage;
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

void Projectile::SetSpeed(float _speed)
{
	m_fSpeed = _speed;
}

void Projectile::SetLifeTime(float _lifeTime)
{
	m_fLifeTime = _lifeTime;
}