#pragma once

#include "Entity.h"

class Mine : public Entity
{
public:
	Mine() = default;
	~Mine() = default;

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther);

	/**********************************************************/
	// Accessors
	int GetDamage() { return m_nDamage; }

	/**********************************************************/
	// Mutators
	void SetDamage(int hurt) { m_nDamage = hurt; }

private:
	int m_nDamage;
};

