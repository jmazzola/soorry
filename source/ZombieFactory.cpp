#include "ZombieFactory.h"

#include "CreateSlowZombieMessage.h"
#include "CreateFastZombieMessage.h"
#include "CreateBeaverZombieMessage.h"

#include "../TinyXML/tinyxml.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"


ZombieFactory::ZombieFactory() : Listener(this)
{
	RegisterForEvent("TEST");
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
	// FOR DEBUG PURPOSES ONLY!
	/*if (m_nEnemiesRemaining > 0 && SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::K))
		m_nEnemiesRemaining--;*/

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
			m_nSlowZombiesToSpawn = waveData[m_nWave - 1].slowZombies;
			m_nFastZombiesToSpawn = waveData[m_nWave - 1].fastZombies;
			m_nBeaverZombiesToSpawn = waveData[m_nWave - 1].beaverZombies;
			m_fSpawnInterval = waveData[m_nWave - 1].spawnInterval;
			return;
		}
	}

	// Zombie mode
	else
	{
		// Check if all enemies are dead
		int zombiesToSpawn = m_nBeaverZombiesToSpawn + m_nSlowZombiesToSpawn + m_nFastZombiesToSpawn;
		if (m_nEnemiesRemaining <= 0 && zombiesToSpawn == 0)
		{
			m_bBuildMode = true;
			m_nWave++;

			// Pause if last wave
			if (m_nWave > (int)waveData.size())
			{
				m_bIsPaused = true;
				return;
			}

			m_fBuildTimeRemaining = (float)waveData[m_nWave - 1].buildTime;
			return;
		}

		// Update timers
		m_fNextSpawnTime -= dt;

		// Spawn zombies when ready
		if (m_fNextSpawnTime <= 0.0f && zombiesToSpawn > 0)
		{
			// Determine which zombie to spawn
			int zombieType;
			bool invalid = false;
			do
			{
				invalid = false;
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
			int spawnX = 0;
			int spawnY = 0;
			do
			{
				invalid = false;

				// Determine which side
				int side = rand() % 4;
				switch (side)
				{
				case 0:
					// Right edge
					spawnX = m_nSpawnWidth - 31;
					spawnY = rand() % (m_nSpawnHeight - 31);
					break;
				case 1:
					// Top edge
					spawnX = rand() % (m_nSpawnWidth - 31);
					spawnY = 1;
					break;
				case 2:
					// Left edge
					spawnX = 1;
					spawnY = rand() % (m_nSpawnHeight - 31);
					break;
				case 3:
					// Bottom edge
					spawnX = rand() % (m_nSpawnWidth - 31);
					spawnY = m_nSpawnHeight - 31;
					break;
				}

				// Make sure we're not spawning on top of something
				if (m_pEntityManager->CheckCollision(SGD::Rectangle((float)spawnX, (float)spawnY, (float)(spawnX + 32), (float)(spawnY + 32))))
					invalid = true;

			} while (invalid);

			// Spawn the zombie
			switch (zombieType)
			{
			case 0:
			{
					  // Slow zombie
					  CreateSlowZombieMessage* pMsg = new CreateSlowZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
					  m_nSlowZombiesToSpawn--;
			}
				break;
			case 1:
			{
					  // Fast zombie
					  CreateFastZombieMessage* pMsg = new CreateFastZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
					  m_nFastZombiesToSpawn--;
			}
				break;
			case 2:
			{
					  // Beaver zombie
					  CreateBeaverZombieMessage* pMsg = new CreateBeaverZombieMessage(spawnX, spawnY);
					  pMsg->QueueMessage();
					  m_nBeaverZombiesToSpawn--;
			}
				break;
			}

			// Update counters
			m_nEnemiesRemaining++;

			// Reset timers
			m_fNextSpawnTime = m_fSpawnInterval;
		}
	}
}

void ZombieFactory::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "TEST")
	{
		//m_nEnemiesRemaining++;
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

void ZombieFactory::SetEntityManager(EntityManager* _entityManager)
{
	m_pEntityManager = _entityManager;
}