#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

class MachineGunBullet : public Entity
{
public:

	MachineGunBullet();
	~MachineGunBullet();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	bool IsHit() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetImage(SGD::HTexture image);
	void SetHit(bool hit);

protected:

	bool m_bHit;
	int m_nDamage;
	float m_fLifetime;
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
};

