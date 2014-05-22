#pragma once
#include "../SGD Wrappers/SGD_Message.h"

class CreateFastZombieMessage : public SGD::Message
{
public:
	CreateFastZombieMessage(int _x, int _y);
	~CreateFastZombieMessage();
	int GetX() const;
	int GetY() const;

protected:

	int m_nX;
	int m_nY;
};

