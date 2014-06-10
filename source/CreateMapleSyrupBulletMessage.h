#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"


class CreateMapleSyrupBulletMessage : public SGD::Message
{
public:

	CreateMapleSyrupBulletMessage(int _x, int _y, SGD::Vector velocity, float _slowTime);
	~CreateMapleSyrupBulletMessage();

	int x, y;
	float slowTime;
	SGD::Vector velocity;
};

