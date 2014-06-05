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
	float m_fRotationRate;
	vector<EnemyHit> m_vEnemiesHit;
};


struct EnemyHit
{
	float angleRemaining;
	Enemy* enemy;
};
