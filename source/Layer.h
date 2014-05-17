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

protected:

	/**********************************************************/
	// Data Members
	Tile** m_pTiles;
};

