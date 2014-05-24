#include "WorldManager.h"

#include "Tile.h"

#include "../TinyXML/tinyxml.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "CreatePlayerSpawnMessage.h"

#include "IEntity.h"

#include <sstream>
#include <cmath>
using namespace std;

#define CLOSED 0


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

	// Create wall/window layer
	Layer layer(m_nWorldWidth, m_nWorldHeight);

	// Loop through the tiles
	for (int x = 0; x < m_nWorldWidth; x++)
	{
		for (int y = 0; y <m_nWorldHeight; y++)
		{
			Tile tile;

			tile.SetX(x);
			tile.SetY(y);
			tile.SetColliderID(0);
			tile.SetCollidable(false);
			tile.SetTriggerCollision("");
			tile.SetTriggerInit("");
			tile.SetTileID(0);

			//SetColliderID(x, y, WALL);
		}
	}

	m_vLayers.push_back(layer);

	// Are there any layers?
	if (m_vLayers.size() > 0)
	{
		GenerateSolidsChart();
		return true;
	}
	else
		return false;
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

	// Unload solids chart
	for (int x = 0; x < m_nWorldWidth; x++)
		delete[] m_bSolidsChart[x];
	delete[] m_bSolidsChart;
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

		// Loop through the viewport
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

#if !CLOSED

	// Bound within the world
	if (top < 0) top = 0;
	if (left < 0) left = 0;
	if (bottom < 0) bottom = 0;
	if (right < 0) right = 0;
	if (top > m_nWorldHeight - 1) top = m_nWorldHeight;
	if (bottom > m_nWorldHeight - 1) bottom = m_nWorldHeight;
	if (left > m_nWorldWidth - 1) left = m_nWorldWidth;
	if (right > m_nWorldWidth - 1) right = m_nWorldWidth;

#endif

	// Loop through tiles to check
	for (int x = left; x < right; x++)
	{
		for (int y = top; y < bottom; y++)
		{
			// Go through each layer (starting on top)
			for (int i = m_vLayers.size() - 1; i >= 0; i--)
			{
				// Check for collision triggers
				if (m_vLayers[i][x][y].GetTriggerCollision() != "")
				{
					SGD::Event* pEvent = new SGD::Event(m_vLayers[i][x][y].GetTriggerCollision().c_str(), nullptr, this);
					pEvent->QueueEvent();
				}

				// Check if collision
				if (m_vLayers[i][x][y].IsCollidable())
					return true;
			}
		}
	}

	return false;
}

int WorldManager::CheckCollisionID(IEntity* _object)
{
	// Get the object's collision rect
	SGD::Rectangle rect = _object->GetRect();

	// Set the tiles to check
	int top = (int)rect.top / m_nTileHeight;
	int left = (int)rect.left / m_nTileWidth;
	int bottom = (int)rect.bottom / m_nTileHeight + 1;
	int right = (int)rect.right / m_nTileWidth + 1;

#if !CLOSED

	// Bound within the world
	if (top < 0) top = 0;
	if (left < 0) left = 0;
	if (bottom < 0) bottom = 0;
	if (right < 0) right = 0;
	if (top > m_nWorldHeight - 1) top = m_nWorldHeight;
	if (bottom > m_nWorldHeight - 1) bottom = m_nWorldHeight;
	if (left > m_nWorldWidth - 1) left = m_nWorldWidth;
	if (right > m_nWorldWidth - 1) right = m_nWorldWidth;

#endif

	// Loop through tiles to check
	for (int x = left; x < right; x++)
	{
		for (int y = top; y < bottom; y++)
		{
			// Go through each layer (starting on top)
			for (int i = m_vLayers.size() - 1; i >= 0; i--)
			{
				// Check for collision ID
				if (m_vLayers[i][x][y].GetColliderID() != 0)
					return m_vLayers[i][x][y].GetColliderID();
			}
		}
	}

	return 0;
}

int WorldManager::ColliderIDAtPosition(int _x, int _y) const
{
	// Start with the top layer

	return 0;
}

bool WorldManager::IsSolidAtPosition(int _x, int _y) const
{
	return m_bSolidsChart[_x][_y];
}

void WorldManager::SetColliderID(int _x, int _y, int _id)
{
	if (_id == EMPTY)
	{
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetColliderID(EMPTY);
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetTileID(0);
	}

	else if (_id == WINDOW)
	{
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetColliderID(WINDOW);
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetTileID(3);
	}

	else if (_id == WALL)
	{
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetColliderID(WALL);
		m_vLayers[m_vLayers.size() - 1][_x][_y].SetTileID(2);
	}
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

void WorldManager::GenerateSolidsChart()
{
	// Allocate new memory for the solids chart
	m_bSolidsChart = new bool*[m_nWorldWidth];
	for (int x = 0; x < m_nWorldWidth; x++)
		m_bSolidsChart[x] = new bool[m_nWorldHeight];

	// Loop through each of the layers
	for (unsigned int i = 0; i < m_vLayers.size(); i++)
	{
		// Loop through each of the tiles
		for (int x = 0; x < m_nWorldWidth; x++)
		{
			for (int y = 0; y < m_nWorldHeight; y++)
			{
				// Check if collidable
				if (m_vLayers[i][x][y].IsCollidable())
				{
					m_bSolidsChart[x][y] = true;
				}
				else
				{
					m_bSolidsChart[x][y] = false;
				}
			}
		}
	}
}