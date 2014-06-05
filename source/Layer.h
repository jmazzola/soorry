#pragma once

/**********************************************************/
// Forward Declarations
class Tile;

class Layer
{
public:

	Layer(unsigned int width, unsigned int height);
	~Layer();

	Tile* operator[](unsigned int index);

	void UnloadTiles();

protected:

	/**********************************************************/
	// Data Members
	int m_nWidth;
	int m_nHeight;
	Tile** m_pTiles;
};

