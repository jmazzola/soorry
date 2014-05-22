#pragma once
#include "../SGD Wrappers/SGD_Message.h"
class CreateBeaverZombieMessage : public SGD::Message
{
public:
	CreateBeaverZombieMessage(int _x,int _y);
	~CreateBeaverZombieMessage();
	int GetX() const;
	int GetY() const;

protected:

	int m_nX;
	int m_nY;

};

