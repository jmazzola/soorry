#include "AmmoPickup.h"
#include "DestroyEntityMessage.h"

int AmmoPickup::GetType() const
{
	return ENT_PICKUP_AMMO;
}

void AmmoPickup::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;
	}
}
