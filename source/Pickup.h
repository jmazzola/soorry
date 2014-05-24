#pragma once
#include "Entity.h"
class Pickup :
	public Entity
{
public:
	Pickup();
	~Pickup();

	virtual void Update(float dt);
	virtual int GetType() const;


private:
	/**********************************************************/
	// Members
	int m_nPickupID;
	float m_fRadius;
};

