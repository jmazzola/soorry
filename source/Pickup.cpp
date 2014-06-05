#include "Pickup.h"


Pickup::Pickup()
{
}


Pickup::~Pickup()
{
}

/*virtual*/ void Pickup::Update(float dt)
{

}
/*virtual*/ int Pickup::GetType() const
{
	return ENT_PICKUP;
}
