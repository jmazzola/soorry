/***************************************************************
|	File:		RZBN.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle saving and loading our
|				own custom binary file format : .rzbn as well as
|				reading and writing certain offsets
***************************************************************/

// -- Note. This is extremely JIT. Please make into a structure whenever you have free time. --
#include "RZBN.h"

#include "Player.h"
#include "Entity.h"
#include "Inventory.h"
#include "Weapon.h"
#include "WorldManager.h"
#include "GameplayState.h"
#include "ZombieFactory.h"
#include "Shop.h"

#include "Tower.h"
//#include "MachineGunTower.h"
//#include "MapleSyrupTower.h"
//#include "HockeyStickTower.h"
//#include "LaserTower.h"
#include "CreatePlaceableMessage.h"
#include "CreateTowerMessage.h"
#include "CreateTrapMessage.h"

#include <fstream>
using namespace std;

#define RZBN_VERSION 5
#define RZBN_MAGIC 0x4E425A52



// LoadRZBNFile
// [in] rzbnFilePath - filepath to load file in appdata
// [out] return 0 - File couldn't open
// [out] return 1 - Magic mismatch
// [out] return 2 - Version mismatch
// [out] return 0x1337 - Good.
// [out] return false - if at anytime, something fails.
int RZBN::LoadRZBNFile(string rzbnFilePath, bool isCheck)
{
	fstream file;
	file.open(rzbnFilePath, ios_base::binary | ios_base::in);

	// If we fail opening or anything at all
	if (!file.is_open() || !file.good())
		// return false
		return 0;

	// Check for magic number
	int magic;
	file.read((char*)&magic, sizeof(int));
	// If the magic doesnt match
	if (magic != RZBN_MAGIC)
		// quit
		return 1;

	// Check for version
	int version;
	file.read((char*)&version, sizeof(int));
	// If the version doesnt match
	if (version != RZBN_VERSION)
		// quit
		return 2;


	// Load Time
	file.read((char*)&m_Time, sizeof(time_t));

	// Load gamemode
	int gameMode;
	file.read((char*)&gameMode, sizeof(int));
	m_nGamemode = gameMode;

	// Load in wave number
	file.read((char*)&m_nWaveNum, sizeof(int));

	// Load in money
	int money;
	file.read((char*)&money, sizeof(int));
	m_nMoney = money;

	if (isCheck)
		return 0x1337;

	// -- Load in the map --

	// Load in the map's dimensions
	int mapWidth, mapHeight;
	file.read((char*)&mapWidth, sizeof(int));
	file.read((char*)&mapHeight, sizeof(int));

	// Load in the tile IDs
	for (int x = 0; x < mapWidth; x++)
		for (int y = 0; y < mapHeight; y++)
			file.read((char*)&m_nColliderIDs[x][y], sizeof(int));

	// Load spawn point
	file.read((char*)&m_fSpawnPointX, sizeof(float));
	file.read((char*)&m_fSpawnPointY, sizeof(float));

	// Load in walls
	file.read((char*)&m_nWalls, sizeof(unsigned int));

	// Load in windows
	file.read((char*)&m_nWindows, sizeof(unsigned int));

	// Load in bear traps
	file.read((char*)&m_nBeartraps, sizeof(unsigned int));

	// Load in mines
	file.read((char*)&m_nMines, sizeof(unsigned int));

	// Load in grenades
	file.read((char*)&m_nGrenades, sizeof(unsigned int));

	// Load in healthpacks
	file.read((char*)&m_nHealthPacks, sizeof(unsigned int));

	// Load in MG Towers
	file.read((char*)&m_nMGTowers, sizeof(unsigned int));

	// Load in Maple Syrup towers
	file.read((char*)&m_nMapleTowers, sizeof(unsigned int));

	// Load in Hockey Stick towers
	file.read((char*)&m_nHockeyTowers, sizeof(unsigned int));

	// Load laser towers
	file.read((char*)&m_nLaserTowers, sizeof(unsigned int));

	// Load lava traps
	file.read((char*)&m_nLavaTraps, sizeof(unsigned int));

	// Load spike traps
	file.read((char*)&m_nSpikeTraps, sizeof(unsigned int));

	// Load drones
	file.read((char*)&m_nDrones, sizeof(unsigned int));

	// -- Load Weapon Booleans --
	file.read((char*)&m_bHasAR, sizeof(bool));
	file.read((char*)&m_bHasSH, sizeof(bool));
	file.read((char*)&m_bHasRL, sizeof(bool));
	file.read((char*)&m_bHasHT, sizeof(bool));


	// -- Load Weapon Ammos --
	file.read((char*)&m_nARAmmo, sizeof(unsigned int));
	file.read((char*)&m_nSHAmmo, sizeof(unsigned int));
	file.read((char*)&m_nRLAmmo, sizeof(unsigned int));
	file.read((char*)&m_nHTAmmo, sizeof(unsigned int));

	// -- Load Weapon Stats --

	file.read((char*)&m_nArMaxAmmo, sizeof(int));
	file.read((char*)&m_nArDamage, sizeof(int));
	file.read((char*)&m_fArFirerate, sizeof(float));

	file.read((char*)&m_nShMaxAmmo, sizeof(int));
	file.read((char*)&m_nShDamage, sizeof(int));
	file.read((char*)&m_fShFirerate, sizeof(float));

	file.read((char*)&m_nRlMaxAmmo, sizeof(int));
	file.read((char*)&m_nRlDamage, sizeof(int));
	file.read((char*)&m_fRlFirerate, sizeof(float));

	file.read((char*)&m_nHtMaxAmmo, sizeof(int));
	file.read((char*)&m_nHtDamage, sizeof(int));
	file.read((char*)&m_fHtFirerate, sizeof(float));

	// -- Load Towers --
	int m_nTowerSize;
	file.read((char*)&m_nTowerSize, sizeof(int));

	for (int i = 0; i < m_nTowerSize; i++)
	{
		TowerInfo towerInfo;

		// Grab the information first
		file.read((char*)&towerInfo.m_nTowerType, sizeof(int));
		file.read((char*)&towerInfo.m_fTowerX, sizeof(float));
		file.read((char*)&towerInfo.m_fTowerY, sizeof(float));
		file.read((char*)&towerInfo.m_nUpgradeOne, sizeof(int));
		file.read((char*)&towerInfo.m_nUpgradeTwo, sizeof(int));

		towerInfos.push_back(towerInfo);
	}

	// -- Load Traps --
	int m_nTrapSize;
	file.read((char*)&m_nTrapSize, sizeof(int));

	for (int i = 0; i < m_nTrapSize; i++)
	{
		TrapInfo trapInfo;

		// Grab the information first
		file.read((char*)&trapInfo.m_nTrapType, sizeof(int));
		file.read((char*)&trapInfo.m_fTrapX, sizeof(float));
		file.read((char*)&trapInfo.m_fTrapY, sizeof(float));

		trapInfos.push_back(trapInfo);
	}

	return 0x1337;

}


// SaveRZBNFile
// [in] rzbnFilePath - filepath to save file in appdata
void RZBN::SaveRZBNFile(string rzbnFilePath)
{
	fstream file;
	file.open(rzbnFilePath, ios_base::binary | ios_base::out);

	// If we fail opening or anything at all
	if (!file.is_open() || !file.good())
		// quit
		return;

	// Write the magic number
	int magic = RZBN_MAGIC;
	file.write((char*)&magic, sizeof(int));

	// Write the version
	int vers = RZBN_VERSION;
	file.write((char*)&vers, sizeof(int));

	// Write time
	time_t t = time(nullptr);
	file.write((char*)&t, sizeof(time_t));

	// Set gamemode
	int gameMode = GameplayState::GetInstance()->GetGameMode();
	file.write((char*)&gameMode, sizeof(int));

	// Set wave number
	int waveNumber = m_pZomFactory->GetWave();
	file.write((char*)&waveNumber, sizeof(int));

	// Set money
	int money = m_pPlayer->GetScore();
	file.write((char*)&money, sizeof(int));


	// -- Write the map --

	// Save the map dimensions
	int mapWidth, mapHeight;
	mapWidth = m_pWorld->GetInstance()->GetWorldWidth();
	mapHeight = m_pWorld->GetInstance()->GetWorldHeight();

	file.write((char*)&mapWidth, sizeof(int));
	file.write((char*)&mapHeight, sizeof(int));

	// Save the IDs
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			int id = m_nColliderIDs[x][y] = m_pWorld->GetInstance()->GetColliderID(x, y);

			SGD::Rectangle rect;
			rect.left = (float)(x * 32 + 1);
			rect.top = (float)(y * 32 + 1);
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;

			if (m_pEntities->CheckCollision(rect, 3))
				m_nColliderIDs[x][y] = 0;

			file.write((char*)&m_nColliderIDs[x][y], sizeof(int));
		}
	}

	// Load spawn point
	float spawnPointX = m_pPlayer->GetPosition().x;
	float spawnPointY = m_pPlayer->GetPosition().y;
	file.write((char*)&spawnPointX, sizeof(float));
	file.write((char*)&spawnPointY, sizeof(float));



	Inventory* inv = m_pPlayer->GetInventory();

	// -- Write the inventory --
	m_nWalls = inv->GetWalls();
	file.write((char*)&m_nWalls, sizeof(int));

	int windows = inv->GetWindows();
	file.write((char*)&windows, sizeof(int));

	int beartraps = inv->GetBearTraps();
	file.write((char*)&beartraps, sizeof(int));

	int mines = inv->GetMines();
	file.write((char*)&mines, sizeof(int));

	int grenades = inv->GetGrenades();
	file.write((char*)&grenades, sizeof(int));

	int healthpacks = inv->GetHealthPacks();
	file.write((char*)&healthpacks, sizeof(int));

	int mgTowers = inv->GetMachineGunTowers();
	file.write((char*)&mgTowers, sizeof(int));

	int mapleTowers = inv->GetMapleSyrupTowers();
	file.write((char*)&mapleTowers, sizeof(int));

	int hockeyStick = inv->GetHockeyStickTowers();
	file.write((char*)&hockeyStick, sizeof(int));

	int laserTowers = inv->GetLaserTowers();
	file.write((char*)&laserTowers, sizeof(int));

	int lavaTraps = inv->GetLavaTraps();
	file.write((char*)&lavaTraps, sizeof(int));

	int spikeTraps = inv->GetSpikeTraps();
	file.write((char*)&spikeTraps, sizeof(int));

	int drones = inv->GetDroneCount();
	file.write((char*)&drones, sizeof(int));

	// -- Write weapon booleans --
	bool hasAR = m_pPlayer->HasAR();
	bool hasSH = m_pPlayer->HasShotty();
	bool hasRL = m_pPlayer->HasRocketLauncher();
	bool hasHT = m_pPlayer->HasHatTrick();

	file.write((char*)&hasAR, sizeof(bool));
	file.write((char*)&hasSH, sizeof(bool));
	file.write((char*)&hasRL, sizeof(bool));
	file.write((char*)&hasHT, sizeof(bool));

	// -- Write weapon stats --
	Weapon* weapons = m_pPlayer->GetWeapons();

	// Current Weapon Ammo
	int arCurAmmo, shCurAmmo, rlCurAmmo, htCurAmmo;
	(hasAR) ? arCurAmmo = weapons[0].GetCurrAmmo() : arCurAmmo = 0;
	(hasSH) ? shCurAmmo = weapons[1].GetCurrAmmo() : shCurAmmo = 0;
	(hasRL) ? rlCurAmmo = weapons[2].GetCurrAmmo() : rlCurAmmo = 0;
	(hasHT) ? htCurAmmo = weapons[3].GetCurrAmmo() : htCurAmmo = 0;

	file.write((char*)&arCurAmmo, sizeof(int));
	file.write((char*)&shCurAmmo, sizeof(int));
	file.write((char*)&rlCurAmmo, sizeof(int));
	file.write((char*)&htCurAmmo, sizeof(int));

	// Max Ammo, Damage and Firerate upgrades
	int arMaxAmmo;
	(hasAR) ? arMaxAmmo = weapons[0].GetMaxAmmo() : arMaxAmmo = 0;
	file.write((char*)&arMaxAmmo, sizeof(int));
	int arDamage;
	(hasAR) ? arDamage = m_pShop->GetARDamage() : arDamage = 0;
	file.write((char*)&arDamage, sizeof(int));
	float arFirerate;
	(hasAR) ? arFirerate = weapons[0].GetFireRate() : arFirerate = 0.0f;
	file.write((char*)&arFirerate, sizeof(float));

	// Holy shit my fingers hurt.

	int shMaxAmmo;
	(hasSH) ? shMaxAmmo = weapons[1].GetMaxAmmo() : shMaxAmmo = 0;
	file.write((char*)&shMaxAmmo, sizeof(int));
	int shDamage;
	(hasSH) ? shDamage = m_pShop->GetShotgunDamage() : shDamage = 20;
	file.write((char*)&shDamage, sizeof(int));
	float shFirerate;
	(hasSH) ? shFirerate = weapons[1].GetFireRate() : shFirerate = .5f;
	file.write((char*)&shFirerate, sizeof(float));

	int rlMaxAmmo;
	(hasRL) ? rlMaxAmmo = weapons[2].GetMaxAmmo() : rlMaxAmmo = 0;;
	file.write((char*)&rlMaxAmmo, sizeof(int));
	int rlDamage;
	(hasRL) ? rlDamage = m_pShop->GetRLDamage() : rlDamage = 150;
	file.write((char*)&rlDamage, sizeof(int));
	float rlFirerate;
	(hasRL) ? rlFirerate = weapons[2].GetFireRate() : rlFirerate = 2;
	file.write((char*)&rlFirerate, sizeof(float));

	int htMaxAmmo;
	(hasHT) ? htMaxAmmo = weapons[3].GetMaxAmmo() : htMaxAmmo = 0;
	file.write((char*)&htMaxAmmo, sizeof(int));
	int htDamage;
	(hasHT) ? htDamage = m_pShop->GetHTDamage() : htMaxAmmo = 75;
	file.write((char*)&htDamage, sizeof(int));
	float htFirerate;
	(hasHT) ? htFirerate = weapons[3].GetFireRate() : htFirerate = .75f;
	file.write((char*)&htFirerate, sizeof(float));


	GameplayState* gps = GameplayState::GetInstance();

	// -- Write the towers --
	if (GameplayState::GetInstance()->GetEntityManager()->GetSize() >= 4)
	{
		vector<IEntity*> towers = gps->GetEntityManager()->GetBucket(3);

		// Write out the towers size
		int towersSize = towers.size();
		file.write((char*)&towersSize, sizeof(int));

		// Save out type, x, y, upgrade one, upgradeTwo
		for (size_t i = 0; i < towers.size(); i++)
		{
			int towerType = dynamic_cast<Tower*>(towers[i])->GetType();
			file.write((char*)&towerType, sizeof(int));
			float towerX = dynamic_cast<Tower*>(towers[i])->GetPosition().x;
			file.write((char*)&towerX, sizeof(float));
			float towerY = dynamic_cast<Tower*>(towers[i])->GetPosition().y;
			file.write((char*)&towerY, sizeof(float));
			int upgradeOne = dynamic_cast<Tower*>(towers[i])->GetUpgradeOne();
			file.write((char*)&upgradeOne, sizeof(int));
			int upgradeTwo = dynamic_cast<Tower*>(towers[i])->GetUpgradeTwo();
			file.write((char*)&upgradeTwo, sizeof(int));
		}
	}

	// -- Write the traps --

	vector<IEntity*> traps = gps->GetEntityManager()->GetBucket(0);
	int trapsSize = traps.size();
	file.write((char*)&trapsSize, sizeof(int));

	// Save the traps
	for (int i = 0; i < trapsSize; i++)
	{
		int trapType = dynamic_cast<Entity*>(traps[i])->GetType();
		file.write((char*)&trapType, sizeof(int));
		float trapX = dynamic_cast<Entity*>(traps[i])->GetPosition().x;
		file.write((char*)&trapX, sizeof(float));
		float trapY = dynamic_cast<Entity*>(traps[i])->GetPosition().y;
		file.write((char*)&trapY, sizeof(float));
	}
}


void RZBN::MakeThatShit()
{
	m_nColliderIDs = new int*[100];
	for (int i = 0; i < 100; i++)
		m_nColliderIDs[i] = new int[100];
}


void RZBN::DeleteThatShit()
{
	for (int i = 0; i < 100; i++)
		delete[] m_nColliderIDs[i];

	delete[] m_nColliderIDs;
}