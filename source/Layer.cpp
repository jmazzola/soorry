#include "Layer.h"

#include "Tile.h"


Layer::Layer(unsigned int _width, unsigned int _height)
{
	// Allocate new memory for the tiles
	m_pTiles = new Tile*[_width];
	for (unsigned int x = 0; x < _width; x++)
		m_pTiles[x] = new Tile[_height];
}


Layer::~Layer()
{
	// Deallocate tile memory
}


Tile* Layer::operator[](unsigned int _index)
{
	return m_pTiles[_index];
}