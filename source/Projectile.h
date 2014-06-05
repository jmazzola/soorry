#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_Handle.h"

class Entity;
class EntityManager;
class Enemy;

class Projectile : public Entity
{
public:

	Projectile();
	~Projectile();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	float GetSpeed() const;
	float GetLifeTime() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetSpeed(float speed);
	void SetLifeTime(float lifeTime);

protected:

	/**********************************************************/
	// Members
	int m_nDamage;
	float m_fSpeed;
	float m_fLifeTime;
	SGD::HAudio m_hHit;
	// Enemy
	Enemy* m_pEnemy;
};

