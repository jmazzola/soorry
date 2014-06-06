#pragma once
#include "Pickup.h"
class SuperPack :
	public Pickup
{
public:
	SuperPack () = default;
	~SuperPack () = default;

	virtual int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);
};

