#include "CreateMachineGunBulletMessage.h"

#include "MessageID.h"

CreateMachineGunBulletMessage::CreateMachineGunBulletMessage(int _x, int _y, SGD::Vector _velocity, int _damage) : Message(MessageID::MSG_CREATE_MACHINE_GUN_BULLET)
{
	x = _x;
	y = _y;
	velocity = _velocity;
	damage = _damage;
}


CreateMachineGunBulletMessage::~CreateMachineGunBulletMessage()
{
}
