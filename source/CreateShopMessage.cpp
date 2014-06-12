#include "CreateShopMessage.h"
#include "MessageID.h"


CreateShopMessage::CreateShopMessage(float x, float y) : Message(MessageID::MSG_CREATE_SHOP)
{
	m_nX = x;
	m_nY = y;
}


CreateShopMessage::~CreateShopMessage()
{
}

// Accessors

float CreateShopMessage::GetX() const
{
	return m_nX;
}


float CreateShopMessage::GetY() const
{
	return m_nY;
}
