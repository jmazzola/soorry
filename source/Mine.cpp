#include "Mine.h"

/*virtual*/ void Mine::Update(float dt) /*override*/
{

}

/*virtual*/ int Mine::GetType() const /*override*/
{
	return EntityType::ENT_TRAP_MINE;
}
