#pragma once

#include <vector>
#include <string>
using namespace std;

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"

#include "Layer.h"

#define CAMERA_IMPLEMENTED 0

/**********************************************************/
// Forward Declarations
class IEntity;

class WorldManager
{
public:

	static WorldManager* GetInstance();

	/**********************************************************/
	// Interface
	bool LoadWorld(string fileName);
	void UnloadWorld();
	void Render(SGD::Point cameraPosition);
	bool CheckCollision(IEntity* object);
	int ColliderIDAtPosition(int x, int y) const;

	/**********************************************************/
	// Accessors
	int GetWorldWidth() const;
	int GetWorldHeight() const;
	int GetTileWidth() const;
	int GetTileHeight() const;
	int GetTilesetWidth() const;
	SGD::HTexture GetTilesetImage() const;

	/**********************************************************/
	// Mutators
	void SetWorldWidth(int worldWidth);
	void SetWorldHeight(int worldHeight);
	void SetTileWidth(int tileWidth);
	void SetTileHeight(int tileHeight);
	void SetTilesetWidth(int tilesetWidth);
	void SetTilesetImage(SGD::HTexture tilesetImage);

protected:

	/**********************************************************/
	// Data Members
	int m_nWorldWidth;
	int m_nWorldHeight;
	int m_nTileWidth;
	int m_nTileHeight;
	int m_nTilesetWidth;
	vector<Layer> m_vLayers;
	SGD::HTexture m_hTilesetImage;

#if !CAMERA_IMPLEMENTED

	SGD::Point cameraPos = { 0, 0 };

#endif

private:

	/**********************************************************/
	// Helper Functions
	void SendInitialTriggerMessage(const Tile& tile) const;

	/**********************************************************/
	// Singleton Business
	WorldManager() = default;
	virtual ~WorldManager() = default;
	WorldManager(const WorldManager&) = delete;
	WorldManager& operator= (const WorldManager&) = delete;
};

