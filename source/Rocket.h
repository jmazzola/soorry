#pragma once
#include "Projectile.h"
#include "IEntity.h"
class Rocket :
	public Projectile
{
public:
	Rocket();
	~Rocket();
	virtual int GetType() const override;


};

