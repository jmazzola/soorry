#include "BearTrap.h"
#include "DestroyEntityMessage.h"
BearTrap::BearTrap() : Listener(this)
{
	RegisterForEvent("Trap_Zombie");
}
/*virtual*/ void BearTrap::Update(float dt) /*override*/
{
	if (m_bActivated == true)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;
		
	}
}
/*virtual*/ int BearTrap::GetType() const /*override*/
{
	return EntityType::ENT_TRAP_BEARTRAP;
}
/*virtual*/ void BearTrap::HandleCollision(const IEntity* pOther)
{
	m_bActivated = true;
}