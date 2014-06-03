#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "AnimationManager.h"
#include "Frame.h"
#include "Sprite.h"
#include "Game.h"
#include "WorldManager.h"
#include "Weapon.h"
#include "ShotgunPellet.h"
#include "Rocket.h"
#include "Projectile.h"
#include "AssaultRifleBullet.h"
#include "CreateProjectileMessage.h"
#include "GameplayState.h"
#include "CreatePlaceableMessage.h"
#include "Inventory.h"
#include "Tile.h"
#include "Camera.h"
#include "CreatePickupMessage.h"
#include "AIComponent.h"
#include "CreateParticleMessage.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "Tower.h"
#include "CreateTowerMessage.h"

#include <queue>
using namespace std;

#define WALLPICK 0
#define WINDOWPICK 1
#define GRIDWIDTH 32
#define GRIDHEIGHT 32

Player::Player() : Listener(this)
{
	// Entity
	m_ptPosition = { 50, 100 };
	m_vtVelocity = { 0, 0 };


	// Animation/ Image
	m_pSprite = AnimationManager::GetInstance()->GetSprite("player");
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "player";
	
	// Player's variables
	m_nMaxHealth = 100.0f;
	m_nCurrHealth = 100.0f;
	m_nCurrWeapon = 0;
	m_nCurrPowerup = -1;
	m_nCurrPlaceable = 0;
	m_unScore = 0;
	m_unEnemiesKilled = 0;
	m_fSpeed = 250.0f;
	m_fScoreMultiplier = 0.0f;
	m_fTimeAlive = 0.0f;

	// Player Inventory
	m_pInventory = new Inventory();
	m_pInventory->SetBearTraps(100);
	m_pInventory->SetGrenades(0);
	m_pInventory->SetHealthPacks(3);
	m_pInventory->SetMines(1);
	m_pInventory->SetWalls(100);
	m_pInventory->SetWindows(100);
	m_pInventory->SetMachineGunTowers(2);
	m_pInventory->SetMapleSyrupTowers(2);
	m_pInventory->SetHockeyStickTowers(2);
	m_pInventory->SetLaserTowers(2);

	m_pWeapons = new Weapon[4];
#pragma region Load Weapons


	//Assault rifle
	Weapon tempWeapon;
	tempWeapon.SetCurrAmmo(30);
	tempWeapon.SetMaxAmmo(500);
	tempWeapon.SetFireRate(.2f);
	tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	m_pWeapons[0] = tempWeapon;

	//Shotgun
	tempWeapon;
	tempWeapon.SetCurrAmmo(10);
	tempWeapon.SetMaxAmmo(500);
	tempWeapon.SetFireRate(.5f);
	tempWeapon.SetType(Guns::TYPE_SHOTGUN);
	m_pWeapons[1] = tempWeapon;

	//rocket launcher
	tempWeapon;
	tempWeapon.SetCurrAmmo(5);
	tempWeapon.SetMaxAmmo(50);
	tempWeapon.SetFireRate(2);
	tempWeapon.SetType(Guns::TYPE_SHOTGUN);
	m_pWeapons[2] = tempWeapon;

	//Fire Axe
	tempWeapon;
	tempWeapon.SetCurrAmmo(0);
	tempWeapon.SetMaxAmmo(0);
	tempWeapon.SetFireRate(.5f);
	tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	m_pWeapons[3] = tempWeapon;
#pragma endregion


	RegisterForEvent("TAKE_DAMAGE");
	RegisterForEvent("INCREASE_SCORE");

	// Create node chart
	WorldManager* pWorld = WorldManager::GetInstance();
	int worldWidth = pWorld->GetWorldWidth();
	int worldHeight = pWorld->GetWorldHeight();
	m_nNodeChart = new int*[worldWidth];
	for (int x = 0; x < worldWidth; x++)
		m_nNodeChart[x] = new int[worldHeight];
}


Player::~Player()
{
	delete[]m_pWeapons;
	delete m_pInventory;

	for (int x = 0; x < WorldManager::GetInstance()->GetWorldWidth(); x++)
		delete[] m_nNodeChart[x];
	delete[] m_nNodeChart;
}


/**********************************************************/
// Interface Methods

/*********************************************************/
// Update
//	- Handle input and move the character
void Player::Update(float dt)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	Game* pGame = Game::GetInstance();
	WorldManager* pWorld = WorldManager::GetInstance();
	//Update  all Timers
	m_fShotTimer -= dt;
	m_fPlaceTimer -= dt;
	SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
	pos.x = (float)((int)(pos.x + Camera::x) / GRIDWIDTH);
	pos.y = (float)((int)(pos.y + Camera::y) / GRIDHEIGHT);
	


	// Set camera
	Camera::x = (int)m_ptPosition.x - 384;
	Camera::y = (int)m_ptPosition.y - 284;

	// Regenerate health
	m_nCurrHealth += 7.0f * dt;
	if (m_nCurrHealth > m_nMaxHealth)
		m_nCurrHealth = m_nMaxHealth;

	//Update Timers

	// Input
	if (pInput->IsKeyDown(SGD::Key::A) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x -= m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true || m_ptPosition.x < 0)
			m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::D) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x += m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true || m_ptPosition.x >= pWorld->GetWorldWidth() * pWorld->GetTileWidth() - pWorld->GetTileWidth())
			m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::W) == true)
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y -= m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true || m_ptPosition.y < 0)
			m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::S) == true)
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y += m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true || m_ptPosition.y >= pWorld->GetWorldHeight() * pWorld->GetTileHeight() - pWorld->GetTileHeight())
			m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::E) == true &&
		m_pInventory->GetHealthPacks() > 0 && m_nCurrHealth < m_nMaxHealth)
	{
		m_nCurrHealth = m_nMaxHealth;
		unsigned int newset = m_pInventory->GetHealthPacks();
		--newset;
		m_pInventory->SetHealthPacks(newset);
	}
	if (pInput->IsKeyPressed(SGD::Key::Space) )
	{
  		CreateParticleMessage* msg = new CreateParticleMessage("Temp_Particle", this, 0, 0);
		msg->QueueMessage();
		msg = nullptr;
		
	}
	//GAH Weapons! - Arnold
	if (pInput->IsKeyPressed(SGD::Key::One) == true && m_pZombieWave->IsBuildMode() == false)
	{
		m_nCurrWeapon = 0;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Two) == true && m_pZombieWave->IsBuildMode() == false)
	{
		m_nCurrWeapon = 1;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Three) == true && m_pZombieWave->IsBuildMode() == false)
	{
		m_nCurrWeapon = 2;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Four) == true && m_pZombieWave->IsBuildMode() == false)
	{
		m_nCurrWeapon = 3;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (m_fShotTimer < 0 && m_pWeapons[m_nCurrWeapon].GetCurrAmmo() > 0)
	{
		if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && m_pZombieWave->IsBuildMode() == false)
		{
			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
			msg->QueueMessage();
			msg = nullptr;
			//set the shot timer to the rate of fire
			int tempInt = m_pWeapons[m_nCurrWeapon].GetCurrAmmo();
			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
			m_pWeapons[m_nCurrWeapon].SetCurrAmmo((m_pWeapons[m_nCurrWeapon].GetCurrAmmo() - 1));
		}
	}
	// Selecting Walls
	if (pInput->IsKeyPressed(SGD::Key::One) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 0;

	// Selecting Windows
	if (pInput->IsKeyPressed(SGD::Key::Two) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 1;

	// Selecting Bear Trap
	if (pInput->IsKeyPressed(SGD::Key::Three) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 2;

	// Selecting Mine
	if (pInput->IsKeyPressed(SGD::Key::Four) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 3;

	// Selecting Machine gun towers
	if (pInput->IsKeyPressed(SGD::Key::Five) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 4;

	// Selecting Maple Syrup towers
	if (pInput->IsKeyPressed(SGD::Key::Six) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 5;

	// Selecting Hockey Stick towers
	if (pInput->IsKeyPressed(SGD::Key::Seven) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 6;

	// Selecting Laser towers
	if (pInput->IsKeyPressed(SGD::Key::Eight) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = 7;

	if (pInput->IsKeyPressed(SGD::Key::MouseRight) == true && Blockable(pos))
	{
		// Test rect
		SGD::Rectangle rect;
		rect.left = pos.x * pWorld->GetTileWidth() + pWorld->GetTileWidth() / 4;
		rect.top = pos.y * pWorld->GetTileHeight() + pWorld->GetTileHeight() / 4;
		rect.right = rect.left + pWorld->GetTileWidth() / 2;
		rect.bottom = rect.top + pWorld->GetTileHeight() / 2;

		// Check if tower is there
		Tower* tower = dynamic_cast<Tower*>(m_pEntityManager->CheckCollision(rect, 5));

		if (tower)
		{

		}

#if 0
		//Colliding with wall
		if (pWorld->GetColliderID((int)pos.x, (int)pos.y) == WALL)
		{
			pWorld->SetColliderID((int)pos.x, (int)pos.y, EMPTY);
			CreatePickupMessage*  pmsg = new CreatePickupMessage(WALLPICK, { pos.x*GRIDWIDTH, pos.y * GRIDHEIGHT });
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
		else if (pWorld->GetColliderID((int)pos.x, (int)pos.y) == WINDOW)
		{
			pWorld->SetColliderID((int)pos.x, (int)pos.y, EMPTY);
			CreatePickupMessage*  pmsg = new CreatePickupMessage(WINDOWPICK, { pos.x*GRIDWIDTH, pos.y * GRIDHEIGHT });
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
#endif
	}

	if (m_pZombieWave->IsBuildMode() == false)
	{
		//if (m_fShotTimer < 0)
		//{
		//	if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
		//	{
		//		switch (m_nCurrWeapon)
		//		{
		//		case 0:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 1:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 2:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 3:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		}
		//	}
		//}
	}
	else
	{
		// Place item
		if (m_nCurrPlaceable != -1)
		{
			// Bear trap
			if (m_nCurrPlaceable == 2 && m_pInventory->GetBearTraps() > 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && m_fPlaceTimer <= 0 && PlacementCheck(pos))
				{
					// Cooldown for placing objects
					m_fPlaceTimer = 1;
					CreatePlaceableMessage* pmsg = new CreatePlaceableMessage(m_ptPosition, m_nCurrPlaceable);
					pmsg->QueueMessage();
					pmsg = nullptr;
					// Decreasing the amount of bear traps left for the player
					unsigned int newset = m_pInventory->GetBearTraps();
					--newset;
					m_pInventory->SetBearTraps(newset);

				}
			}

			// Mine
			else if (m_nCurrPlaceable == 3 && m_pInventory->GetMines() > 0 && m_fPlaceTimer <= 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && PlacementCheck(pos))
				{
					// Cooldown for placing objects
					m_fPlaceTimer = 1;
					CreatePlaceableMessage* pmsg = new CreatePlaceableMessage(m_ptPosition, m_nCurrPlaceable);
					pmsg->QueueMessage();
					pmsg = nullptr;
					// Decreasing the amount of mines left for the player
					unsigned int newset = m_pInventory->GetMines();
					--newset;
					m_pInventory->SetMines(newset);
				}
			}

			// Walls
			else if (m_nCurrPlaceable == 0 && m_pInventory->GetWalls() > 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && PlacementCheck(pos))
				{
					pWorld->SetColliderID((int)pos.x, (int)pos.y, WALL);
					// Decreasing the amount of mines left for the player
					unsigned int newset = m_pInventory->GetWalls();
					--newset;
					m_pInventory->SetWalls(newset);
				}
			}

			// Windows
			else if (m_nCurrPlaceable == 1 && m_pInventory->GetWindows() > 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && PlacementCheck(pos))
				{
					pWorld->SetColliderID((int)pos.x, (int)pos.y, WINDOW);
					// Decreasing the amount of mines left for the player
					unsigned int newset = m_pInventory->GetWindows();
					--newset;
					m_pInventory->SetWindows(newset);
				}
			}

			// Machine gun tower
			else if (m_nCurrPlaceable == 4 && m_pInventory->GetMachineGunTowers() > 0)
			{
				if (pInput->IsKeyPressed(SGD::Key::MouseLeft) == true && PlacementCheck(pos))
				{
					CreateTowerMessage* msg = new CreateTowerMessage(pos.x + pWorld->GetTileWidth(), pos.y + pWorld->GetTileHeight(), 
						CreateTowerMessage::TOWER_MACHINE_GUN);

					// Decreasing the amount of mines left for the player
					m_pInventory->SetMachineGunTowers(m_pInventory->GetMachineGunTowers() - 1);
				}
			}
		}

	}

}

int Player::GetType() const
{
	return ENT_PLAYER;
}

void Player::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ZOMBIE_BEAVER)
	{
		m_nCurrHealth--;
	}
	if (pOther->GetType() == ENT_PICKUP_WALL)
	{
		unsigned int newset = m_pInventory->GetWalls();
		++newset;
		m_pInventory->SetWalls(newset);

	}
	if (pOther->GetType() == ENT_PICKUP_WINDOW)
	{
		unsigned int newset = m_pInventory->GetWindows();
		++newset;
		m_pInventory->SetWindows(newset);

	}
}

void Player::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "TAKE_DAMAGE")
	{
		float damage = *((float*)pEvent->GetData());
		m_nCurrHealth -= damage;

		// Make sure we don't underflow
		if (m_nCurrHealth < 0.0f)
			m_nCurrHealth = 0.0f;
	}

	if (pEvent->GetEventID() == "INCREASE_SCORE")
	{
		int score = *((int*)pEvent->GetData());
		m_unScore += score;
	}
}

bool Player::Blockable(SGD::Point mouse)
{
	return (mouse.x >= 1 && mouse.x < WorldManager::GetInstance()->GetWorldWidth() - 1
		&& mouse.y >= 1 && mouse.y < WorldManager::GetInstance()->GetWorldHeight() - 1);
}


/**********************************************************/
// Accessors

float Player::GetMaxHealth() const
{
	return m_nMaxHealth;
}

float Player::GetCurrHealth() const
{
	return m_nCurrHealth;
}

int Player::GetCurrWeapon() const
{
	return m_nCurrWeapon;
}

int Player::GetCurrPowerup() const
{
	return m_nCurrPowerup;
}

int Player::GetCurrPlaceable() const
{
	return m_nCurrPlaceable;
}

unsigned int Player::GetScore() const
{
	return m_unScore;
}

unsigned int Player::GetEnemiesKilled() const
{
	return m_unEnemiesKilled;
}

float Player::GetSpeed() const
{
	return m_fSpeed;
}

float Player::GetScoreMultiplier() const
{
	return m_fScoreMultiplier;
}

float Player::GetTimeAlive() const
{
	return m_fTimeAlive;
}

Inventory* Player::GetInventory() const
{
	return m_pInventory;
}


Weapon* Player::GetWeapons() const
{
	return m_pWeapons;
}

/**********************************************************/
// Mutators

void Player::SetMaxHealth(float _maxHealth)
{
	m_nMaxHealth = _maxHealth;
}

void Player::SetCurrHealth(float _currHealth)
{
	m_nCurrHealth = _currHealth;
}

void Player::SetCurrWeapon(int _currWeapon)
{
	m_nCurrWeapon = _currWeapon;
}

void Player::SetCurrPowerup(int _currPowerup)
{
	m_nCurrPowerup = _currPowerup;
}

void Player::SetCurrPlaceable(int _currPlaceable)
{
	m_nCurrPlaceable = _currPlaceable;
}

void Player::SetScore(unsigned int _score)
{
	m_unScore = _score;
}

void Player::SetEnemiesKilled(unsigned int _enemiesKilled)
{
	m_unEnemiesKilled = _enemiesKilled;
}

void Player::SetSpeed(float _speed)
{
	m_fSpeed = _speed;
}

void Player::SetScoreMultiplier(float _multiplier)
{
	m_fScoreMultiplier = _multiplier;
}

void Player::SetTimeAlive(float _timeAlive)
{
	m_fTimeAlive = _timeAlive;
}

void Player::SetInventory(Inventory* _inventory)
{
	m_pInventory = _inventory;
}


void Player::SetWeapons(Weapon* _weapons)
{
	m_pWeapons = _weapons;
}


bool Player::CheckLegalPlacement(Node end, Node block)
{
	WorldManager* pWorld = WorldManager::GetInstance();

	Node start;
	start.x = 0;
	start.y = 0;

	// Reset node chart
	for (int x = 0; x < pWorld->GetWorldWidth(); x++)
	{
		for (int y = 0; y < pWorld->GetWorldHeight(); y++)
		{
			if (pWorld->IsSolidAtPosition(x, y))
				m_nNodeChart[x][y] = -1;
			else
				m_nNodeChart[x][y] = 0;
		}
	}

	queue<Node> nodes;

	m_nNodeChart[block.x][block.y] = -1;

	nodes.push(end);
	m_nNodeChart[end.x][end.y] = 1;

	int highestNode = 1;

	while (!nodes.empty())
	{
		Node node;
		node = nodes.front();
		nodes.pop();

		if (node.x == start.x && node.y == start.y)
			return true;

		/*if (node.x < 1 || node.y < 1 || node.x >= m_nWorldWidth - 1 || node.y >= m_nWorldHeight - 1)
		continue;*/

		if (node.x > 0 && m_nNodeChart[node.x - 1][node.y] == 0)
		{
			nodes.push(Node(node.x - 1, node.y));
			m_nNodeChart[node.x - 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.x < pWorld->GetWorldWidth() - 1 && m_nNodeChart[node.x + 1][node.y] == 0)
		{
			nodes.push(Node(node.x + 1, node.y));
			m_nNodeChart[node.x + 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.y < pWorld->GetWorldHeight() - 1 && m_nNodeChart[node.x][node.y + 1] == 0)
		{
			nodes.push(Node(node.x, node.y + 1));
			m_nNodeChart[node.x][node.y + 1] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.y > 0 && m_nNodeChart[node.x][node.y - 1] == 0)
		{
			nodes.push(Node(node.x, node.y - 1));
			m_nNodeChart[node.x][node.y - 1] = m_nNodeChart[node.x][node.y] + 1;
		}
	}

	return false;
}

bool Player::PlacementCheck(SGD::Point mouse)
{
	if (Blockable(mouse)
		&& WorldManager::GetInstance()->IsSolidAtPosition((int)mouse.x, (int)mouse.y) == false
		&& m_pEntityManager->CheckCollision({ mouse.x * GRIDWIDTH, mouse.y * GRIDHEIGHT, mouse.x * GRIDWIDTH + GRIDWIDTH, mouse.y * GRIDHEIGHT + GRIDHEIGHT }) == false
		&& CheckLegalPlacement(Node((int)(m_ptPosition.x + 16) / GRIDWIDTH, (int)(m_ptPosition.y + 16) / GRIDHEIGHT), Node((int)mouse.x, (int)mouse.y)))
	{
		return true;
	}
	else 
		return false;
}
