//***********************************************************************
//	File:		DestroyEntityMessage.h
//	Author:		Doug Monroe
//	Course:		SGD 1403
//	Purpose:	DestroyEntityMessage class holds the entity to be
//				removed from the Entity Manager
//***********************************************************************

#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "Entity.h"

class DestroyEntityMessage : public SGD::Message
{
public:
	DestroyEntityMessage(Entity* ent);
	~DestroyEntityMessage();

	Entity* GetEntity() const { return m_pEntity; }

private:
	Entity* m_pEntity;
};

