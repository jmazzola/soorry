#include "WallPickup.h"
#include "DestroyEntityMessage.h"
#include "StatTracker.h"

int WallPickup::GetType() const
{
	return ENT_PICKUP_WALL;
}

/*virtual*/ void WallPickup::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;

		StatTracker::GetInstance()->WallExchange(false);
	}
}
