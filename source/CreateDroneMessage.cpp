#include "CreateDroneMessage.h"
#include "MessageID.h"

CreateDroneMessage::CreateDroneMessage() : SGD::Message(MessageID::MSG_CREATE_DRONE)
{
}


CreateDroneMessage::~CreateDroneMessage()
{
}

