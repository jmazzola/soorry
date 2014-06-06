#pragma once
#include "Pickup.h"
class HealthPackPickup :
	public Pickup
{
public:
	HealthPackPickup() = default;
	~HealthPackPickup() = default;

	int GetType() const;
	void HandleCollision(const IEntity* pOther);
};

