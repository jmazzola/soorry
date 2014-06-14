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
#include <vector>
using namespace std;

class Player;
class ZombieFactory;
class Shop;
class EntityManager;
class WorldManager;

struct TowerInfo
{
	int m_nTowerType;
	float m_fTowerX, m_fTowerY;
	int m_nUpgradeOne, m_nUpgradeTwo;
};

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
	EntityManager* GetEntityManager() { return m_pEntities; }
	WorldManager* GetWorldManager() { return m_pWorld; }

	int GetMoney() const { return m_nMoney; }
	int GetGameMode() const { return m_nGamemode; }

	// Mutators
	void SetPlayer(Player* player) { m_pPlayer = player; }
	void SetZombieFactory(ZombieFactory* zom) { m_pZomFactory = zom; }
	void SetShop(Shop* shop) { m_pShop = shop; }
	void SetEntityManager(EntityManager* em)  { m_pEntities = em; }
	void SetWorldManager(WorldManager* wm) { m_pWorld = wm; }

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

	// Entity Manager
	EntityManager* m_pEntities;

	// World Manager
	WorldManager* m_pWorld;

public:

	int m_nMoney;
	int m_nGamemode;
	int m_nColliderIDs[100][100];
	float m_fSpawnPointX;
	float m_fSpawnPointY;
	int m_nWaveNum;
	
	int m_nWalls, m_nWindows, m_nBeartraps, m_nMines, m_nGrenades,
		m_nHealthPacks, m_nMGTowers, m_nMapleTowers, m_nHockeyTowers,
		m_nLaserTowers, m_nLavaTraps, m_nSpikeTraps, m_nDrones;

	bool m_bHasAR, m_bHasSH, m_bHasRL, m_bHasHT;

	int m_nARAmmo, m_nSHAmmo, m_nRLAmmo, m_nHTAmmo;

	int m_nArMaxAmmo, m_nArDamage; float m_fArFirerate;
	int m_nShMaxAmmo, m_nShDamage; float m_fShFirerate;
	int m_nRlMaxAmmo, m_nRlDamage; float m_fRlFirerate;
	int m_nHtMaxAmmo, m_nHtDamage; float m_fHtFirerate;

	vector<TowerInfo> towerInfos;
};

