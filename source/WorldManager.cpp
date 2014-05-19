#include "WorldManager.h"

#include "Tile.h"

#include "../TinyXML/tinyxml.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


WorldManager* WorldManager::GetInstance()
{
	static WorldManager s_Instance;
	return &s_Instance;
}


/**********************************************************/
// Interface

bool WorldManager::LoadWorld(string fileName)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	if (doc.LoadFile(fileName.c_str()) == false)
		return false;

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Make sure the root is there
	if (pRoot == nullptr)
		return false;

	// Load world data
	pRoot->Attribute("worldWidth", &m_nWorldWidth);
	pRoot->Attribute("worldHeight", &m_nWorldHeight);
	pRoot->Attribute("tileWidth", &m_nTileWidth);
	pRoot->Attribute("tileHeight", &m_nTileHeight);
	pRoot->Attribute("tilesetWidth", &m_nTilesetWidth);

	// Load tilesetImage
	const char* tilesetImage = pRoot->Attribute("image");
	m_hTilesetImage = SGD::GraphicsManager::GetInstance()->LoadTexture(tilesetImage);

	// Get the first layer
	TiXmlElement* pLayer = pRoot->FirstChildElement("layer");

	// Go through all the layers
	while (pLayer != nullptr)
	{
		// Create a layer
		Layer layer(m_nWorldWidth, m_nWorldHeight);

		// Get the first tile
		TiXmlElement* pTile = pLayer->FirstChildElement("tile");

		// Go through all the tiles
		while (pTile != nullptr)
		{
			// Create a tile
			Tile tile;

			// Load tile's values
			int x, y, tileID, collisionID;
			pTile->Attribute("x", &x);
			pTile->Attribute("y", &y);
			pTile->Attribute("index", &tileID);
			string collider = pTile->Attribute("collider");
			string triggerInit = pTile->FirstChildElement("initial_trigger")->Attribute("id");
			string triggerCollision = pTile->FirstChildElement("collision_trigger")->Attribute("id");
			pTile->FirstChildElement("collision")->Attribute("id", &collisionID);

			// Set tile's values
			tile.SetTileID(tileID);
			(collider == "true") ? tile.SetCollidable(true) : tile.SetCollidable(false);
			tile.SetColliderID(collisionID);
			tile.SetTriggerInit(triggerInit);
			tile.SetTriggerCollision(triggerCollision);

			// Place tile in layer
			layer[x][y] = tile;

			// Move on to next tile
			pTile = pTile->NextSiblingElement("tile");
		}

		// Append layer
		m_vLayers.push_back(layer);

		// Move on to next layer
		pLayer = pLayer->NextSiblingElement("layer");
	}

	// Are there any layers?
	return (m_vLayers.size() > 0);
}

void WorldManager::UnloadWorld()
{
	// Unload all tiles from all layers
	for (unsigned int i = 0; i < m_vLayers.size(); i++)
		m_vLayers[i].UnloadTiles();

	// Clear out layers
	m_vLayers.clear();

	// Unload tilesetImage
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hTilesetImage);
}

void WorldManager::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Loop through each layer
	for (unsigned int layer = 0; layer < m_vLayers.size(); layer++)
	{
		// Loop through ENTIRE tile array (to be replaced with culling)
		for (int x = 0; x < m_nWorldWidth; x++)
		{
			for (int y = 0; y < m_nWorldHeight; y++)
			{
				// Get tile information
				int tileID = m_vLayers[layer][x][y].GetTileID();

				// Cell algorithm
				SGD::Rectangle sourceRect;
				sourceRect.left = (tileID % m_nTilesetWidth) * m_nTileWidth;
				sourceRect.top = (tileID / m_nTilesetWidth) * m_nTileHeight;
				sourceRect.right = sourceRect.left + m_nTileWidth;
				sourceRect.bottom = sourceRect.top + m_nTileHeight;

				// Draw the tile
				pGraphics->DrawTextureSection(m_hTilesetImage, SGD::Point(x * m_nTileWidth, y * m_nTileHeight), sourceRect);
			}
		}
	}
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

int WorldManager::GetTilesetWidth() const
{
	return m_nTilesetWidth;
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

void WorldManager::SetTilesetWidth(int _tilesetWidth)
{
	m_nTilesetWidth = _tilesetWidth;
}

void WorldManager::SetTilesetImage(SGD::HTexture _tilesetImage)
{
	m_hTilesetImage = _tilesetImage;
}