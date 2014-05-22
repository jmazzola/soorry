#pragma once
#include "../SGD Wrappers/SGD_Message.h"

class CreateSlowZombieMessage : public SGD::Message
{
public:
	CreateSlowZombieMessage(int _x, int _y);
	~CreateSlowZombieMessage();

	int GetX() const;
	int GetY() const;

protected:

	int m_nX;
	int m_nY;

};

