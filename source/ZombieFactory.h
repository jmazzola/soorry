#pragma once

#include "WaveData.h"

#include "../SGD Wrappers/SGD_Listener.h"

#include "EntityManager.h"

#include <string>
#include <vector>
using namespace std;

class ZombieFactory : public SGD::Listener
{
public:

	ZombieFactory();
	~ZombieFactory();

	/**********************************************************/
	// Interface
	bool LoadWaves(string fileName);
	void Start();
	void Stop();
	void Update(float dt);
	virtual void HandleEvent(const SGD::Event* pEvent);

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
	int GetSpawnWidth() const;
	int GetSpawnHeight() const;
	int GetTotalWaves() const;
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
	void SetSpawnWidth(int spawnWidth);
	void SetSpawnHeight(int spawnHeight);
	void SetBuildTime(float buildTime);
	void SetBuildTImeRemaining(float buildTimeRemaining);
	void SetSpawnInterval(float spawnInterval);
	void SetNextSpawnTime(float nextSpawnTime);
	void SetEntityManager(EntityManager* entityManager);

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
	int m_nSpawnWidth;
	int m_nSpawnHeight;
	float m_fBuildTime;
	float m_fBuildTimeRemaining;
	float m_fSpawnInterval;
	float m_fNextSpawnTime;
	vector<WaveData> waveData;
	EntityManager* m_pEntityManager;
};

