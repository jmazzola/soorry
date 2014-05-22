#pragma once

#include "Enemy.h"

class BeaverZombie : public Enemy
{
public:

	BeaverZombie();
	~BeaverZombie();

	/**********************************************************/
	// Interface Methods
	virtual void Render() override;
	virtual int GetType() const override;
};

