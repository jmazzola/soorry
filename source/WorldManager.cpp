#include "WorldManager.h"

#include "Tile.h"

#include "../TinyXML/tinyxml.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "CreatePlayerSpawnMessage.h"

#include "IEntity.h"

#include <sstream>
using namespace std;


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
			tile.SetX(x);
			tile.SetY(y);
			tile.SetTileID(tileID);
			(collider == "true") ? tile.SetCollidable(true) : tile.SetCollidable(false);
			tile.SetColliderID(collisionID);
			tile.SetTriggerInit(triggerInit);
			tile.SetTriggerCollision(triggerCollision);

			// Place tile in layer
			layer[x][y] = tile;

			// Send initalize messages
			if (triggerInit != "")
				SendInitialTriggerMessage(tile);

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

void WorldManager::Render(SGD::Point _cameraPos)
{
	// NOTE: cameraPos to be replaced with _cameraPos
#if !CAMERA_IMPLEMENTED

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	const float speed = 0.5f;

	// Move right
	if (pInput->IsKeyDown(SGD::Key::D))
		cameraPos.x += speed;

	// Move up
	if (pInput->IsKeyDown(SGD::Key::W))
		cameraPos.y -= speed;

	// Move left
	if (pInput->IsKeyDown(SGD::Key::A))
		cameraPos.x -= speed;

	// Move down
	if (pInput->IsKeyDown(SGD::Key::S))
		cameraPos.y += speed;

#endif

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Loop through each layer
	for (unsigned int layer = 0; layer < m_vLayers.size(); layer++)
	{
		// Get camera position in terms of tiles
		int camTileX = (int)_cameraPos.x / m_nTileWidth;
		int camTileY = (int)_cameraPos.y / m_nTileHeight;

		// Get stop point for rendering
		int stopX = camTileX + (int)ceil((800.0f / m_nTileWidth)) + 1;
		int stopY = camTileY + (int)ceil((600.0f / m_nTileHeight)) + 1;

		// Loop through ENTIRE tile array (to be replaced with culling)
		for (int x = camTileX; x < stopX; x++)
		{
			for (int y = camTileY; y < stopY; y++)
			{
				// Don't render out-of-bounds index
				if (x < 0 || y < 0 || x >= m_nWorldWidth || y >= m_nWorldHeight)
					continue;

				// Get tile information
				int tileID = m_vLayers[layer][x][y].GetTileID();

				// Check if tile is empty
				if (tileID == 0)
					continue;

				// Cell algorithm
				SGD::Rectangle sourceRect;
				sourceRect.left = (float)((tileID - 1) % m_nTilesetWidth) * m_nTileWidth;
				sourceRect.top = (float)((tileID - 1) / m_nTilesetWidth) * m_nTileHeight;
				sourceRect.right = sourceRect.left + m_nTileWidth;
				sourceRect.bottom = sourceRect.top + m_nTileHeight;

				// Figure out where to draw to the screen
				SGD::Point drawPos;
				drawPos.x = x * (float)m_nTileWidth - _cameraPos.x;
				drawPos.y = y * (float)m_nTileHeight - _cameraPos.y;

				// Draw the tile
				pGraphics->DrawTextureSection(m_hTilesetImage, drawPos, sourceRect);

				// FOR DEBUG PURPOSES ONLY!
				/*wostringstream id;
				id << tileID;
				pGraphics->DrawString(id.str().c_str(), SGD::Point(x * (float)m_nTileWidth, y * (float)m_nTileHeight));*/
			}
		}
	}
}

bool WorldManager::CheckCollision(IEntity* _object)
{
	// Get the object's collision rect
	SGD::Rectangle rect = _object->GetRect();

	// Set the tiles to check
	int top = (int)rect.top / m_nTileHeight;
	int left = (int)rect.left / m_nTileWidth;
	int bottom = (int)rect.bottom / m_nTileHeight + 1;
	int right = (int)rect.right / m_nTileWidth + 1;

	// FOR DEBUG PURPOSES ONLY!
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(SGD::Rectangle((float)left, (float)top, (float)right, (float)bottom), SGD::Color(100, 0, 0, 0));

	// Loop through tiles to check
	for (int x = left; x < right; x++)
	{
		for (int y = top; y < bottom; y++)
		{
			// Go through each layer (starting on top)
			for (int i = m_vLayers.size() - 1; i >= 0; i--)
			{
				// Check if collision
				if (m_vLayers[i][x][y].IsCollidable())
					return true;
			}
		}
	}

	return false;
}

int WorldManager::ColliderIDAtPosition(int _x, int _y) const
{
	// Start with the top layer

	return 0;
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

/**********************************************************/
// Helper Functions

void WorldManager::SendInitialTriggerMessage(const Tile& _tile) const
{
	// PLAYER_SPAWN
	if (_tile.GetTriggerInit() == "PLAYER_SPAWN")
	{
		CreatePlayerSpawnMessage* pMsg = new CreatePlayerSpawnMessage(_tile.GetX(), _tile.GetY());
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}