/***************************************************************
|	File:		Player.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	Player class stores the data members for 
|				the main character/controllable character
***************************************************************/

#pragma once

#include "Entity.h"
#include "AnimationManager.h"
#include "AnimationID.h"
/**********************************************************/
// Forward Declarations
class Weapon;
class Inventory;
class Cursor;

class Player : public Entity
{
public:

	Player();
	~Player();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	/**********************************************************/
	// Accessors
	int GetMaxHealth() const;
	int GetCurrHealth() const;
	int GetCurrWeapon() const;
	int GetCurrPowerup() const;
	int GetCurrPlaceable() const;
	unsigned int GetScore() const;
	unsigned int GetEnemiesKilled() const;
	float GetSpeed() const;
	float GetScoreMultiplier() const;
	float GetTimeAlive() const;
	Inventory* GetInventory() const;
	Cursor* GetCursor() const;
	Weapon* GetWeapons() const;

	/**********************************************************/
	// Mutators
	void SetMaxHealth(int maxHealth);
	void SetCurrHealth(int currHealth);
	void SetCurrWeapon(int currWeapon);
	void SetCurrPowerup(int currPowerup);
	void SetCurrPlaceable(int currPlaceable);
	void SetScore(unsigned int score);
	void SetEnemiesKilled(unsigned int enemiesKilled);
	void SetSpeed(float speed);
	void SetScoreMultiplier(float multiplier);
	void SetTimeAlive(float timeAlive);
	void SetInventory(Inventory* inventory);
	void SetCursor(Cursor* cursor);
	void SetWeapons(Weapon* weapons);

protected:

	/**********************************************************/
	// Members
	float m_fShotTimer;
	int m_nMaxHealth;
	int m_nCurrHealth;
	int m_nCurrWeapon;
	int m_nCurrPowerup;
	int m_nCurrPlaceable;
	unsigned int m_unScore;
	unsigned int m_unEnemiesKilled;
	float m_fSpeed;
	float m_fScoreMultiplier;
	float m_fTimeAlive;
	Inventory* m_pInventory;
	Cursor* m_pCursor;
	Weapon* m_pWeapons;
	AnimationTimestamp m_antsAnimation;
	AnimationID m_CurrAnimation;
};

