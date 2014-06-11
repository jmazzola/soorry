/***************************************************************
|	File:		CreateShopMessage.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle the message to create a shop
|				entity at a given location
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class CreateShopMessage : public SGD::Message
{

public:
	CreateShopMessage(int x, int y);
	~CreateShopMessage();

	/**********************************************************/
	// Accessors
	int GetX() const;
	int GetY() const;
	
protected:
	
	// X and Y coords of the shop spawn point
	int m_nX, m_nY;
};

