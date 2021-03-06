//***********************************************************************
//	File:		DestroyEntityMessage.cpp
//	Author:		Doug Monroe
//	Course:		SGD 1403
//	Purpose:	DestroyEntityMessage class holds the entity to be
//				removed from the Entity Manager
//***********************************************************************

#include "DestroyEntityMessage.h"
#include "MessageID.h"

DestroyEntityMessage::DestroyEntityMessage(Entity* ent) : Message(MessageID::MSG_DESTROY_ENTITY)
{
	m_pEntity = ent;
	m_pEntity->AddRef();
}

DestroyEntityMessage::~DestroyEntityMessage()
{
	m_pEntity->Release();
	m_pEntity = nullptr;
}
