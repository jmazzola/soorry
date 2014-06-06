#pragma once
#include "Pickup.h"
class AmmoPickup :
	public Pickup
{
public:
	AmmoPickup() = default;
	~AmmoPickup() = default;

	virtual int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);
};

