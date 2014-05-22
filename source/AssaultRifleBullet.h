#pragma once
#include "Projectile.h"
class AssaultRifleBullet :
	public Projectile
{
public:
	AssaultRifleBullet();
	~AssaultRifleBullet();
protected:
	EntityType m_nType;
};

