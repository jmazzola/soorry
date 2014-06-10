/***************************************************************
|	File:		RZBN.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle saving and loading our
|				own custom binary file format : .rzbn
***************************************************************/

#include "RZBN.h"

#include "Player.h"
#include "Inventory.h"
#include "Weapon.h"
#include "WorldManager.h"

#include <fstream>
using namespace std;

#define RZBN_VERSION 1

// LoadRZBNFile
// [in] rzbnFilePath - filepath to load file in appdata
// [out] return true - if the whole file is loaded successfully
// [out] return false - if at anytime, something fails.
bool RZBN::LoadRZBNFile(string rzbnFilePath)
{
	fstream file;
	file.open(rzbnFilePath);

	// If we fail opening or anything at all
	if (!file.is_open() || !file.good())
		// return false
		return false;

	// Check for magic number
	int magic;
	file.read((char*)&magic, sizeof(int));
	// If the magic doesnt match
	if (magic != 0x525A424E)
		// quit
		return false;

	// Check for version
	int version;
	file.read((char*)&version, sizeof(int));
	// If the version doesnt match
	if (version != RZBN_VERSION)
		// quit
		return false;

	// -- Load in the map --

	// Load in the map's dimensions
	int mapWidth, mapHeight;
	file.read((char*)&mapWidth, sizeof(int));
	file.read((char*)&mapHeight, sizeof(int));

	// Load in the tile IDs
	int id[100][100];
	for (int w = 0; w < mapWidth; w++)
	{
		for (int h = 0; h < mapHeight; h++)
		{
			file.read((char*)&id[w][h], sizeof(int));
			WorldManager::GetInstance()->SetColliderID(w, h, id[w][h]);
		}
	}

	// -- Load in inventory --

	Inventory* inv = m_pPlayer->GetInventory();

	// Load in walls
	unsigned int walls;
	file.read((char*)&walls, sizeof(unsigned int));

	// Load in windows
	unsigned int windows;
	file.read((char*)&windows, sizeof(unsigned int));

	// Load in bear traps
	unsigned int beartraps;
	file.read((char*)&beartraps, sizeof(unsigned int));

	// Load in mines
	unsigned int mines;
	file.read((char*)&mines, sizeof(unsigned int));

	// Load in grenades
	unsigned int grenades;
	file.read((char*)&grenades, sizeof(unsigned int));

	// Load in healthpacks
	unsigned int healthpacks;
	file.read((char*)&healthpacks, sizeof(unsigned int));

	// Load in MG Towers
	unsigned int mgTowers;
	file.read((char*)&mgTowers, sizeof(unsigned int));

	// Load in Maple Syrup towers
	unsigned int mapleSyrup;
	file.read((char*)&mapleSyrup, sizeof(unsigned int));

	// Load in Hockey Stick towers
	unsigned int hockeyStick;
	file.read((char*)&hockeyStick, sizeof(unsigned int));

	// Load laser towers
	unsigned int laserTowers;
	file.read((char*)&laserTowers, sizeof(unsigned int));

	// Load lava traps
	unsigned int lavaTraps;
	file.read((char*)&lavaTraps, sizeof(unsigned int));

	// Load spike traps
	unsigned int spikeTraps;
	file.read((char*)&spikeTraps, sizeof(unsigned int));

	// Set inventory
	inv->SetWalls(walls);
	inv->SetWindows(windows);
	inv->SetBearTraps(beartraps);
	inv->SetMines(mines);
	inv->SetGrenades(grenades);
	inv->SetMachineGunTowers(mgTowers);
	inv->SetMapleSyrupTowers(mapleSyrup);
	inv->SetHockeyStickTowers(hockeyStick);
	inv->SetLaserTowers(laserTowers);
	inv->SetLavaTraps(lavaTraps);
	inv->SetSpikeTraps(spikeTraps);

	// -- Load weapons --

	Weapon* weaps = m_pPlayer->GetWeapons();

}


// SaveRZBNFile
// [in] rzbnFilePath - filepath to save file in appdata
void RZBN::SaveRZBNFile(string rzbnFilePath)
{
	fstream file;
	file.open(rzbnFilePath);

	// If we fail opening or anything at all
	if (!file.is_open() || !file.good())
		// quit
		return;

	
}
