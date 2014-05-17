#pragma once

/**********************************************************/
// Forward Declarations
class Tile;

class Layer
{
public:

	Layer();
	~Layer();

	Tile* operator[](unsigned int index);

protected:

	/**********************************************************/
	// Data Members
	Tile** m_pTiles;
};

