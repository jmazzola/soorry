#pragma once
#include "Pickup.h"
class WallPickup :
	public Pickup
{
public:
	WallPickup() = default;
	~WallPickup() = default;

	int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);

};

