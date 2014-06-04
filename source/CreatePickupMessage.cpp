#include "CreatePickupMessage.h"
#include "MessageID.h"


CreatePickupMessage::CreatePickupMessage(int id, SGD::Point pos) : Message(MessageID::MSG_CREATE_PICKUP)
{
	m_nPickupID = id;
	m_ptPosition = pos;
}

