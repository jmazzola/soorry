#include "Layer.h"

#include "Tile.h"


Layer::Layer()
{
}


Layer::~Layer()
{
}


Tile* Layer::operator[](unsigned int _index)
{
	return m_pTiles[_index];
}