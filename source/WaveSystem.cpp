#include "WaveSystem.h"

#include "../TinyXML/tinyxml.h"


WaveSystem::WaveSystem()
{
}


WaveSystem::~WaveSystem()
{
}


/**********************************************************/
// Interface

bool WaveSystem::LoadWaves(string _fileName)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	if (doc.LoadFile(_fileName.c_str()) == false)
		return false;

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Make sure the root is there
	if (pRoot == nullptr)
		return false;

	// Get the first wave
	TiXmlElement* pWave = pRoot->FirstChildElement("wave");
}

void WaveSystem::Update(float dt)
{

}

/**********************************************************/
// Accessors

bool WaveSystem::IsBuildMode() const
{
	return m_bBuildMode;
}

bool WaveSystem::IsInfiniteBuildTime() const
{
	return m_bInfiniteBuildTime;
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

float WaveSystem::GetSpawnInterval() const
{
	return m_fSpawnInterval;
}

float WaveSystem::GetNextSpawnTime() const
{
	return m_fNextSpawnTime;
}

/**********************************************************/
// Mutators

void WaveSystem::SetBuildMode(bool _buildMode)
{
	m_bBuildMode = _buildMode;
}

void WaveSystem::SetInfiniteBuildTime(bool _infiniteBuildTime)
{
	m_bInfiniteBuildTime = _infiniteBuildTime;
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

void WaveSystem::SetSpawnInterval(float _spawnInterval)
{
	m_fSpawnInterval = _spawnInterval;
}

void WaveSystem::SetNextSpawnTime(float _nextSpawnTime)
{
	m_fNextSpawnTime = _nextSpawnTime;
}