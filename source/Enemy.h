#pragma once

#include "Entity.h"

/**********************************************************/
// Forward Declarations
class AIComponent;

class Enemy : public Entity
{
public:

	Enemy();
	~Enemy();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	int GetMaxHealth() const;
	int GetCurrHealth() const;
	float GetAttackRate() const;
	float GetAttackRange() const;
	float GetSpeed() const;
	AIComponent* GetAIComponent() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetMaxHealth(int maxHealth);
	void SetCurrHealth(int currHealth);
	void SetAttackRate(float attackRate);
	void SetAttackRange(float attackRange);
	void SetSpeed(float speed);
	void SetAIComponent(AIComponent* aiComponent);

protected:

	/**********************************************************/
	// Members
	int m_nDamage;
	int m_nMaxHeatlh;
	int m_nCurrHealth;
	float m_fAttackRate;
	float m_fAttackRange;
	float m_fSpeed;
	AIComponent* m_pAIComponent;
};

