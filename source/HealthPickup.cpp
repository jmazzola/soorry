#include "HealthPickup.h"
#include "DestroyEntityMessage.h"

int HealthPickup::GetType() const
{
	return ENT_PICKUP_HEALTHPACK;
}

void HealthPickup::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;
	}
}
