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
	void SetWalls(unsigned int walls);
	void SetWindows(unsigned int windows);
	void SetBearTraps(unsigned int beartraps);
	void SetMines(unsigned int mines);
	void SetGrenades(unsigned int grenades);
	void SetHealthPacks(unsigned int healthpack);
	void SetMachineGunTowers(unsigned int machineGunTowers);
	void SetMapleSyrupTowers(unsigned int mapleSyrupTowers);
	void SetHockeyStickTowers(unsigned int hockeyStickTowers);
	void SetLaserTowers(unsigned int laserTowers);
	void SetLavaTraps(unsigned int lavaTraps);
	void SetSpikeTraps(unsigned int spikeTraps);
	void SetDroneCount(unsigned int drones);

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
	unsigned int m_unBearTrapsMAX = 100;
	unsigned int m_unMinesMAX = 100;
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

