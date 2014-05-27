#pragma once
#include "Projectile.h"
class AssaultRifleBullet :
	public Projectile
{
public:
	AssaultRifleBullet();
	~AssaultRifleBullet();
	virtual int GetType() const override;

protected:
	EntityType m_nType;
};

