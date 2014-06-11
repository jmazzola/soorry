#include "Enemy.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "AIComponent.h"
#include "CreatePickupMessage.h"
#include "DestroyEntityMessage.h"
#include "GameplayState.h"
#include "MachineGunBullet.h"
#include "MapleSyrupBullet.h"
#include "Camera.h"
#include "Game.h"
#include "SpikeTrap.h"
#include "LavaTrap.h"
#include "Grenade.h"
#include "CreateParticleMessage.h"
#include "TrickShotBullet.h"
#include "StatTracker.h"

#define HEALTH_BAR 1


Enemy::Enemy() : Listener(this)
{
	m_AIComponent.SetAgent(this);
	m_fTrapTimer = 0;
	m_nCurrHealth = 100;
	m_nMaxHeatlh = 100;
	m_fSlowTime = 0.0f;
	m_bIsInLava = false;
	RegisterForEvent("GRENADE_EXPLOSION");
}


Enemy::~Enemy()
{
	UnregisterFromEvent("GRENADE_EXPLOSION");
}


/**********************************************************/
// Interface Methods

void Enemy::Update(float dt)
{
	// Update timers
	m_fTrapTimer -= dt;
	m_fSlowTime -= dt;

	if (m_nCurrHealth > 0 && m_fTrapTimer < 0)
	{
		m_AIComponent.Update(dt);
		
		m_nCurrHealth += m_fRegeneration * dt;
		if (m_nCurrHealth > m_nMaxHeatlh)
			m_nCurrHealth = m_nMaxHeatlh;
	}

	else if (m_nCurrHealth <= 0)
	{
		switch ( GetType() )
		{
		case ENT_ZOMBIE_BEAVER:
			StatTracker::GetInstance()->SpillBlood(1.1f);
			break;
		case ENT_ZOMBIE_FAST:
			StatTracker::GetInstance()->SpillBlood(5.5f);
			break;
		case ENT_ZOMBIE_SLOW:
			StatTracker::GetInstance()->SpillBlood(7.3f);
			break;
		}

		float chance = (float)((float)(rand() % 1000 + 1) / 1000.0f);

		if(chance >= 0 && chance <= m_fHealthChance)
		{
			CreatePickupMessage*  pmsg = new CreatePickupMessage(ENT_PICKUP_HEALTHPACK, m_ptPosition);
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
		else if(chance > m_fHealthChance && chance <= m_fHealthChance + m_fSuperChance)
		{
			CreatePickupMessage*  pmsg = new CreatePickupMessage(ENT_PICKUP_SUPER, m_ptPosition);
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
		else if( chance > m_fHealthChance + m_fSuperChance && chance <= m_fAmmoChance)
		{
			CreatePickupMessage*  pmsg = new CreatePickupMessage(ENT_PICKUP_AMMO, m_ptPosition);
			pmsg->QueueMessage();
			pmsg = nullptr;
		}


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
	m_bIsInLava = false;
}

void Enemy::Render()
{
	Entity::Render();

	m_AIComponent.Render();
}

void Enemy::PostRender()
{
	// Draw health bar
#if HEALTH_BAR

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	SGD::Rectangle backRect;
	backRect.left = m_ptPosition.x - Camera::x;
	backRect.top = m_ptPosition.y - 5 - Camera::y;
	backRect.right = m_ptPosition.x + 32 - Camera::x;
	backRect.bottom = m_ptPosition.y - Camera::y;

	// Get offset from max health
	int offset = (int)(30 * (m_nMaxHeatlh - m_nCurrHealth) / (float)(m_nMaxHeatlh));

	SGD::Rectangle frontRect;
	frontRect.left = backRect.left + 1;
	frontRect.top = backRect.top + 1;
	frontRect.right = backRect.right - 1 - offset;
	frontRect.bottom = backRect.bottom - 1;

	pGraphics->DrawRectangle(backRect, SGD::Color(0, 0, 0));

	// Determine bar color
	SGD::Color color = { (unsigned char)(255 * (float)((m_nMaxHeatlh - m_nCurrHealth) / (float)(m_nMaxHeatlh))), (unsigned char)(255 * (float)(m_nCurrHealth / (float)(m_nMaxHeatlh))), 0 };
	pGraphics->DrawRectangle(frontRect, color);

#endif

	// -- Debugging, if showing paths is on --
	Game* pGame = Game::GetInstance();
	if (pGame->IsShowingPaths())
		m_AIComponent.Render();
}

int Enemy::GetType() const
{
	return ENT_ENEMY;
}

/*virtual*/ void Enemy::HandleCollision(const IEntity* pOther)
{
	int pastHealth = (int)m_nCurrHealth;
	int type = pOther->GetType();
	switch (pOther->GetType())
	{
	case ENT_BULLET_ASSAULT:
	{
		m_nCurrHealth -= 40;
	}
			break;
	case ENT_BULLET_SHOTGUN:
	{
		m_nCurrHealth -= 8;
	}
			break;
	case ENT_BULLET_ROCKET:
	{
		m_nCurrHealth -= 100;
	}
			break;
	case ENT_BULLET_TRICKSHOT:
	{
		const TrickShotBullet* tsb = reinterpret_cast<const TrickShotBullet*>(pOther);
		if(tsb->AmIOnTheHitList((IEntity *)this) == false)
		{
			SGD::Event* pEvent = new SGD::Event("IM_HIT", nullptr, this);
			pEvent->QueueEvent();
			m_nCurrHealth -= tsb->GetDamage();
		}
		break;
	}
	case ENT_TRAP_BEARTRAP:
	{
		m_bIsTrapped = true;
	}
			break;
	case ENT_MACHINE_GUN_BULLET:
	{
		m_nCurrHealth -= dynamic_cast<const MachineGunBullet*>(pOther)->GetDamage();
	}
			break;
	case ENT_MAPLE_SYRUP_BULLET:
		if (m_fSlowTime < 0.0f)
			m_fSlowTime = 0.0f;
		m_fSlowTime += dynamic_cast<const MapleSyrupBullet*>(pOther)->GetSlowTime();
		break;
		//NOTE: may have to delete
	case ENT_TRAP_MINE:
		m_nCurrHealth = 0;
		break;
	case ENT_TRAP_SPIKE:
	{
		const SpikeTrap* spike = dynamic_cast<const SpikeTrap*>(pOther);
		// If the spikes are up do take damage
		if (spike->GetActive() == true)
		{
			m_nCurrHealth -= spike->GetDamage();
			
		}
	}
		break;
	case ENT_TRAP_LAVA:
	{
		const LavaTrap* lava = dynamic_cast<const LavaTrap*>(pOther);
		m_nCurrHealth -= lava->GetDamage();
		m_bIsInLava = true;
		
	}
		break;
	}
		if (pastHealth > m_nCurrHealth)
		{
			CreateParticleMessage* msg = new CreateParticleMessage("Blood_Particle1", this, 8, 8);
			msg->QueueMessage();
			msg = nullptr;
		}
}

void Enemy::HandleEvent(const SGD::Event* pEvent)
{
	if(pEvent->GetEventID() == "GRENADE_EXPLOSION")
	{
		const Grenade* grenade = reinterpret_cast<const Grenade*>(pEvent->GetSender());
		SGD::Point a = grenade->GetPosition();
		SGD::Point b = m_ptPosition;
		float distance = sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
		if(distance <= grenade->GetRadius())
			m_nCurrHealth -= grenade->GetDamage() * (distance/grenade->GetRadius());
	}

}

/**********************************************************/
// Accessors

int Enemy::GetDamage() const
{
	return m_nDamage;
}

float Enemy::GetMaxHealth() const
{
	return m_nMaxHeatlh;
}

float Enemy::GetCurrHealth() const
{
	return m_nCurrHealth;
}

float Enemy::GetRegeneration() const
{
	return m_fRegeneration;
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
	float speed = m_fSpeed;

	if (m_bIsInLava)
		speed *= 0.5f;

	if (m_fSlowTime > 0.0f)
		speed *= 0.2f;

	return speed;
}

float Enemy::GetHealthChance() const
{
	return m_fHealthChance;
}

float Enemy::GetAmmoChance() const
{
	return m_fAmmoChance;
}

float Enemy::GetSuperChance() const
{
	return m_fSuperChance;
}

bool Enemy::GetInLava() const
{
	return m_bIsInLava;
}

AIComponent* Enemy::GetAIComponent()
{
	return &m_AIComponent;
}

/**********************************************************/
// Mutators

void Enemy::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void Enemy::SetMaxHealth(float _maxHealth)
{
	m_nMaxHeatlh = _maxHealth;
}

void Enemy::SetCurrHealth(float _currHealth)
{
	m_nCurrHealth = _currHealth;
}

void Enemy::SetRegeneration(float _regeneration)
{
	m_fRegeneration = _regeneration;
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

void Enemy::SetHealthChance(float _chance)
{
	m_fHealthChance = _chance;
}

void Enemy::SetAmmoChance(float _chance)
{
	m_fAmmoChance = _chance;
}

void Enemy::SetSuperChance(float _chance)
{
	m_fSuperChance = _chance;
}

void Enemy::SetInLava(bool yes)
{
	m_bIsInLava = yes;
}