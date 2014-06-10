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
						MSG_CREATE_SLOW_ZOMBIE, MSG_CREATE_PROJECTILE, MSG_DESTROY_ENTITY, MSG_CREATE_PLACEABLE,
						MSG_CREATE_PICKUP, MSG_CREATE_STATIC_PARTICLE,MSG_CREATE_DYNAMIC_PARTICLE,
						MSG_CREATE_TOWER,  MSG_CREATE_MACHINE_GUN_BULLET, MSG_CREATE_DRONE, MSG_CREATE_TRAP,
						MSG_WAVE_COMPLETE, MSG_CREATE_MAPLE_SYRUP_BULLET, MSG_CREATE_GRENADE, MSG_IM_HIT,
						MSG_CREATE_VECTOR_PARTICLE,
};


#endif //MESSAGEID_H