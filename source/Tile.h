#pragma once

#include <string>
using namespace std;

class Tile
{
public:

	Tile();
	~Tile();

	/**********************************************************/
	// Accessors
	int GetTileID() const;
	int GetColliderID() const;
	bool IsCollidable() const;
	string GetTriggerInit() const;
	string GetTriggerCollision() const;

	/**********************************************************/
	// Mutators
	void SetTileID(int tileID);
	void SetColliderID(int colliderID);
	void SetCollidable(bool isCollidable);
	void SetTriggerInit(string triggerInit);
	void SetTriggerCollision(string triggerCollision);

protected:

	/**********************************************************/
	// Data Members
	int m_nTileID;
	int m_nColliderID;
	bool m_bIsCollidable;
	string m_strTriggerInit;
	string m_strTriggerCollision;
};

