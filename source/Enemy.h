#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Listener.h"

#include "AIComponent.h"

class Enemy : public Entity, public SGD::Listener
{
public:

	Enemy();
	~Enemy();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void HandleCollision(const IEntity* pOther);

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	int GetMaxHealth() const;
	int GetCurrHealth() const;
	float GetAttackRate() const;
	float GetAttackRange() const;
	float GetSpeed() const;
	float GetHealthChance() const;
	float GetAmmoChance() const;
	float GetSuperChance() const;
	bool GetInLava() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetMaxHealth(int maxHealth);
	void SetCurrHealth(int currHealth);
	void SetAttackRate(float attackRate);
	void SetAttackRange(float attackRange);
	void SetSpeed(float speed);
	void SetPlayer(Entity* player);
	void SetHealthChance(float chance);
	void SetAmmoChance(float chance);
	void SetSuperChance(float chance);
	void SetInLava(bool slowed);

protected:

	/**********************************************************/
	// Members
	int m_nDamage;
	int m_nMaxHeatlh;
	int m_nCurrHealth;
	float m_fAttackRate;
	float m_fAttackRange;
	float m_fSpeed;
	float m_fTrapTimer;
	float m_fHealthChance;
	float m_fAmmoChance;
	float m_fSuperChance;
	bool m_bIsTrapped;
	bool m_bIsInLava;
	AIComponent m_AIComponent;
};

