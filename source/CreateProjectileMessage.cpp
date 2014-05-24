#include "CreateProjectileMessage.h"
#include "MessageID.h"

CreateProjectileMessage::CreateProjectileMessage(int _num) : Message(MessageID::MSG_CREATE_PROJECTILE)
{
	m_nWeaponNumber = _num;
}


CreateProjectileMessage::~CreateProjectileMessage()
{
}


int CreateProjectileMessage::GetWeaponNumber() const
{
	return m_nWeaponNumber;
}
