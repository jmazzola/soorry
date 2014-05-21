#include "CreatePlayerSpawnMessage.h"

#include "MessageID.h"


CreatePlayerSpawnMessage::CreatePlayerSpawnMessage(int _x, int _y) : Message(MessageID::MSG_CREATE_PLAYER_SPAWN)
{
	m_nX = _x;
	m_nY = _y;
}


CreatePlayerSpawnMessage::~CreatePlayerSpawnMessage()
{
}


/**********************************************************/
// Accessors
int CreatePlayerSpawnMessage::GetX() const
{
	return m_nX;
}

int CreatePlayerSpawnMessage::GetY() const
{
	return m_nY;
}