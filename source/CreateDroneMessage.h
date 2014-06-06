#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "Drone.h"

class CreateDroneMessage : public SGD::Message
{
public:
	CreateDroneMessage();
	~CreateDroneMessage();
private:
};

