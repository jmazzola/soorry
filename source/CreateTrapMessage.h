#pragma once
#include "../SGD Wrappers/SGD_Message.h"

class CreateTrapMessage :
	public SGD::Message
{
public:

	enum TrapType {TRAP_SPIKE, TRAP_LAVA};

	CreateTrapMessage (int x, int y, int trapType);
	~CreateTrapMessage ();

	int x, y;
	int trapType;
};

