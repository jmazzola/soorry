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
	unsigned int GetGrenads() { return m_unGrenads; }
	unsigned int GetHealthPacks() { return m_unHealthPacks; }

	/**********************************************************/
	// Mutators
	void SetWalls(unsigned int walls) { m_unWalls = walls; }
	void SetWindows(unsigned int windows) {  m_unWindows = windows; }
	void SetBearTraps(unsigned int beartraps) { m_unBearTraps = beartraps; }
	void SetMines(unsigned int mines) { m_unMines = mines; }
	void SetGrenads(unsigned int grenads) { m_unGrenads = grenads; }
	void SetHealthPacks(unsigned int healthpack) { m_unHealthPacks = healthpack; }


private:
	unsigned int m_unWalls;
	unsigned int m_unWindows;
	unsigned int m_unBearTraps;
	unsigned int m_unMines;
	unsigned int m_unGrenads;
	unsigned int m_unHealthPacks;
};

