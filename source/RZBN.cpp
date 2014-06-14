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

#define RZBN_VERSION 1
#define RZBN_MAGIC 0x4E425A52

// LoadRZBNFile
// [in] rzbnFilePath - filepath to load file in appdata
// [out] return true - if the whole file is loaded successfully
// [out] return false - if at anytime, something fails.
bool RZBN::LoadRZBNFile(string rzbnFilePath)
{
	fstream file;
	file.open(rzbnFilePath, ios_base::binary | ios_base::in);

	// If we fail opening or anything at all
	if (!file.is_open() || !file.good())
		// return false
		return false;

	// Check for magic number
	int magic;
	file.read((char*)&magic, sizeof(int));
	// If the magic doesnt match
	if (magic != RZBN_MAGIC)
		// quit
		return false;

	// Check for version
	int version;
	file.read((char*)&version, sizeof(int));
	// If the version doesnt match
	if (version != RZBN_VERSION)
		// quit
		return false;

	// Load gamemode
	int gameMode;
	file.read((char*)&gameMode, sizeof(int));
	m_nGamemode = gameMode;

	// -- Load in the map --

	// Load in the map's dimensions
	int mapWidth, mapHeight;
	file.read((char*)&mapWidth, sizeof(int));
	file.read((char*)&mapHeight, sizeof(int));

	// Load in the tile IDs
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			file.read((char*)&m_nColliderIDs[x][y], sizeof(int));
			//m_pWorld->GetInstance()->SetColliderID(x, y, id[x][y]);
		}
	}

	// Load spawn point
	file.read((char*)&m_fSpawnPointX, sizeof(float));
	file.read((char*)&m_fSpawnPointY, sizeof(float));
	//m_pPlayer->SetPosition({ spawnPointX, spawnPointY });


	// Load in wave number
	file.read((char*)&m_nWaveNum, sizeof(int));
	//m_pZomFactory->SetWave(waveNumber);

	// Load in money
	int money;
	file.read((char*)&money, sizeof(int));
	m_nMoney = money;
	//m_pPlayer->SetScore(money);

	// -- Load in inventory --

	//Inventory* inv = m_pPlayer->GetInventory();

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

	//// Set inventory
	//inv->SetWalls(walls);
	//inv->SetWindows(windows);
	//inv->SetBearTraps(beartraps);
	//inv->SetMines(mines);
	//inv->SetGrenades(grenades);
	//inv->SetMachineGunTowers(mgTowers);
	//inv->SetMapleSyrupTowers(mapleSyrup);
	//inv->SetHockeyStickTowers(hockeyStick);
	//inv->SetLaserTowers(laserTowers);
	//inv->SetLavaTraps(lavaTraps);
	//inv->SetSpikeTraps(spikeTraps);
	//inv->SetDroneCount(droneCount);

	//Weapon* weapons = m_pPlayer->GetWeapons();

	// -- Load Weapon Booleans --
	file.read((char*)&m_bHasAR, sizeof(bool));
	file.read((char*)&m_bHasSH, sizeof(bool));
	file.read((char*)&m_bHasRL, sizeof(bool));
	file.read((char*)&m_bHasHT, sizeof(bool));

	/*m_pPlayer->SetAR(hasAR);
	m_pPlayer->SetShotgun(hasShotty);
	m_pPlayer->SetRocketLauncher(hasRL);
	m_pPlayer->SetHatTrick(hasHT);*/

	// -- Load Weapon Ammos --
	file.read((char*)&m_nARAmmo, sizeof(unsigned int));
	file.read((char*)&m_nSHAmmo, sizeof(unsigned int));
	file.read((char*)&m_nRLAmmo, sizeof(unsigned int));
	file.read((char*)&m_nHTAmmo, sizeof(unsigned int));

	/*weapons[0].SetCurrAmmo(arAmmo);
	weapons[1].SetCurrAmmo(shAmmo);
	weapons[2].SetCurrAmmo(rlAmmo);
	weapons[3].SetCurrAmmo(htAmmo);
*/
	// -- Load Weapon Stats --

	file.read((char*)&m_nArMaxAmmo, sizeof(int));
	file.read((char*)&m_nArDamage, sizeof(int));
	file.read((char*)&m_fArFirerate, sizeof(float));

	/*weapons[0].SetMaxAmmo(arMaxAmmo);
	weapons[0].SetFireRate(arFirerate);
	m_pShop->SetARDamage(arDamage);*/

	file.read((char*)&m_nShMaxAmmo, sizeof(int));
	file.read((char*)&m_nShDamage, sizeof(int));
	file.read((char*)&m_fShFirerate, sizeof(float));

	/*weapons[1].SetMaxAmmo(shMaxAmmo);
	weapons[1].SetFireRate(shFirerate);
	m_pShop->SetShotgunDamage(shDamage);*/

	file.read((char*)&m_nRlMaxAmmo, sizeof(int));
	file.read((char*)&m_nRlDamage, sizeof(int));
	file.read((char*)&m_fRlFirerate, sizeof(float));

	/*weapons[2].SetMaxAmmo(rlMaxAmmo);
	weapons[2].SetFireRate(rlFirerate);
	m_pShop->SetRLDamage(rlDamage);*/

	file.read((char*)&m_nHtMaxAmmo, sizeof(int));
	file.read((char*)&m_nHtDamage, sizeof(int));
	file.read((char*)&m_fHtFirerate, sizeof(float));

	/*weapons[2].SetMaxAmmo(htMaxAmmo);
	weapons[2].SetFireRate(htFirerate);
	m_pShop->SetHTDamage(htDamage);*/

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

		// Create the towers
		/*switch (m_nTowerType)
		{
		case Entity::ENT_TOWER_MACHINE_GUN:
		{
			CreateTowerMessage* pmsg =
				new CreateTowerMessage((int)(m_fTowerX), (int)(m_fTowerY),
				CreateTowerMessage::TOWER_MACHINE_GUN);
			pmsg->SendMessageNow();
			delete pmsg;
			pmsg = nullptr;
		}
			break;

		case Entity::ENT_TOWER_MAPLE_SYRUP:
		{
			CreateTowerMessage* pmsg =
				new CreateTowerMessage((int)(m_fTowerX), (int)(m_fTowerY),
				CreateTowerMessage::TOWER_MAPLE_SYRUP);
			pmsg->SendMessageNow();
			delete pmsg;
			pmsg = nullptr;

		}
			break;

		case Entity::ENT_TOWER_HOCKEY_STICK:
		{
			CreateTowerMessage* pmsg =
				new CreateTowerMessage((int)(m_fTowerX), (int)(m_fTowerY),
				CreateTowerMessage::TOWER_HOCKEY_STICK);
			pmsg->SendMessageNow();
			delete pmsg;
			pmsg = nullptr;

		}
			break;

		case Entity::ENT_TOWER_LASER:
		{
			CreateTowerMessage* pmsg =
				new CreateTowerMessage((int)(m_fTowerX), (int)(m_fTowerY),
				CreateTowerMessage::TOWER_LASER);
			pmsg->SendMessageNow();
			delete pmsg;
			pmsg = nullptr;

		}
			break;

		}*/

		// Set upgrades 
		/*GameplayState* gps = GameplayState::GetInstance();
		vector<IEntity*> towers = gps->GetEntityManager()->GetBucket(3);
		dynamic_cast<Tower*>(towers[i])->SetUpgradeOne(m_nUpgradeOne);
		dynamic_cast<Tower*>(towers[i])->SetUpgradeTwo(m_nUpgradeTwo);*/

	}

	return false;

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

	// Set gamemode
	int gameMode = GameplayState::GetInstance()->GetGameMode();
	file.write((char*)&gameMode, sizeof(int));


	// -- Write the map --

	// Save the map dimensions
	int mapWidth, mapHeight;
	mapWidth = m_pWorld->GetInstance()->GetWorldWidth();
	mapHeight = m_pWorld->GetInstance()->GetWorldHeight();

	file.write((char*)&mapWidth, sizeof(int));
	file.write((char*)&mapHeight, sizeof(int));

	// Save the IDs
	int id[100][100];
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			id[x][y] = m_pWorld->GetInstance()->GetColliderID(x, y);
			file.write((char*)&id[x][y], sizeof(int));
		}
	}

	// Load spawn point
	float spawnPointX = m_pPlayer->GetPosition().x;
	float spawnPointY = m_pPlayer->GetPosition().y;
	file.write((char*)&spawnPointX, sizeof(float));
	file.write((char*)&spawnPointY, sizeof(float));

	// Set wave number
	int waveNumber = m_pZomFactory->GetWave();
	file.write((char*)&waveNumber, sizeof(int));

	// Set money
	int money = m_pPlayer->GetScore();
	file.write((char*)&money, sizeof(int));

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
	bool hasHT = m_pPlayer->HasRocketLauncher();

	file.write((char*)&hasAR, sizeof(bool));
	file.write((char*)&hasSH, sizeof(bool));
	file.write((char*)&hasRL, sizeof(bool));
	file.write((char*)&hasHT, sizeof(bool));

	// -- Write weapon stats --
	Weapon* weapons = m_pPlayer->GetWeapons();
	// Current Weapon Ammo
	int arCurAmmo = weapons[0].GetCurrAmmo();
	int shCurAmmo = weapons[1].GetCurrAmmo();
	int rlCurAmmo = weapons[2].GetCurrAmmo();
	int htCurAmmo = weapons[3].GetCurrAmmo();
	file.write((char*)&arCurAmmo, sizeof(int));
	file.write((char*)&shCurAmmo, sizeof(int));
	file.write((char*)&rlCurAmmo, sizeof(int));
	file.write((char*)&htCurAmmo, sizeof(int));

	// Max Ammo, Damage and Firerate upgrades
	int arMaxAmmo = weapons[0].GetMaxAmmo();
	file.write((char*)&arMaxAmmo, sizeof(int));
	int arDamage = m_pShop->GetARDamage();
	file.write((char*)&arDamage, sizeof(int));
	float arFirerate = weapons[0].GetFireRate();
	file.write((char*)&arFirerate, sizeof(float));

	// Holy shit my fingers hurt.

	int shMaxAmmo = weapons[1].GetMaxAmmo();
	file.write((char*)&shMaxAmmo, sizeof(int));
	int shDamage = m_pShop->GetShotgunDamage();
	file.write((char*)&shDamage, sizeof(int));
	float shFirerate = weapons[1].GetFireRate();
	file.write((char*)&shFirerate, sizeof(float));

	int rlMaxAmmo = weapons[2].GetMaxAmmo();
	file.write((char*)&rlMaxAmmo, sizeof(int));
	int rlDamage = m_pShop->GetRLDamage();
	file.write((char*)&rlDamage, sizeof(int));
	float rlFirerate = weapons[2].GetFireRate();
	file.write((char*)&rlFirerate, sizeof(float));

	int htMaxAmmo = weapons[3].GetMaxAmmo();
	file.write((char*)&htMaxAmmo, sizeof(int));
	int htDamage = m_pShop->GetHTDamage();
	file.write((char*)&htDamage, sizeof(int));
	float htFirerate = weapons[3].GetFireRate();
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
	for (size_t i = 0; i < traps.size(); i++)
	{

	}


}