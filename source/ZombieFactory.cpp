#include "ZombieFactory.h"

#include "CreateSlowZombieMessage.h"
#include "CreateFastZombieMessage.h"
#include "CreateBeaverZombieMessage.h"
#include "WaveCompleteMessage.h"
#include "Player.h"
#include "StatTracker.h"

#include "../TinyXML/tinyxml.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cmath>

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

	int infinite;
	pRoot->Attribute("infinite", &infinite);
	if (infinite == 0)
		m_bInfiniteWaves = false;
	else
		m_bInfiniteWaves = true;

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

	if (m_bInfiniteWaves == false)
	{
		m_bInfiniteBuildTime = waveData[m_nWave - 1].infiniteBuildTime;
		m_nSlowZombiesToSpawn = waveData[m_nWave - 1].slowZombies;
		m_nFastZombiesToSpawn = waveData[m_nWave - 1].fastZombies;
		m_nBeaverZombiesToSpawn = waveData[m_nWave - 1].beaverZombies;
		m_fBuildTime = (float)waveData[m_nWave - 1].buildTime;
		m_fBuildTimeRemaining = m_fBuildTime;
		m_fSpawnInterval = waveData[m_nWave - 1].spawnInterval;
	}
	else
	{
		m_fBuildTime = 10.0f;
		m_fBuildTimeRemaining = m_fBuildTime;
		m_bInfiniteBuildTime = false;
		m_nSlowZombiesToSpawn = 10;
		m_nFastZombiesToSpawn = 10;
		m_nBeaverZombiesToSpawn = 10;
		m_fSpawnInterval = 1.0f;
	}

	m_nEnemiesRemaining = 0;
	m_fNextSpawnTime = 0.0f;

	// Null out alpha zombies
	m_pSlowAlpha = nullptr;
	m_pFastAlpha = nullptr;
	m_pBeaverAlpha = nullptr;
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
		//m_fBuildTimeRemaining -= dt; INFINITE BUILD MODE EVERYWHERE!!!

		// Check to see if we leave build mode
		if (m_fBuildTimeRemaining <= 0.0f)
		{
			m_bBuildMode = false;

			if (m_bInfiniteWaves == false)
			{
				m_nSlowZombiesToSpawn = waveData[m_nWave - 1].slowZombies;
				m_nFastZombiesToSpawn = waveData[m_nWave - 1].fastZombies;
				m_nBeaverZombiesToSpawn = waveData[m_nWave - 1].beaverZombies;
				m_fSpawnInterval = waveData[m_nWave - 1].spawnInterval;
			}

			else
			{
				m_nSlowZombiesToSpawn = (int)(10 * pow(1.5f, m_nWave));
				m_nFastZombiesToSpawn = (int)(10 * pow(1.6f, m_nWave));
				m_nBeaverZombiesToSpawn = (int)(10 * pow(1.4f, m_nWave));
				m_fSpawnInterval = 0.02f + 0.2f / m_nWave;
			}

			// Deselect all towers
			m_pPlayer->SetSelectedTower(nullptr);

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
			
			StatTracker::GetInstance()->SurvivedRound(m_nWave);
			m_bBuildMode = true;
			m_nWave++;

			WaveCompleteMessage* msg = new WaveCompleteMessage();
			msg->QueueMessage();

			// Pause if last wave
			if (m_nWave > (int)waveData.size() && !m_bInfiniteWaves)
			{
				m_bIsPaused = true;
				return;
			}

			m_fBuildTimeRemaining = 10.0f;

			m_pSlowAlpha = nullptr;
			m_pFastAlpha = nullptr;
			m_pBeaverAlpha = nullptr;

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

bool ZombieFactory::IsInfiniteWaves() const
{
	return m_bInfiniteWaves;
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

int ZombieFactory::GetTotalWaves() const
{
	return (m_bInfiniteWaves) ? INT_MAX : waveData.size();
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

Enemy* ZombieFactory::GetSlowAlpha() const
{
	return m_pSlowAlpha;
}

Enemy* ZombieFactory::GetFastAlpha() const
{
	return m_pFastAlpha;
}

Enemy* ZombieFactory::GetBeaverAlpha() const
{
	return m_pBeaverAlpha;
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

void ZombieFactory::SetInfiniteWaves(bool _infiniteWaves)
{
	m_bInfiniteWaves = _infiniteWaves;
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

void ZombieFactory::SetPlayer(Player* _player)
{
	m_pPlayer = _player;
}

void ZombieFactory::SetSlowAlpha(Enemy* _slowAlpha)
{
	m_pSlowAlpha = _slowAlpha;
}

void ZombieFactory::SetFastAlpha(Enemy* _fastAlpha)
{
	m_pFastAlpha = _fastAlpha;
}

void ZombieFactory::SetBeaverAlpha(Enemy* _beaverAlpha)
{
	m_pBeaverAlpha = _beaverAlpha;
}
