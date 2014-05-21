#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

ParticleFlyweight::ParticleFlyweight()
{
}


ParticleFlyweight::~ParticleFlyweight()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
}

