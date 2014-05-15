#pragma once

#include "Enemy.h"

class FastZombie : public Enemy
{
public:

	FastZombie();
	~FastZombie();

	/**********************************************************/
	// Interface Methods
	virtual int GetType() const override;
};

