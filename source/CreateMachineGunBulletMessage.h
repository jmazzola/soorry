#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"


class CreateMachineGunBulletMessage : public SGD::Message
{
public:

	CreateMachineGunBulletMessage(int _x, int _y, SGD::Vector velocity, int _damage);
	~CreateMachineGunBulletMessage();

	int x, y;
	int damage;
	SGD::Vector velocity;
};

