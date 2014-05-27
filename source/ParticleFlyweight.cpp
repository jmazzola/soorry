#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

ParticleFlyweight::ParticleFlyweight()
{
}


ParticleFlyweight::~ParticleFlyweight()
{
	/*if (image == INVALID_HANDLE)
	{*/
		SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
	//}
}

