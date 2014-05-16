#pragma once

/**********************************************************/
// Forward Declarations
class Tile;

class Layer
{
public:

	Layer();
	~Layer();

protected:

	/**********************************************************/
	// Data Members
	Tile** m_pTiles;
};

