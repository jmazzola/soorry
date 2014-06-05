#include "CreateDroneMessage.h"
#include "MessageID.h"

CreateDroneMessage::CreateDroneMessage(Drone* _Drone) : SGD::Message(MessageID::MSG_CREATE_DRONE)
{
	m_pDrone = _Drone;
}


CreateDroneMessage::~CreateDroneMessage()
{
}

Drone*	CreateDroneMessage::GetDrone() const
{
	return m_pDrone;
}
void	CreateDroneMessage::SetDrone(Drone* _Drone)
{
	m_pDrone = _Drone;
}