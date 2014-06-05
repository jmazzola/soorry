#include "CreateBeaverZombieMessage.h"
#include "MessageID.h"

CreateBeaverZombieMessage::CreateBeaverZombieMessage(int _x,int _y) : Message(MessageID::MSG_CREATE_BEAVER_ZOMBIE)
{
	m_nX = _x;
	m_nY = _y;
}


CreateBeaverZombieMessage::~CreateBeaverZombieMessage()
{
}

/**********************************************************/
// Accessors
int CreateBeaverZombieMessage::GetX() const
{
	return m_nX;
}

int CreateBeaverZombieMessage::GetY() const
{
	return m_nY;
}