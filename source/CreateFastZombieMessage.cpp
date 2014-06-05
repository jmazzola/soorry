#include "CreateFastZombieMessage.h"
#include "MessageID.h"


CreateFastZombieMessage::CreateFastZombieMessage(int _x, int _y) : Message(MessageID::MSG_CREATE_FAST_ZOMBIE)
{
	m_nX = _x;
	m_nY = _y;
}


CreateFastZombieMessage::~CreateFastZombieMessage()
{
}

/**********************************************************/
// Accessors
int CreateFastZombieMessage::GetX() const
{
	return m_nX;
}

int CreateFastZombieMessage::GetY() const
{
	return m_nY;
}