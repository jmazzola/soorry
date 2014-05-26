#pragma once

#include <string>
using namespace std;

#define EMPTY 0
#define WINDOW 1
#define WALL 2

class Tile
{
public:

	Tile();
	~Tile();

	/**********************************************************/
	// Accessors
	int GetX() const;
	int GetY() const;
	int GetTileID() const;
	int GetColliderID() const;
	bool IsCollidable() const;
	const string& GetTriggerInit() const;
	const string& GetTriggerCollision() const;

	/**********************************************************/
	// Mutators
	void SetX(int x);
	void SetY(int y);
	void SetTileID(int tileID);
	void SetColliderID(int colliderID);
	void SetCollidable(bool isCollidable);
	void SetTriggerInit(const string& triggerInit);
	void SetTriggerCollision(const string& triggerCollision);

protected:

	/**********************************************************/
	// Data Members
	int m_nX;
	int m_nY;
	int m_nTileID;
	int m_nColliderID;
	bool m_bIsCollidable;
	string m_strTriggerInit;
	string m_strTriggerCollision;
};

