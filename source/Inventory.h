#pragma once
#include <Windows.h>

// Defines for easy balancing
#define WALLS_MAX 100
#define WINDOWS_MAX 110
#define BEARTRAPS_MAX 50
#define MINES_MAX 50
#define GRENADES_MAX 22
#define HEALTHPACKS_MAX 20
#define TOWER_MG_MAX 50
#define TOWER_MAPLE_MAX 50
#define TOWER_HOCKEY_MAX 50
#define TOWER_LASER_MAX 50
#define LAVATRAP_MAX 100
#define SPIKETRAP_MAX 100
#define DRONE_MAX 5

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


	unsigned int m_unWallsMAX = WALLS_MAX;
	unsigned int m_unWindowsMAX = WINDOWS_MAX;
	unsigned int m_unBearTrapsMAX = BEARTRAPS_MAX;
	unsigned int m_unMinesMAX = MINES_MAX;
	unsigned int m_unGrenadesMAX = GRENADES_MAX;
	unsigned int m_unHealthPacksMAX = HEALTHPACKS_MAX;
	unsigned int m_unMachineGunTowersMAX = TOWER_MG_MAX;
	unsigned int m_unMapleSyrupTowersMAX = TOWER_MAPLE_MAX;
	unsigned int m_unHockeyStickTowersMAX = TOWER_HOCKEY_MAX;
	unsigned int m_unLaserTowersMAX = TOWER_LASER_MAX;
	unsigned int m_unLavaTrapsMAX = LAVATRAP_MAX;
	unsigned int m_unSpikeTrapsMAX = SPIKETRAP_MAX;
	unsigned int m_unDronesMAX = DRONE_MAX;

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
};

