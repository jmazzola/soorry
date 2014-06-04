#pragma once
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

	/**********************************************************/
	// Mutators
	void SetWalls(unsigned int walls) { m_unWalls = walls; }
	void SetWindows(unsigned int windows) {  m_unWindows = windows; }
	void SetBearTraps(unsigned int beartraps) { m_unBearTraps = beartraps; }
	void SetMines(unsigned int mines) { m_unMines = mines; }
	void SetGrenades(unsigned int grenades) { m_unGrenades = grenades; }
	void SetHealthPacks(unsigned int healthpack) { m_unHealthPacks = healthpack; }
	void SetMachineGunTowers(unsigned int machineGunTowers) { m_unMachineGunTowers = machineGunTowers; }
	void SetMapleSyrupTowers(unsigned int mapleSyrupTowers) { m_unMapleSyrupTowers = mapleSyrupTowers; }
	void SetHockeyStickTowers(unsigned int hockeyStickTowers) { m_unHockeyStickTowers = hockeyStickTowers; }
	void SetLaserTowers(unsigned int laserTowers) { m_unLaserTowers = laserTowers; }

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
};

