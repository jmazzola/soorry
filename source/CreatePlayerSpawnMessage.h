#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class CreatePlayerSpawnMessage : public SGD::Message
{
public:

	CreatePlayerSpawnMessage(int x, int y);
	~CreatePlayerSpawnMessage();

	/**********************************************************/
	// Accessors
	int GetX() const;
	int GetY() const;

protected:

	int m_nX;
	int m_nY;
};

