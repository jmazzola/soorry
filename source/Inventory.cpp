#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}
/**********************************************************/
// Mutators
void Inventory::SetWalls(unsigned int walls) 
{ 
	if (walls > m_unWallsMAX) 
		return; 
	else
		m_unWalls = walls;
}

void Inventory::SetWindows(unsigned int windows) 
{ 
	if (windows > m_unWindowsMAX) 
		return; 
	else
		m_unWindows = windows;
}

void Inventory::SetBearTraps(unsigned int beartraps) 
{ 
	if (beartraps > m_unBearTrapsMAX) 
		return;
	else
		m_unBearTraps = beartraps;
}

void Inventory::SetMines(unsigned int mines) 
{ 
	if (mines > m_unMinesMAX)
		return;
	else
		m_unMines = mines;
}

void Inventory::SetGrenades(unsigned int grenades) 
{ 
	if (grenades > m_unGrenadesMAX) 
		return;
	else
		m_unGrenades = grenades;
}

void Inventory::SetHealthPacks(unsigned int healthpack) 
{ 
	if (healthpack > m_unHealthPacksMAX) 
		return;
	else
		m_unHealthPacks = healthpack;
}

void Inventory::SetMachineGunTowers(unsigned int machineGunTowers) 
{ 
	if (machineGunTowers > m_unMachineGunTowersMAX) 
		return;
	else
		m_unMachineGunTowers = machineGunTowers;
}

void Inventory::SetMapleSyrupTowers(unsigned int mapleSyrupTowers) 
{ 
	if (mapleSyrupTowers > m_unMapleSyrupTowersMAX) 
		return;
	else
		m_unMapleSyrupTowers = mapleSyrupTowers;
}

void Inventory::SetHockeyStickTowers(unsigned int hockeyStickTowers) 
{ 
	if (hockeyStickTowers > m_unHockeyStickTowersMAX) 
		return;
	else
		m_unHockeyStickTowers = hockeyStickTowers;
}

void Inventory::SetLaserTowers(unsigned int laserTowers) 
{
	if (laserTowers > m_unLaserTowersMAX) 
		return;
	else
		m_unLaserTowers = laserTowers;
}

void Inventory::SetLavaTraps(unsigned int lavaTraps) 
{ 
	if (lavaTraps > m_unLavaTrapsMAX) 
		return;
	else
		m_unLavaTraps = lavaTraps;
}

void Inventory::SetSpikeTraps(unsigned int spikeTraps) 
{ 
	if (spikeTraps > m_unSpikeTrapsMAX) 
		return;
	else
		m_unSpikeTraps = spikeTraps;
}

void Inventory::SetDroneCount(unsigned int drones) 
{ 
	if (drones > m_unDronesMAX) 
		return;
	else
		m_unDrones = drones; 
}

