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
	{
		if (m_unWalls == m_unWallsMAX)
			return false;

		m_unWalls = m_unWallsMAX;
		return true;
	}
	else
	{
		m_unWalls = walls;
		return true;
	}
}

bool Inventory::SetWindows(unsigned int windows)
{ 
	if (windows > m_unWindowsMAX)
	{
		if (m_unWindows == m_unWindowsMAX)
			return false;

		m_unWindows = m_unWindowsMAX;
		return true;

	}
	else
	{
		m_unWindows = windows;
		return true;
	}
}

bool Inventory::SetBearTraps(unsigned int beartraps)
{ 
	if (beartraps > m_unBearTrapsMAX) 
	{
		if (m_unBearTraps == m_unBearTrapsMAX)
			return false;

		m_unBearTraps = m_unBearTrapsMAX;
		return true;

	}
	else
	{
		m_unBearTraps = beartraps;
		return true;
	}
}

bool Inventory::SetMines(unsigned int mines)
{ 
	if (mines > m_unMinesMAX)
	{
		if (m_unMines == m_unMinesMAX)
			return false;

		m_unMines = m_unMinesMAX;
		return true;
	}
	else
	{
		m_unMines = mines;
		return true;
	}
}

bool Inventory::SetGrenades(unsigned int grenades)
{ 
	if (grenades > m_unGrenadesMAX) 
	{
		if (m_unGrenades == m_unGrenadesMAX)
			return false;

		m_unGrenades = m_unGrenadesMAX;
		return true;

	}
	else
	{
		m_unGrenades = grenades;
		return true;
	}
}

bool Inventory::SetHealthPacks(unsigned int healthpack)
{ 
	if (healthpack > m_unHealthPacksMAX)
	{
		if (m_unHealthPacks == m_unHealthPacksMAX)
			return false;

		m_unHealthPacks = m_unHealthPacksMAX;
		return true;
	}
	else
	{
		m_unHealthPacks = healthpack;
		return true;
	}
}

bool Inventory::SetMachineGunTowers(unsigned int machineGunTowers)
{ 
	if (machineGunTowers > m_unMachineGunTowersMAX)
	{
		if (m_unMachineGunTowers == m_unMachineGunTowersMAX)
			return false;

		m_unMachineGunTowers = m_unMachineGunTowersMAX;
		return true;
	}
	else
	{
		m_unMachineGunTowers = machineGunTowers;
		return true;
	}
}

bool Inventory::SetMapleSyrupTowers(unsigned int mapleSyrupTowers)
{ 
	if (mapleSyrupTowers > m_unMapleSyrupTowersMAX)
	{
		if (m_unMapleSyrupTowers == m_unMapleSyrupTowersMAX)
			return false;

		m_unMapleSyrupTowers = m_unMapleSyrupTowersMAX;
		return true;
	}
	else
	{
		m_unMapleSyrupTowers = mapleSyrupTowers;
		return true;
	}
}

bool Inventory::SetHockeyStickTowers(unsigned int hockeyStickTowers)
{ 
	if (hockeyStickTowers > m_unHockeyStickTowersMAX) 
	{
		if (m_unHockeyStickTowers == m_unHockeyStickTowersMAX)
			return false;

		m_unHockeyStickTowers = m_unHockeyStickTowersMAX;
		return true;
	}
	else
	{
		m_unHockeyStickTowers = hockeyStickTowers;
		return true;
	}
}

bool Inventory::SetLaserTowers(unsigned int laserTowers)
{
	if (laserTowers > m_unLaserTowersMAX)
	{
		if (m_unLaserTowers == m_unLaserTowersMAX)
			return false;

		m_unLaserTowers = m_unLaserTowersMAX;
		return true;
	}
	else
	{
		m_unLaserTowers = laserTowers;
		return true;
	}
}

bool Inventory::SetLavaTraps(unsigned int lavaTraps)
{ 
	if (lavaTraps > m_unLavaTrapsMAX)
	{
		if (m_unLavaTraps == m_unLavaTrapsMAX)
			return false;

		m_unLavaTraps = m_unLavaTrapsMAX;
		return true;

	}
	else
	{
		m_unLavaTraps = lavaTraps;
		return true;
	}
}

bool Inventory::SetSpikeTraps(unsigned int spikeTraps)
{ 
	if (spikeTraps > m_unSpikeTrapsMAX)
	{
		if (m_unSpikeTraps == m_unSpikeTrapsMAX)
			return false;

		m_unSpikeTraps = m_unSpikeTrapsMAX;
		return true;
	}
	else
	{
		m_unSpikeTraps = spikeTraps;
		return true;
	}
}

bool Inventory::SetDroneCount(unsigned int drones)
{ 
	if (drones > m_unDronesMAX)
	{
		if (m_unDrones == m_unDronesMAX)
			return false;

		m_unDrones = m_unDronesMAX;
		return true;
	}
	else
	{
		m_unDrones = drones;
		return true;
	}
}

