#include "CreateTowerMessage.h"

#include "MessageID.h"

CreateTowerMessage::CreateTowerMessage(int _x, int _y, int _towerType) : Message(MessageID::MSG_CREATE_TOWER)
{
	x = _x;
	y = _y;
	towerType = _towerType;
}


CreateTowerMessage::~CreateTowerMessage()
{
}
