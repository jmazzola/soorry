#pragma once

#include "Enemy.h"

class BeaverZombie : public Enemy
{
public:

	BeaverZombie();
	~BeaverZombie();

	/**********************************************************/
	// Interface Methods
	virtual int GetType() const override;
};

