#include "Enemy.h"

#include "AIComponent.h"


Enemy::Enemy()
{
	m_AIComponent.SetAgent(this);
}


Enemy::~Enemy()
{
}


/**********************************************************/
// Interface Methods

void Enemy::Update(float dt)
{
	m_AIComponent.Update(dt);
}

int Enemy::GetType() const
{
	return ENT_ENEMY;
}

/**********************************************************/
// Accessors

int Enemy::GetDamage() const
{
	return m_nDamage;
}

int Enemy::GetMaxHealth() const
{
	return m_nMaxHeatlh;
}

int Enemy::GetCurrHealth() const
{
	return m_nCurrHealth;
}

float Enemy::GetAttackRate() const
{
	return m_fAttackRate;
}

float Enemy::GetAttackRange() const
{
	return m_fAttackRange;
}

float Enemy::GetSpeed() const
{
	return m_fSpeed;
}

/**********************************************************/
// Mutators

void Enemy::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void Enemy::SetMaxHealth(int _maxHealth)
{
	m_nMaxHeatlh = _maxHealth;
}

void Enemy::SetCurrHealth(int _currHealth)
{
	m_nCurrHealth = _currHealth;
}

void Enemy::SetAttackRate(float _attackRate)
{
	m_fAttackRate = _attackRate;
}

void Enemy::SetAttackRange(float _attackRange)
{
	m_fAttackRange = _attackRange;
}

void Enemy::SetSpeed(float _speed)
{
	m_fSpeed = _speed;
}

void Enemy::SetPlayer(Entity* _player)
{
	m_AIComponent.SetPlayer(_player);
}