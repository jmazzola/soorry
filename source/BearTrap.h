#pragma once

#include "Entity.h"

class BearTrap : public Entity
{
public:

	BearTrap() = default;
	~BearTrap() = default;

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;

	/**********************************************************/
	// Accessors
	int IsTrapping() { return m_bIsTrapping; }

	/**********************************************************/
	// Mutators
	void SetTrap(bool hold) { m_bIsTrapping = hold; }

private:
	bool m_bIsTrapping;
};

