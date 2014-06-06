#include "SuperPack.h"
#include "DestroyEntityMessage.h"

int SuperPack::GetType() const
{
	return ENT_PICKUP_SUPER;
}

void SuperPack::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
		pmsg = nullptr;
	}
}
