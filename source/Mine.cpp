#include "Mine.h"
#include "DestroyEntityMessage.h"
//maybe have the collision rect HUGE to add zombies to the listener event then get track of the distance between them. then when the position approaches the cente send the event
/*virtual*/ void Mine::Update(float dt) /*override*/
{
	
}

/*virtual*/ int Mine::GetType() const /*override*/
{
	return EntityType::ENT_TRAP_MINE;
}

/*virtual*/ void Mine::HandleCollision(const IEntity* pOther)
{
	int type = pOther->GetType();
	
	if (type == ENT_ZOMBIE_BEAVER || type == ENT_ZOMBIE_FAST || type == ENT_ZOMBIE_SLOW)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
	
}
