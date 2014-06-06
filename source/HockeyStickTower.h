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
	virtual int GetType() const override;

protected:

	/**********************************************************/
	// Data members
	float m_fRadius;
	float m_fRotationRate;
	vector<EnemyHit> m_vEnemiesHit;

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
