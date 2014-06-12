#include "CreateExplosionMessage.h"
#include "MessageID.h"


CreateExplosionMessage::CreateExplosionMessage(float _x, float _y, float _damage, float _radius) : Message(MessageID::MSG_CREATE_EXPLOSION)
{
	x = _x;
	y = _y;
	damage = _damage;
	radius = _radius;
}


CreateExplosionMessage::~CreateExplosionMessage()
{
}