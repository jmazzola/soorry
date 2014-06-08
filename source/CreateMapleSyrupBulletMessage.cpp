#include "CreateMapleSyrupBulletMessage.h"

#include "MessageID.h"

CreateMapleSyrupBulletMessage::CreateMapleSyrupBulletMessage(int _x, int _y, SGD::Vector _velocity, float _slowTime) : Message(MessageID::MSG_CREATE_MAPLE_SYRUP_BULLET)
{
	x = _x;
	y = _y;
	velocity = _velocity;
	slowTime = _slowTime;
}


CreateMapleSyrupBulletMessage::~CreateMapleSyrupBulletMessage()
{
}
