#pragma once

#include "Tower.h"

class MachineGunTower : public Tower
{
public:

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual int GetType() const override;

	MachineGunTower();
	~MachineGunTower();

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	float GetFireRate() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetFireRate(float fireRate);

protected:

	/**********************************************************/
	// Data members
	int m_nDamage;
	float m_fFireRate;
	float m_fBulletSpeed;

private:

	float m_fNextShotTimer;
};

