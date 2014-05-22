#include "ZombieFactory.h"

#include "../TinyXML/tinyxml.h"


ZombieFactory::ZombieFactory()
{
}


ZombieFactory::~ZombieFactory()
{
}


/**********************************************************/
// Interface

bool ZombieFactory::LoadWaves(string _fileName)
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

	// Go through all the waves
	while (pWave != nullptr)
	{
		// Create a WaveData
		WaveData wd;

		// Get the data
		string infinite = pWave->FirstChildElement("infinite_build_time")->Attribute("value");
		wd.infiniteBuildTime = (infinite == "true") ? true : false;
		pWave->FirstChildElement("build_time")->Attribute("value", &wd.buildTime);
		pWave->FirstChildElement("slow_zombies")->Attribute("amount", &wd.slowZombies);
		pWave->FirstChildElement("fast_zombies")->Attribute("amount", &wd.fastZombies);
		pWave->FirstChildElement("beaver_zombies")->Attribute("amount", &wd.beaverZombies);
		double _spawnInterval;
		pWave->FirstChildElement("spawn_interval")->Attribute("value", &_spawnInterval);
		wd.spawnInterval = (float)_spawnInterval;

		// Add WaveData to container
		waveData.push_back(wd);

		// Move on to next wave
		pWave = pWave->NextSiblingElement("wave");
	}
}

void ZombieFactory::Update(float dt)
{
	//loop for amount, account for spawn delay
}

/**********************************************************/
// Accessors

bool ZombieFactory::IsPaused() const
{
	return m_bIsPaused;
}

bool ZombieFactory::IsBuildMode() const
{
	return m_bBuildMode;
}

bool ZombieFactory::IsInfiniteBuildTime() const
{
	return m_bInfiniteBuildTime;
}

int ZombieFactory::GetWave() const
{
	return m_nWave;
}

int ZombieFactory::GetEnemiesRemaining() const
{
	return m_nEnemiesRemaining;
}

int ZombieFactory::GetSlowZombiesToSpawn() const
{
	return m_nSlowZombiesToSpawn;
}

int ZombieFactory::GetFastZombiesToSpawn() const
{
	return m_nFastZombiesToSpawn;
}

int ZombieFactory::GetBeaverZombiesToSpawn() const
{
	return m_nBeaverZombiesToSpawn;
}

float ZombieFactory::GetBuildTime() const
{
	return m_fBuildTime;
}

float ZombieFactory::GetBuildTimeRemaining() const
{
	return m_fBuildTimeRemaining;
}

float ZombieFactory::GetSpawnInterval() const
{
	return m_fSpawnInterval;
}

float ZombieFactory::GetNextSpawnTime() const
{
	return m_fNextSpawnTime;
}

/**********************************************************/
// Mutators

void ZombieFactory::SetPaused(bool _isPaused)
{
	m_bIsPaused = _isPaused;
}

void ZombieFactory::SetBuildMode(bool _buildMode)
{
	m_bBuildMode = _buildMode;
}

void ZombieFactory::SetInfiniteBuildTime(bool _infiniteBuildTime)
{
	m_bInfiniteBuildTime = _infiniteBuildTime;
}

void ZombieFactory::SetWave(int _wave)
{
	m_nWave = _wave;
}

void ZombieFactory::SetEnemiesRemaining(int _enemiesRemaining)
{
	m_nEnemiesRemaining = _enemiesRemaining;
}

void ZombieFactory::SetSlowZombiesToSpawn(int _slowZombiesToSpawn)
{
	m_nSlowZombiesToSpawn = _slowZombiesToSpawn;
}

void ZombieFactory::SetFastZombiesToSpawn(int _fastZombiesToSpawn)
{
	m_nFastZombiesToSpawn = _fastZombiesToSpawn;
}

void ZombieFactory::SetBeaverZombiesToSpawn(int _beaverZombiesToSpawn)
{
	m_nBeaverZombiesToSpawn = _beaverZombiesToSpawn;
}

void ZombieFactory::SetBuildTime(float _buildTime)
{
	m_fBuildTime = _buildTime;
}

void ZombieFactory::SetBuildTImeRemaining(float _buildTimeRemaining)
{
	m_fBuildTimeRemaining = _buildTimeRemaining;
}

void ZombieFactory::SetSpawnInterval(float _spawnInterval)
{
	m_fSpawnInterval = _spawnInterval;
}

void ZombieFactory::SetNextSpawnTime(float _nextSpawnTime)
{
	m_fNextSpawnTime = _nextSpawnTime;
}