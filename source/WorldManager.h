#pragma once

#include <vector>
using namespace std;

#include "../SGD Wrappers/SGD_Handle.h"

#include "Layer.h"

class WorldManager
{
public:

	static WorldManager* GetInstance();

	WorldManager();
	~WorldManager();

protected:

	/**********************************************************/
	// Data Members
	int m_nWorldWidth;
	int m_nWorldHeight;
	int m_nTileWidth;
	int m_nTileHeight;
	vector<Layer> m_vLayers;
	SGD::HTexture m_hTilesetImage;

private:

	WorldManager() = default;
	virtual ~WorldManager() = default;

	WorldManager(const WorldManager&) = delete;
	WorldManager& operator= (const WorldManager&) = delete;
};

