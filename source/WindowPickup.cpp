#include "WindowPickup.h"
#include "DestroyEntityMessage.h"

int WindowPickup::GetType() const
{
	return ENT_PICKUP_WINDOW;
}

/*virtual*/ void WindowPickup::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;
	}
}
