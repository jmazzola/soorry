#include "WaveSystem.h"


WaveSystem::WaveSystem()
{
}


WaveSystem::~WaveSystem()
{
}


/**********************************************************/
// Interface

void WaveSystem::Update(float dt)
{

}

/**********************************************************/
// Accessors

bool WaveSystem::IsBuildMode() const
{
	return m_bBuildMode;
}

int WaveSystem::GetWave() const
{
	return m_nWave;
}

int WaveSystem::GetEnemiesRemaining() const
{
	return m_nEnemiesRemaining;
}

int WaveSystem::GetSlowZombiesToSpawn() const
{
	return m_nSlowZombiesToSpawn;
}

int WaveSystem::GetFastZombiesToSpawn() const
{
	return m_nFastZombiesToSpawn;
}

int WaveSystem::GetBeaverZombiesToSpawn() const
{
	return m_nBeaverZombiesToSpawn;
}

float WaveSystem::GetBuildTime() const
{
	return m_fBuildTime;
}

float WaveSystem::GetBuildTimeRemaining() const
{
	return m_fBuildTimeRemaining;
}

/**********************************************************/
// Mutators

void WaveSystem::SetBuildMode(bool _buildMode)
{
	m_bBuildMode = _buildMode;
}

void WaveSystem::SetWave(int _wave)
{
	m_nWave = _wave;
}

void WaveSystem::SetEnemiesRemaining(int _enemiesRemaining)
{
	m_nEnemiesRemaining = _enemiesRemaining;
}

void WaveSystem::SetSlowZombiesToSpawn(int _slowZombiesToSpawn)
{
	m_nSlowZombiesToSpawn = _slowZombiesToSpawn;
}

void WaveSystem::SetFastZombiesToSpawn(int _fastZombiesToSpawn)
{
	m_nFastZombiesToSpawn = _fastZombiesToSpawn;
}

void WaveSystem::SetBeaverZombiesToSpawn(int _beaverZombiesToSpawn)
{
	m_nBeaverZombiesToSpawn = _beaverZombiesToSpawn;
}

void WaveSystem::SetBuildTime(float _buildTime)
{
	m_fBuildTime = _buildTime;
}

void WaveSystem::SetBuildTImeRemaining(float _buildTimeRemaining)
{
	m_fBuildTimeRemaining = _buildTimeRemaining;
}