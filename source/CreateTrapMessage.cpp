#include "CreateTrapMessage.h"
#include "MessageID.h"

CreateTrapMessage::CreateTrapMessage (int _x, int _y, int _trapType) : Message(MessageID::MSG_CREATE_TRAP)
{
	x = _x;
	y = _y;
	trapType = _trapType;
}


CreateTrapMessage::~CreateTrapMessage ()
{
}
