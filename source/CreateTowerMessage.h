#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class CreateTowerMessage : public SGD::Message
{
public:

	enum TowerType { TOWER_MACHINE_GUN, TOWER_MAPLE_SYRUP, TOWER_HOCKEY_STICK, TOWER_LASER, };

	CreateTowerMessage(int _x, int _y, int _towerType);
	~CreateTowerMessage();

	int x, y;
	int towerType;
};

