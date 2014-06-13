/***************************************************************
|	File:		RZBN.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle saving and loading our
|				own custom binary file format : .rzbn as well as
|				reading and writing certain offsets
***************************************************************/
#pragma once

#include <string>
using namespace std;

class Player;
class ZombieFactory;
class Shop;

class RZBN
{

public:
	// Ctor and dtor
	RZBN() = default;
	virtual ~RZBN() = default;

	// Accessors
	Player* GetPlayer() const { return m_pPlayer; }
	ZombieFactory* GetZombieFactory() const { return m_pZomFactory; }
	Shop* GetShop() const { return m_pShop; }

	// Mutators
	void SetPlayer(Player* player) { m_pPlayer = player; }
	void SetZombieFactory(ZombieFactory* zom) { m_pZomFactory = zom; }
	void SetShop(Shop* shop) { m_pShop = shop; }

	// Load Savefile
	bool LoadRZBNFile(string rzbnFilePath);

	// Save savefile
	void SaveRZBNFile(string rzbnFilePath);

private:

	// Player object
	Player* m_pPlayer;

	// ZombieFactory object
	ZombieFactory* m_pZomFactory;

	// Shop object
	Shop* m_pShop;
};

