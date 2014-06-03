#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
class BearTrap : public Entity, public SGD::Listener
{
public:

	BearTrap();
	~BearTrap() = default;

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther);

	/**********************************************************/
	// Accessors
	int IsTrapping() { return m_bIsTrapping; }

	/**********************************************************/
	// Mutators
	void SetTrap(bool hold) { m_bIsTrapping = hold; }

private:
	bool m_bIsTrapping;

	bool m_bActivated;
};

