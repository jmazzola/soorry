#pragma once

#include "Enemy.h"

class FastZombie : public Enemy
{
public:

	FastZombie();
	~FastZombie();

	/**********************************************************/
	// Interface Methods
	virtual void Render() override;
	virtual int GetType() const override;
};

