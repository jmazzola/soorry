#pragma once
#include "Pickup.h"
class HealthPickup :
	public Pickup
{
public:
	HealthPickup () = default;
	~HealthPickup () = default;

	virtual int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);

};

