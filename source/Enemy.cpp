#include "Enemy.h"

#include "../SGD Wrappers/SGD_Event.h"

#include "AIComponent.h"
#include "DestroyEntityMessage.h"
#include "GameplayState.h"

Enemy::Enemy() : Listener(this)
{
	m_AIComponent.SetAgent(this);
	m_fTrapTimer = 0;
}


Enemy::~Enemy()
{
}


/**********************************************************/
// Interface Methods

void Enemy::Update(float dt)
{
	m_fTrapTimer -= dt;
	if (m_nCurrHealth > 0 && m_fTrapTimer < 0)
		m_AIComponent.Update(dt);
	else if (m_nCurrHealth <= 0)
	{
		// Get rid of that bitch
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;

		// Increase player's score
		int score = 20;
		SGD::Event e("INCREASE_SCORE", (void*)&score);
		e.SendEventNow();
		
		// Update the ZombieFactory to adjust count
		ZombieFactory* z = GameplayState::GetInstance()->GetZombieFactory();
		z->SetEnemiesRemaining( z->GetEnemiesRemaining() - 1);
	}
	if (m_bIsTrapped == true)
	{
		m_fTrapTimer = 2;
		m_bIsTrapped = false;
	}
}

void Enemy::Render()
{
	Entity::Render();
	//m_AIComponent.Render();
}

int Enemy::GetType() const
{
	return ENT_ENEMY;
}

/*virtual*/ void Enemy::HandleCollision(const IEntity* pOther)
{
	int type = pOther->GetType();
	switch (pOther->GetType())
	{
		case ENT_BULLET_ASSAULT:
			m_nCurrHealth -= 40;
			break;
		case ENT_BULLET_SHOTGUN:
			m_nCurrHealth -= 80;
			break;
		case ENT_BULLET_ROCKET:
			m_nCurrHealth -= 100;
			break;
		case ENT_TRAP_BEARTRAP:
			m_bIsTrapped = true;
			break;
			//NOTE: may have to delete
		case ENT_TRAP_MINE:
			m_nCurrHealth = 0;
			break;

	}
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