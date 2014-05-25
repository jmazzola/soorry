#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
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

#define WALLPICK 0
#define WINDOWPICK 1

Player::Player()
{
	// Entity
	m_ptPosition = { 50, 100 };
	m_vtVelocity = { 0, 0 };


	// Animation/ Image
	m_pSprite = AnimationManager::GetInstance()->GetSprite("running");
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "running";

	// Player's variables
	m_nMaxHealth = 100;
	m_nCurrHealth = 100;
	m_nCurrWeapon = 0;
	m_nCurrPowerup = -1;
	m_nCurrPlaceable = -1;
	m_unScore = 0;
	m_unEnemiesKilled = 0;
	m_fSpeed = 250.0f;
	m_fScoreMultiplier = 0.0f;
	m_fTimeAlive = 0.0f;

	// Player Inventory
	m_pInventory = new Inventory();
	m_pInventory->SetBearTraps(100);
	m_pInventory->SetGrenads(0);
	m_pInventory->SetHealthPacks(0);
	m_pInventory->SetMines(1);
	m_pInventory->SetWalls(100);
	m_pInventory->SetWindows(100);

	//NOTE: Do I initialize this here? was this created right?
	m_pWeapons = new Weapon[4];
#pragma region Load Weapons


	//Assault rifle
	//NOTE: totally made up
	Weapon tempWeapon;
	tempWeapon.SetCurrAmmo(30);
	//NOTE: totally made up
	tempWeapon.SetMaxAmmo(500);
	tempWeapon.SetFireRate(.2f);
	tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	m_pWeapons[0] = tempWeapon;

	//Shotgun
	//NOTE: totally made up
	tempWeapon;
	tempWeapon.SetCurrAmmo(10);
	//NOTE: totally made up
	tempWeapon.SetMaxAmmo(500);
	tempWeapon.SetFireRate(.5f);
	tempWeapon.SetType(Guns::TYPE_SHOTGUN);
	m_pWeapons[1] = tempWeapon;

	//rocket launcher
	//NOTE: totally made up
	tempWeapon;
	tempWeapon.SetCurrAmmo(5);
	//NOTE: totally made up
	tempWeapon.SetMaxAmmo(50);
	tempWeapon.SetFireRate(2);
	tempWeapon.SetType(Guns::TYPE_SHOTGUN);
	m_pWeapons[2] = tempWeapon;

	//Fire Axe
	//NOTE: totally made up
	tempWeapon;
	tempWeapon.SetCurrAmmo(0);
	//NOTE: totally made up
	tempWeapon.SetMaxAmmo(0);
	tempWeapon.SetFireRate(.5f);
	tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	m_pWeapons[3] = tempWeapon;
#pragma endregion



	//is three appropriate?
	switch (3)
	{
		//NOTE:Will it hit this?
	case 0: //Assault rifle
	{
				//NOTE: totally made up
				Weapon tempWeapon = m_pWeapons[0];
				tempWeapon.SetCurrAmmo(30);
				//NOTE: totally made up
				tempWeapon.SetMaxAmmo(500);
				tempWeapon.SetFireRate(.5);
				tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	}
		break;
	case 1://Shotgun
	{
			   //NOTE: totally made up
			   Weapon tempWeapon = m_pWeapons[1];
			   tempWeapon.SetCurrAmmo(10);
			   //NOTE: totally made up
			   tempWeapon.SetMaxAmmo(500);
			   tempWeapon.SetFireRate(1);
			   tempWeapon.SetType(Guns::TYPE_SHOTGUN);

	}
		break;
	case 2://rocket launcher
	{
			   //NOTE: totally made up
			   Weapon tempWeapon = m_pWeapons[2];
			   tempWeapon.SetCurrAmmo(5);
			   //NOTE: totally made up
			   tempWeapon.SetMaxAmmo(50);
			   tempWeapon.SetFireRate(4);
			   tempWeapon.SetType(Guns::TYPE_SHOTGUN);
	}
		break;
	case 3:
	{
			  //NOTE: totally made up
			  Weapon tempWeapon = m_pWeapons[3];
			  tempWeapon.SetCurrAmmo(0);
			  //NOTE: totally made up
			  tempWeapon.SetMaxAmmo(0);
			  tempWeapon.SetFireRate(1);
			  tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
	}
		break;
	}

}


Player::~Player()
{
	delete[]m_pWeapons;
		delete m_pInventory;
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

	// Set camera
	Camera::x = (int)m_ptPosition.x - 384;
	Camera::y = (int)m_ptPosition.y - 284;

	//Update Timers

	// Input
	if (pInput->IsKeyDown(SGD::Key::A) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x -= m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true)
			m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::D) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x += m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true)
			m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::W) == true)
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y -= m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true)
			m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::S) == true)
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y += m_fSpeed * dt;

		if (pWorld->CheckCollision(this) == true)
			m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	//GAH Weapons! - Arnold
	if (pInput->IsKeyPressed(SGD::Key::One) == true)
	{
		m_nCurrWeapon = 0;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Two) == true)
	{
		m_nCurrWeapon = 1;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Three) == true)
	{
		m_nCurrWeapon = 2;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (pInput->IsKeyPressed(SGD::Key::Four) == true)
	{
		m_nCurrWeapon = 3;
		m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
	}
	if (m_fShotTimer < 0 && m_pWeapons[m_nCurrWeapon].GetCurrAmmo() > 0)
	{
		if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
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
	// Selecting Bear Trap
	if (pInput->IsKeyPressed(SGD::Key::Nine) == true)
		m_nCurrPlaceable = 0;
	// Selecting Mine
	if (pInput->IsKeyPressed(SGD::Key::Zero) == true)
		m_nCurrPlaceable = 1;

	if (pInput->IsKeyPressed(SGD::Key::O) == true)
		m_nCurrPlaceable = 2;
	if (pInput->IsKeyPressed(SGD::Key::P) == true)
		m_nCurrPlaceable = 3;

	if (pInput->IsKeyPressed(SGD::Key::MouseRight) == true)
	{
		m_ptPosition.x += 30;
		 //Colliding with wall
		if (pWorld->CheckCollisionID(this) == WALL)
		{
			SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			pos.x = ((pos.x - (int)pos.x % 32) + Camera::x) / 32;
			pos.y = ((pos.y - (int)pos.y % 32) + Camera::y) / 32;

			pWorld->SetColliderID(pos.x, pos.y, EMPTY);
			CreatePickupMessage*  pmsg = new CreatePickupMessage(WALLPICK, m_ptPosition);
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
		else if (pWorld->CheckCollisionID(this) == WINDOW)
		{
			SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			pos.x = ((pos.x - (int)pos.x % 32) + Camera::x) / 32;
			pos.y = ((pos.y - (int)pos.y % 32) + Camera::y) / 32;

			pWorld->SetColliderID(pos.x, pos.y, EMPTY);
			CreatePickupMessage*  pmsg = new CreatePickupMessage(WINDOWPICK, m_ptPosition);
			pmsg->QueueMessage();
			pmsg = nullptr;
		}
		m_ptPosition.x -= 30;

	}

	if (m_pZombieWave.IsBuildMode() == true)
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
		// Send a Message to Create a bear trap if the player has any
		if (m_nCurrPlaceable != -1)
		{
			if (m_nCurrPlaceable == 0 && m_pInventory->GetBearTraps() > 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true && m_fPlaceTimer <= 0)
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
			// Send a Message to Create a mine if the player has any
			else if (m_nCurrPlaceable == 1 && m_pInventory->GetMines() > 0 && m_fPlaceTimer <= 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
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
			else if (m_nCurrPlaceable == 2 && m_pInventory->GetWalls() > 0 && m_fPlaceTimer <= 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
				{
					m_fPlaceTimer = 1;
					SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
					pos.x = ((pos.x - (int)pos.x % 32) + Camera::x) / 32;
					pos.y = ((pos.y - (int)pos.y % 32) + Camera::y) / 32;

					pWorld->SetColliderID((int)pos.x, (int)pos.y, WALL);
					// Decreasing the amount of mines left for the player
					unsigned int newset = m_pInventory->GetWalls();
					--newset;
					m_pInventory->SetWalls(newset);
				}
			}
			else if (m_nCurrPlaceable == 3 && m_pInventory->GetWindows() > 0 && m_fPlaceTimer <= 0)
			{
				if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
				{
					m_fPlaceTimer = 1;
					SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
					pos.x = (pos.x - (int)pos.x % 32) + Camera::x;
					pos.y = (pos.y - (int)pos.y % 32) + Camera::y;

					pWorld->SetColliderID((int)pos.x / 32, (int)pos.y / 32, WINDOW);
					// Decreasing the amount of mines left for the player
					unsigned int newset = m_pInventory->GetWindows();
					--newset;
					m_pInventory->SetWindows(newset);
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

/**********************************************************/
// Accessors

int Player::GetMaxHealth() const
{
	return m_nMaxHealth;
}

int Player::GetCurrHealth() const
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

void Player::SetMaxHealth(int _maxHealth)
{
	m_nMaxHealth = _maxHealth;
}

void Player::SetCurrHealth(int _currHealth)
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