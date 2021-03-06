#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class CreatePlaceableMessage : public SGD::Message
{
public:
	CreatePlaceableMessage(SGD::Point pt, int id);
	~CreatePlaceableMessage() = default;

	int GetPlaceableType() const { return m_nPlaceableType; }
	SGD::Point GetPlaceablePos() const { return m_ptPosition; }

protected:
	SGD::Point m_ptPosition;
	int m_nPlaceableType;
};

