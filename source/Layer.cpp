#include "Layer.h"

#include "Tile.h"


Layer::Layer(unsigned int _width, unsigned int _height)
{
	// Allocate new memory for the tiles
	m_pTiles = new Tile*[_width];
	for (unsigned int x = 0; x < _width; x++)
		m_pTiles[x] = new Tile[_height];

	// Initialize data members
	m_nWidth = _width;
	m_nHeight = _height;
}


Layer::~Layer()
{
}


Tile* Layer::operator[](unsigned int _index)
{
	return m_pTiles[_index];
}


void Layer::UnloadTiles()
{
	// Deallocate tile memory
	for (int x = 0; x < m_nWidth; x++)
		delete[] m_pTiles[x];
	delete[] m_pTiles;
}