#pragma once

#include "Tower.h"

class MapleSyrupTower : public Tower
{
public:

	MapleSyrupTower();
	~MapleSyrupTower();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual int GetType() const override;

	/**********************************************************/
	// Accessors
	float GetSlowTime() const;
	float GetFireRate() const;

	/**********************************************************/
	// Mutators
	void SetSlowTime(float slowTime);
	void SetFireRate(float fireRate);

protected:

	/**********************************************************/
	// Data members
	float m_fSlowTime;
	float m_fFireRate;
	float m_fBulletSpeed;

private:

	float m_fNextShotTimer;
};

