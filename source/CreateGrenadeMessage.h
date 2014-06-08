#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class CreateGrenadeMessage :
	public SGD::Message
{
public:
	CreateGrenadeMessage (float _x, float _y, SGD::Vector _force);
	~CreateGrenadeMessage ();

	float x, y;
	SGD::Vector force;
};

