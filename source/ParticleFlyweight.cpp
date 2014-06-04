#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

ParticleFlyweight::ParticleFlyweight()
{
}


ParticleFlyweight::~ParticleFlyweight()
{
	if (image != SGD::INVALID_HANDLE)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
		image = SGD::INVALID_HANDLE;
	}
}

