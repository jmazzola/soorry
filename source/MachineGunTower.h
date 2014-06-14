#pragma once

#include "Tower.h"

class MachineGunTower : public Tower
{
public:

	MachineGunTower();
	~MachineGunTower();

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
	int GetDamage() const;
	float GetFireRate() const;
	int GetRange() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetFireRate(float fireRate);
	void SetRange(int range);

protected:

	/**********************************************************/
	// Data members
	int m_nDamage;
	float m_fFireRate;
	float m_fBulletSpeed;
	int m_nRange;

	virtual void ExternalUpgrade() override;

private:

	float m_fNextShotTimer;
};

