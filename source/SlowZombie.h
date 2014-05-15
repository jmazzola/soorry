#pragma once

#include "Enemy.h"

class SlowZombie : public Enemy
{
public:

	SlowZombie();
	~SlowZombie();

	/**********************************************************/
	// Interface Methods
	virtual int GetType() const override;
};

