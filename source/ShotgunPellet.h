#pragma once
#include "Projectile.h"
class ShotgunPellet :
	public Projectile
{
public:
	ShotgunPellet();
	~ShotgunPellet();
	virtual int GetType() const override;

};

