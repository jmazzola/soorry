#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}
/**********************************************************/
// Mutators
bool Inventory::SetWalls(unsigned int walls)
{ 
	if (walls > m_unWallsMAX) 
		return false; 
	else
	{
		m_unWalls = walls;
		return true;
	}
}

bool Inventory::SetWindows(unsigned int windows)
{ 
	if (windows > m_unWindowsMAX) 
		return false; 
	else
	{
		m_unWindows = windows;
		return true;
	}
}

bool Inventory::SetBearTraps(unsigned int beartraps)
{ 
	if (beartraps > m_unBearTrapsMAX) 
		return false;
	else
	{
		m_unBearTraps = beartraps;
		return true;
	}
}

bool Inventory::SetMines(unsigned int mines)
{ 
	if (mines > m_unMinesMAX)
		return false;
	else
	{
		m_unMines = mines;
		return true;
	}
}

bool Inventory::SetGrenades(unsigned int grenades)
{ 
	if (grenades > m_unGrenadesMAX) 
		return false;
	else
	{
		m_unGrenades = grenades;
		return true;
	}
}

bool Inventory::SetHealthPacks(unsigned int healthpack)
{ 
	if (healthpack > m_unHealthPacksMAX) 
		return false;
	else
	{
		m_unHealthPacks = healthpack;
		return true;
	}
}

bool Inventory::SetMachineGunTowers(unsigned int machineGunTowers)
{ 
	if (machineGunTowers > m_unMachineGunTowersMAX) 
		return false;
	else
	{
		m_unMachineGunTowers = machineGunTowers;
		return true;
	}
}

bool Inventory::SetMapleSyrupTowers(unsigned int mapleSyrupTowers)
{ 
	if (mapleSyrupTowers > m_unMapleSyrupTowersMAX) 
		return false;
	else
	{
		m_unMapleSyrupTowers = mapleSyrupTowers;
		return true;
	}
}

bool Inventory::SetHockeyStickTowers(unsigned int hockeyStickTowers)
{ 
	if (hockeyStickTowers > m_unHockeyStickTowersMAX) 
		return false;
	else
	{
		m_unHockeyStickTowers = hockeyStickTowers;
		return true;
	}
}

bool Inventory::SetLaserTowers(unsigned int laserTowers)
{
	if (laserTowers > m_unLaserTowersMAX) 
		return false;
	else
	{
		m_unLaserTowers = laserTowers;
		return true;
	}
}

bool Inventory::SetLavaTraps(unsigned int lavaTraps)
{ 
	if (lavaTraps > m_unLavaTrapsMAX) 
		return false;
	else
	{
		m_unLavaTraps = lavaTraps;
		return true;
	}
}

bool Inventory::SetSpikeTraps(unsigned int spikeTraps)
{ 
	if (spikeTraps > m_unSpikeTrapsMAX) 
		return false;
	else
	{
		m_unSpikeTraps = spikeTraps;
		return true;
	}
}

bool Inventory::SetDroneCount(unsigned int drones)
{ 
	if (drones > m_unDronesMAX) 
		return false;
	else
	{
		m_unDrones = drones;
		return true;
	}
}

