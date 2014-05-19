#include "Tile.h"


Tile::Tile()
{
	// Initialize data members
	m_nTileID = 0;
	m_nColliderID = 0;
	m_bIsCollidable = false;
	m_strTriggerInit.clear();
	m_strTriggerCollision.clear();
}

Tile::~Tile()
{
}

/**********************************************************/
// Accessors

int Tile::GetTileID() const
{
	return m_nTileID;
}

int Tile::GetColliderID() const
{
	return m_nColliderID;
}

bool Tile::IsCollidable() const
{
	return m_bIsCollidable;
}

string Tile::GetTriggerInit() const
{
	return m_strTriggerInit;
}

string Tile::GetTriggerCollision() const
{
	return m_strTriggerCollision;
}

/**********************************************************/
// Mutators

void Tile::SetTileID(int _tileID)
{
	m_nTileID = _tileID;
}

void Tile::SetColliderID(int _colliderID)
{
	m_nColliderID = _colliderID;
}

void Tile::SetCollidable(bool _isCollidable)
{
	m_bIsCollidable = _isCollidable;
}

void Tile::SetTriggerInit(string _triggerInit)
{
	m_strTriggerInit = _triggerInit;
}

void Tile::SetTriggerCollision(string _triggerCollision)
{
	m_strTriggerCollision = _triggerCollision;
}