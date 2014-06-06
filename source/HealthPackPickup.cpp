#include "HealthPackPickup.h"
#include "DestroyEntityMessage.h"

int HealthPackPickup::GetType() const
{
	return ENT_PICKUP_HEALTHPACK;
}

void HealthPackPickup::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;
	}
}
