/***************************************************************
|	File:		MessageID.h
|	Author:		
|	Course:		
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H


/**************************************************************/
// MessageID enum
//	- enumerated list of message identifiers
enum class MessageID { MSG_UNKNOWN, 
						MSG_CREATE_PLAYER_SPAWN,MSG_CREATE_BEAVER_ZOMBIE, MSG_CREATE_FAST_ZOMBIE, 
						MSG_CREATE_SLOW_ZOMBIE, MSG_CREATE_PROJECTILE,
};


#endif //MESSAGEID_H