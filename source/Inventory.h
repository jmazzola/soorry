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
	

	/**********************************************************/
	// Mutators
	void SetWalls(unsigned int walls) { if (walls == UINT_MAX) walls = 0; m_unWalls = walls; }
	void SetWindows(unsigned int windows) { if (windows == UINT_MAX) windows = 0; m_unWindows = windows; }
	void SetBearTraps(unsigned int beartraps) { if (beartraps == UINT_MAX) beartraps = 0; m_unBearTraps = beartraps; }
	void SetMines(unsigned int mines) { if (mines == UINT_MAX) mines = 0; m_unMines = mines; }
	void SetGrenades(unsigned int grenades) { if (grenades == UINT_MAX) grenades = 0; m_unGrenades = grenades; }
	void SetHealthPacks(unsigned int healthpack) { if (healthpack == UINT_MAX) healthpack = 0; m_unHealthPacks = healthpack; }
	void SetMachineGunTowers(unsigned int machineGunTowers) { if (machineGunTowers == UINT_MAX) machineGunTowers = 0; m_unMachineGunTowers = machineGunTowers; }
	void SetMapleSyrupTowers(unsigned int mapleSyrupTowers) { if (mapleSyrupTowers == UINT_MAX) mapleSyrupTowers = 0; m_unMapleSyrupTowers = mapleSyrupTowers; }
	void SetHockeyStickTowers(unsigned int hockeyStickTowers) { if (hockeyStickTowers == UINT_MAX) hockeyStickTowers = 0; m_unHockeyStickTowers = hockeyStickTowers; }
	void SetLaserTowers(unsigned int laserTowers) { if (laserTowers == UINT_MAX) laserTowers = 0; m_unLaserTowers = laserTowers; }
	void SetLavaTraps(unsigned int lavaTraps) { if (lavaTraps == UINT_MAX) lavaTraps = 0; m_unLavaTraps = lavaTraps; }
	void SetSpikeTraps(unsigned int spikeTraps) { if (spikeTraps == UINT_MAX) spikeTraps = 0; m_unSpikeTraps = spikeTraps; }

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
};

