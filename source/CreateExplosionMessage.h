#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class CreateExplosionMessage : public SGD::Message
{
public:
	CreateExplosionMessage(float _x, float _y, float damage, float radius);
	~CreateExplosionMessage();

	float x;
	float y;
	float damage;
	float radius;
};

