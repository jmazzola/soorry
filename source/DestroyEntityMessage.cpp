//***********************************************************************
//	File:		DestroyEntityMessage.cpp
//	Author:		Doug Monroe
//	Course:		SGD 1403
//	Purpose:	DestroyEntityMessage class holds the entity to be
//				removed from the Entity Manager
//***********************************************************************

#include "DestroyEntityMessage.h"

#include "MessageID.h"
#include "Entity.h"
#include <cassert>


//***********************************************************************
// CONSTRUCTOR
//	- initialize parent with message ID
//	- store a reference the entity parameter
DestroyEntityMessage::DestroyEntityMessage(Entity* ptr)
: Message(MessageID::MSG_DESTROY_ENTITY)
{
	// Validate the parameter
	assert(ptr != nullptr && "DestroyEntityMessage - parameter cannot be NULL!");

	// Store the parameter
	m_pEntity = ptr;
	m_pEntity->AddRef();
}

//***********************************************************************
// DESTRUCTOR
//	- release the reference to entity
DestroyEntityMessage::~DestroyEntityMessage()
{
	// Release the member's reference
	m_pEntity->Release();
	m_pEntity = nullptr;
}
