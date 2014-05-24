#include "ZombieFactory.h"

#include "CreateSlowZombieMessage.h"
#include "CreateFastZombieMessage.h"
#include "CreateBeaverZombieMessage.h"

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

	// Did we load anything?
	return (waveData.size() > 0);
}

void ZombieFactory::Start()
{
	// Default start behavior
	m_bIsPaused = false;
	m_bBuildMode = true;

	// Start at wave one
	m_nWave = 1;
	m_bInfiniteBuildTime = waveData[m_nWave - 1].infiniteBuildTime;
	m_nSlowZombiesToSpawn = waveData[m_nWave - 1].slowZombies;
	m_nFastZombiesToSpawn = waveData[m_nWave - 1].fastZombies;
	m_nBeaverZombiesToSpawn = waveData[m_nWave - 1].beaverZombies;
	m_nEnemiesRemaining = 0;
	m_fBuildTime = (float)waveData[m_nWave - 1].buildTime;
	m_fBuildTimeRemaining = m_fBuildTime;
	m_fSpawnInterval = waveData[m_nWave - 1].spawnInterval;
	m_fNextSpawnTime = 0.0f;
}

void ZombieFactory::Stop()
{

}

void ZombieFactory::Update(float dt)
{
	// If paused, don't do anything
	if (m_bIsPaused)
		return;

	// Build mode
	if (m_bBuildMode)
	{
		// Update timers
		m_fBuildTimeRemaining -= dt;

		// Check to see if we leave build mode
		if (m_fBuildTimeRemaining <= 0.0f)
		{
			m_bBuildMode = false;
			return;
		}
	}

	// Zombie mode
	else
	{
		// Check if all enemies are dead
		if (m_nEnemiesRemaining <= 0)
		{
			m_bBuildMode = true;
			return;
		}

		// Update timers
		m_fNextSpawnTime -= dt;

		// Spawn zombies when ready
		if (m_fNextSpawnTime <= 0.0f)
		{
			// Determine which zombie to spawn
			int zombieType;
			bool invalid = false;
			do
			{
				zombieType = rand() % 3;
				switch (zombieType)
				{
				case 0:
					// Slow zombie
					if (m_nSlowZombiesToSpawn <= 0)
						invalid = true;
					break;
				case 1:
					// Fast zombie
					if (m_nFastZombiesToSpawn <= 0)
						invalid = true;
					break;
				case 2:
					// Beaver zombie
					if (m_nBeaverZombiesToSpawn <= 0)
						invalid = true;
					break;
				}
			} while (invalid);

			// Determine where to spawn
			invalid = false;
			int spawnX;
			int spawnY;
			do
			{
				// Determine which side
				int side = rand() % 4;
				switch (side)
				{
				case 0:
					// Right edge
					spawnX = m_nSpawnWidth - 32;
					spawnY = rand() % (m_nSpawnHeight - 32);
					break;
				case 1:
					// Top edge
					spawnX = rand() % (m_nSpawnWidth - 32);
					spawnY = 0;
					break;
				case 2:
					// Left edge
					spawnX = 0;
					spawnY = rand() % (m_nSpawnHeight - 32);
					break;
				case 3:
					// Bottom edge
					spawnX = rand() % (m_nSpawnWidth - 32);
					spawnY = m_nSpawnHeight - 32;
					break;
				}
			} while (invalid);

			// Spawn the zombie
			switch (zombieType)
			{
			case 0:
			{
					  // Slow zombie
					  CreateSlowZombieMessage* pMsg = new CreateSlowZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
			}
				break;
			case 1:
			{
					  // Fast zombie
					  CreateFastZombieMessage* pMsg = new CreateFastZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
			}
				break;
			case 2:
			{
					  // Beaver zombie
					  CreateBeaverZombieMessage* pMsg = new CreateBeaverZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
			}
				break;
			}

			// Reset timers
			m_fNextSpawnTime = m_fSpawnInterval;
		}
	}
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

int ZombieFactory::GetSpawnWidth() const
{
	return m_nSpawnWidth;
}

int ZombieFactory::GetSpawnHeight() const
{
	return m_nSpawnHeight;
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

void ZombieFactory::SetSpawnWidth(int _spawnWidth)
{
	m_nSpawnWidth = _spawnWidth;
}

void ZombieFactory::SetSpawnHeight(int _spawnHeight)
{
	m_nSpawnHeight = _spawnHeight;
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