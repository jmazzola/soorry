#pragma once

#include "Tower.h"

#include "Enemy.h"

struct EnemyHit;

class HockeyStickTower : public Tower
{
public:

	HockeyStickTower();
	~HockeyStickTower();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void DrawMenu() override;
	virtual int GetType() const override;
	virtual void Upgrade(int _slot, unsigned int* _points) override;

	/**********************************************************/
	// Mutators
	void SetRotationRate(float rotationRate);
	void SetDamage(float damage);

protected:

	/**********************************************************/
	// Data members
	float m_fRadius;
	float m_fRotationRate;
	float m_fDamage;
	vector<EnemyHit> m_vEnemiesHit;

	virtual void ExternalUpgrade() override;

private:

	/**********************************************************/
	// Helper functions
	bool CheckHitList(IEntity* enemy) const;
};


struct EnemyHit
{
	EnemyHit(Enemy* _enemy) :
	enemy(_enemy),
	angleRemaining(3.0f)
	{ }

	float angleRemaining;
	Enemy* enemy;
};
