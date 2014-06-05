#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "Drone.h"

class CreateDroneMessage : public SGD::Message
{
public:
	CreateDroneMessage(Drone* _Drone);
	~CreateDroneMessage();
	Drone* GetDrone() const;
	void SetDrone(Drone*);
private:
	Drone* m_pDrone;
};

