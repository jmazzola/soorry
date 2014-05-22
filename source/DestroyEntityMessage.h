//***********************************************************************
//	File:		DestroyEntityMessage.h
//	Author:		Doug Monroe
//	Course:		SGD 1403
//	Purpose:	DestroyEntityMessage class holds the entity to be
//				removed from the Entity Manager
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Message.h"	// uses SGD::Message
class Entity;								// uses Entity*


//***********************************************************************
// DestroyEntityMessage class
//	- stores the entity to be removed from the Entity Manager
class DestroyEntityMessage : public SGD::Message
{
public:
	//*******************************************************************
	// Constructor REQUIRES the entity to remove
	DestroyEntityMessage(Entity* ptr);
	virtual ~DestroyEntityMessage(void);

	//*******************************************************************
	// Accessor:
	Entity*		GetEntity(void) const		{ return m_pEntity; }

private:
	//*******************************************************************
	// entity to destroy
	Entity*		m_pEntity = nullptr;
};

