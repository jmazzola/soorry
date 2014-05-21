#pragma once

class WaveSystem
{
public:

	WaveSystem();
	~WaveSystem();

	/**********************************************************/
	// Interface
	void Update(float dt);

	/**********************************************************/
	// Accessors
	bool IsBuildMode() const;
	int GetWave() const;
	int GetEnemiesRemaining() const;
	int GetSlowZombiesToSpawn() const;
	int GetFastZombiesToSpawn() const;
	int GetBeaverZombiesToSpawn() const;
	float GetBuildTime() const;
	float GetBuildTimeRemaining() const;

	/**********************************************************/
	// Mutators
	void SetBuildMode(bool buildMode);
	void SetWave(int wave);
	void SetEnemiesRemaining(int enemiesRemaining);
	void SetSlowZombiesToSpawn(int slowZombiesToSpawn);
	void SetFastZombiesToSpawn(int fastZombiesToSpawn);
	void SetBeaverZombiesToSpawn(int beaverZombiesToSpawn);
	void SetBuildTime(float buildTime);
	void SetBuildTImeRemaining(float buildTimeRemaining);

protected:

	/**********************************************************/
	// Data Members
	bool m_bBuildMode;
	int m_nWave;
	int m_nEnemiesRemaining;
	int m_nSlowZombiesToSpawn;
	int m_nFastZombiesToSpawn;
	int m_nBeaverZombiesToSpawn;
	float m_fBuildTime;
	float m_fBuildTimeRemaining;
};

