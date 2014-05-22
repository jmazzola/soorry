#include "CreateSlowZombieMessage.h"
#include "MessageID.h"

CreateSlowZombieMessage::CreateSlowZombieMessage(int _x, int _y) : Message(MessageID::MSG_CREATE_SLOW_ZOMBIE)
{
	m_nX = _x;
	m_nY = _y;
}


CreateSlowZombieMessage::~CreateSlowZombieMessage()
{
}

/**********************************************************/
// Accessors
int CreateSlowZombieMessage::GetX() const
{
	return m_nX;
}

int CreateSlowZombieMessage::GetY() const
{
	return m_nY;
}