#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}


/**********************************************************/
// Interface Methods

void Player::Update(float dt)
{

}

void Player::Render()
{

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

float Player::GetMultiplier() const
{
	return m_fMultiplier;
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

void Player::SetMultiplier(float _multiplier)
{
	m_fMultiplier = _multiplier;
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