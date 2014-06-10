#include "CreateShopMessage.h"
#include "MessageID.h"


CreateShopMessage::CreateShopMessage(int x, int y) : Message(MessageID::MSG_CREATE_SHOP)
{
	m_nX = x;
	m_nY = y;
}


CreateShopMessage::~CreateShopMessage()
{
}

// Accessors

int CreateShopMessage::GetX() const
{
	return m_nX;
}


int CreateShopMessage::GetY() const
{
	return m_nY;
}
