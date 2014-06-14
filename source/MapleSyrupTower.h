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
	virtual void DrawMenu() override;
	virtual int GetType() const override;
	virtual void Upgrade(int slot, unsigned int* points);

	/**********************************************************/
	// Accessors
	float GetSlowTime() const;
	float GetFireRate() const;
	int GetRange() const;

	/**********************************************************/
	// Mutators
	void SetSlowTime(float slowTime);
	void SetFireRate(float fireRate);
	void SetRange(int range);

protected:

	/**********************************************************/
	// Data members
	float m_fSlowTime;
	float m_fFireRate;
	float m_fBulletSpeed;
	int m_nRange;

	virtual void ExternalUpgrade() override;

private:

	float m_fNextShotTimer;
};

