#include "CreatePlaceableMessage.h"
#include "MessageID.h"

CreatePlaceableMessage::CreatePlaceableMessage(SGD::Point pt, int id) : Message(MessageID::MSG_CREATE_PLACEABLE)
{
	m_ptPosition.x = pt.x;
	m_ptPosition.y = pt.y;
	m_nPlaceableType = id;
}

