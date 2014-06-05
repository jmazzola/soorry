#pragma once
#include "../SGD Wrappers/SGD_Message.h"
class CreateProjectileMessage : public SGD::Message
{
public:
	CreateProjectileMessage(int _num);
	~CreateProjectileMessage();

	int GetWeaponNumber() const;
protected:
	int m_nWeaponNumber;
};

