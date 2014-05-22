#include "Projectile.h"


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
}

int Projectile::GetType() const
{
	return ENT_PROJECTILE;
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