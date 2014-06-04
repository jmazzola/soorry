#pragma once
#include "Pickup.h"
class WindowPickup :
	public Pickup
{
public:
	WindowPickup() = default;
	~WindowPickup() = default;

	int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);

};

