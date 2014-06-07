#include "CreateGrenadeMessage.h"
#include "MessageID.h"

CreateGrenadeMessage::CreateGrenadeMessage (float _x, float _y, SGD::Vector _force) : Message(MessageID::MSG_CREATE_GRENADE)
{
	x = _x;
	y = _y;
	force = _force;
}


CreateGrenadeMessage::~CreateGrenadeMessage ()
{
}
