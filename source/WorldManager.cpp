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


/**********************************************************/
// Interface

bool WorldManager::LoadWorld(string fileName)
{
	return false;
}

void WorldManager::Render()
{

}

bool WorldManager::CheckCollision(IEntity* object)
{
	return false;
}

/**********************************************************/
// Accessors

int WorldManager::GetWorldWidth() const
{
	return m_nWorldWidth;
}

int WorldManager::GetWorldHeight() const
{
	return m_nWorldHeight;
}

int WorldManager::GetTileWidth() const
{
	return m_nTileWidth;
}

int WorldManager::GetTileHeight() const
{
	return m_nTileHeight;
}

SGD::HTexture WorldManager::GetTilesetImage() const
{
	return m_hTilesetImage;
}

/**********************************************************/
// Mutators

void WorldManager::SetWorldWidth(int _worldWidth)
{
	m_nWorldWidth = _worldWidth;
}

void WorldManager::SetWorldHeight(int _worldHeight)
{
	m_nWorldHeight = _worldHeight;
}

void WorldManager::SetTileWidth(int _tileWidth)
{
	m_nTileWidth = _tileWidth;
}

void WorldManager::SetTileHeight(int _tileHeight)
{
	m_nTileHeight = _tileHeight;
}

void WorldManager::SetTilesetImage(SGD::HTexture _tilesetImage)
{
	m_hTilesetImage = _tilesetImage;
}