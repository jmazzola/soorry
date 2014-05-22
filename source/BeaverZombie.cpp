#include "BeaverZombie.h"

#include "AnimationManager.h"

BeaverZombie::BeaverZombie()
{
	
}


BeaverZombie::~BeaverZombie()
{
}


/**********************************************************/
// Interface Methods

int BeaverZombie::GetType() const
{
	return ENT_ZOMBIE_BEAVER;
}