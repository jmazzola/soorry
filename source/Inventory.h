#pragma once
#include <Windows.h>
class Inventory
{
public:
	Inventory();
	~Inventory();

	/**********************************************************/
	// Accessors
	unsigned int GetWalls() { return m_unWalls; }
	unsigned int GetWindows() { return m_unWindows; }
	unsigned int GetBearTraps() { return m_unBearTraps; }
	unsigned int GetMines() { return m_unMines; }
	unsigned int GetGrenades() { return m_unGrenades; }
	unsigned int GetHealthPacks() { return m_unHealthPacks; }
	unsigned int GetMachineGunTowers() { return m_unMachineGunTowers; }
	unsigned int GetMapleSyrupTowers() { return m_unMapleSyrupTowers; }
	unsigned int GetHockeyStickTowers() { return m_unHockeyStickTowers; }
	unsigned int GetLaserTowers() { return m_unLaserTowers; }
	unsigned int GetLavaTraps() { return m_unLavaTraps; }
	unsigned int GetSpikeTraps() { return m_unSpikeTraps; }
	unsigned int GetDroneCount() { return m_unDrones; }
	

	/**********************************************************/
	// Mutators
	bool SetWalls(unsigned int walls);
	bool SetWindows(unsigned int windows);
	bool SetBearTraps(unsigned int beartraps);
	bool SetMines(unsigned int mines);
	bool SetGrenades(unsigned int grenades);
	bool SetHealthPacks(unsigned int healthpack);
	bool SetMachineGunTowers(unsigned int machineGunTowers);
	bool SetMapleSyrupTowers(unsigned int mapleSyrupTowers);
	bool SetHockeyStickTowers(unsigned int hockeyStickTowers);
	bool SetLaserTowers(unsigned int laserTowers);
	bool SetLavaTraps(unsigned int lavaTraps);
	bool SetSpikeTraps(unsigned int spikeTraps);
	bool SetDroneCount(unsigned int drones);

private:

	unsigned int m_unWalls;
	unsigned int m_unWindows;
	unsigned int m_unBearTraps;
	unsigned int m_unMines;
	unsigned int m_unGrenades;
	unsigned int m_unHealthPacks;
	unsigned int m_unMachineGunTowers;
	unsigned int m_unMapleSyrupTowers;
	unsigned int m_unHockeyStickTowers;
	unsigned int m_unLaserTowers;
	unsigned int m_unLavaTraps;
	unsigned int m_unSpikeTraps;
	unsigned int m_unDrones;


	unsigned int m_unWallsMAX = 100;
	unsigned int m_unWindowsMAX = 100;
	unsigned int m_unBearTrapsMAX = 50;
	unsigned int m_unMinesMAX = 50;
	unsigned int m_unGrenadesMAX = 20;
	unsigned int m_unHealthPacksMAX = 20;
	unsigned int m_unMachineGunTowersMAX = 50;
	unsigned int m_unMapleSyrupTowersMAX = 50;
	unsigned int m_unHockeyStickTowersMAX = 50;
	unsigned int m_unLaserTowersMAX = 50;
	unsigned int m_unLavaTrapsMAX = 100;
	unsigned int m_unSpikeTrapsMAX = 100;
	unsigned int m_unDronesMAX = 5;
};

