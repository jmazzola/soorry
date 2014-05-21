#pragma once

#include "WaveData.h"

#include <string>
#include <vector>
using namespace std;

class ZombieFactory
{
public:

	ZombieFactory();
	~ZombieFactory();

	/**********************************************************/
	// Interface
	bool LoadWaves(string fileName);
	void Update(float dt);

	/**********************************************************/
	// Accessors
	bool IsPaused() const;
	bool IsBuildMode() const;
	bool IsInfiniteBuildTime() const;
	int GetWave() const;
	int GetEnemiesRemaining() const;
	int GetSlowZombiesToSpawn() const;
	int GetFastZombiesToSpawn() const;
	int GetBeaverZombiesToSpawn() const;
	float GetBuildTime() const;
	float GetBuildTimeRemaining() const;
	float GetSpawnInterval() const;
	float GetNextSpawnTime() const;

	/**********************************************************/
	// Mutators
	void SetPaused(bool isPaused);
	void SetBuildMode(bool buildMode);
	void SetInfiniteBuildTime(bool infiniteBuildTime);
	void SetWave(int wave);
	void SetEnemiesRemaining(int enemiesRemaining);
	void SetSlowZombiesToSpawn(int slowZombiesToSpawn);
	void SetFastZombiesToSpawn(int fastZombiesToSpawn);
	void SetBeaverZombiesToSpawn(int beaverZombiesToSpawn);
	void SetBuildTime(float buildTime);
	void SetBuildTImeRemaining(float buildTimeRemaining);
	void SetSpawnInterval(float spawnInterval);
	void SetNextSpawnTime(float nextSpawnTime);

protected:

	/**********************************************************/
	// Data Members
	bool m_bIsPaused;
	bool m_bBuildMode;
	bool m_bInfiniteBuildTime;
	int m_nWave;
	int m_nEnemiesRemaining;
	int m_nSlowZombiesToSpawn;
	int m_nFastZombiesToSpawn;
	int m_nBeaverZombiesToSpawn;
	float m_fBuildTime;
	float m_fBuildTimeRemaining;
	float m_fSpawnInterval;
	float m_fNextSpawnTime;
	vector<WaveData> waveData;
};

