#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "AnimationManager.h"
#include "Frame.h"
#include "Sprite.h"
#include "Game.h"
#include "AnimationID.h"
#include "WorldManager.h"

Player::Player()
{
	// Entity
	m_ptPosition = { 0, 20 };
	m_vtVelocity = { 0, 0 };


	// Animation/ Image
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_CurrAnimation.LoadPlayerRun(m_CurrAnimation.PlayerRun);
	m_antsAnimation.m_nCurrAnimation = m_CurrAnimation.PlayerRun;
	m_pSprite = AnimationManager::GetInstance()->GetSprite("running");

	// Player's variables
	m_nMaxHealth = 100;
	m_nCurrHealth = 100;
	m_nCurrWeapon = 0;
	m_nCurrPowerup = -1;
	m_nCurrPlaceable = -1;
	m_unScore = 0;
	m_unEnemiesKilled = 0;
	m_fSpeed = 5.0f;
	m_fScoreMultiplier = 0.0f;
	m_fTimeAlive = 0.0f;
	 //m_pInventory;
	 //m_pCursor;
	 //m_pWeapons;
}


Player::~Player()
{

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
	// Input

	if (pInput->IsKeyDown(SGD::Key::A) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x -= m_fSpeed * dt;

		//if (pWorld->CheckCollision(this) == false)
		//	m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::D) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x += m_fSpeed * dt;

		//if (pWorld->CheckCollision(this) == false)
		//	m_ptPosition.x = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::W) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.y -= m_fSpeed * dt;

		//if (pWorld->CheckCollision(this) == false)
		//	m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	if (pInput->IsKeyDown(SGD::Key::S) == true)
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.y += m_fSpeed * dt;

		//if (pWorld->CheckCollision(this) == false)
		//	m_ptPosition.y = oldpos;

		AnimationManager::GetInstance()->Update(m_antsAnimation, dt);
	}
	
}

/*********************************************************/
// Update
//	- Draw the character
void Player::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Placeholder for the player
	//pGraphics->DrawRectangle({ 0, 0, m_ptPosition.x, m_ptPosition.y }, { 0, 255, 0 });
	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x, m_ptPosition.y);
}

SGD::Rectangle Player::GetRect() const
{
	return SGD::Rectangle();
}

int Player::GetType() const
{
	return ENT_PLAYER;
}

void Player::HandleCollision(const IEntity* pOther)
{

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

Cursor* Player::GetCursor() const
{
	return m_pCursor;
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

void Player::SetCursor(Cursor* _cursor)
{
	m_pCursor = _cursor;
}

void Player::SetWeapons(Weapon* _weapons)
{
	m_pWeapons = _weapons;
}