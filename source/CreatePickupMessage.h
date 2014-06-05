#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"
class CreatePickupMessage : public SGD::Message
{
public:
	CreatePickupMessage(int id, SGD::Point pos);
	~CreatePickupMessage() = default;

	int GetPickUpID() const { return m_nPickupID; }
	SGD::Point GetPosition() const { return m_ptPosition; }

protected:
	int m_nPickupID;
	SGD::Point m_ptPosition;
};

