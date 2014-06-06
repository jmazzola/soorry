#pragma once
#include "Entity.h"
class Pickup :
	public Entity
{
public:
	enum PickUpType { HEALTHPACK, AMMOPACK};
	Pickup();
	~Pickup();

	virtual void Update(float dt);
	virtual void Render () override;
	virtual int GetType() const;
	//virtual void HandleCollision(const IEntity* pOther);


private:
	/**********************************************************/
	// Members
	int m_nPickupID;
	float m_fRadius;
	float m_fRotation;
};

