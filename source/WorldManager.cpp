#include "WorldManager.h"


WorldManager::WorldManager()
{
}


WorldManager::~WorldManager()
{
}


WorldManager* WorldManager::GetInstance()
{
	static WorldManager s_Instance;
	return &s_Instance;
}