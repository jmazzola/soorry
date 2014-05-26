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
#include "ZombieFactory.h"
#include "AIComponent.h"

#include "../SGD Wrappers/SGD_Listener.h"
#include "EntityManager.h"
/**********************************************************/
// Forward Declarations
class Weapon;
class Inventory;
class Cursor;

class Player : public Entity, public SGD::Listener
{
public:

	Player();
	~Player();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	bool Blockable(SGD::Point mouse);
	virtual void HandleEvent(const SGD::Event* pEvent);
	bool PlacementCheck(SGD::Point mouse);
	/**********************************************************/
	// Accessors
	float GetMaxHealth() const;
	float GetCurrHealth() const;
	int GetCurrWeapon() const;
	int GetCurrPowerup() const;
	int GetCurrPlaceable() const;
	unsigned int GetScore() const;
	unsigned int GetEnemiesKilled() const;
	float GetSpeed() const;
	float GetScoreMultiplier() const;
	float GetTimeAlive() const;
	Inventory* GetInventory() const;
	Weapon* GetWeapons() const;
	EntityManager* GetEntityManager() const { return m_pEntityManager; }
	/**********************************************************/
	// Mutators
	void SetMaxHealth(float maxHealth);
	void SetCurrHealth(float currHealth);
	void SetCurrWeapon(int currWeapon);
	void SetCurrPowerup(int currPowerup);
	void SetCurrPlaceable(int currPlaceable);
	void SetScore(unsigned int score);
	void SetEnemiesKilled(unsigned int enemiesKilled);
	void SetSpeed(float speed);
	void SetScoreMultiplier(float multiplier);
	void SetTimeAlive(float timeAlive);
	void SetInventory(Inventory* inventory);
	void SetWeapons(Weapon* weapons);
	void SetZombieFactory(ZombieFactory* wave)  { m_pZombieWave = wave; }
	void SetEntityManager(EntityManager* manager) { m_pEntityManager = manager; }
protected:

	/**********************************************************/
	// Members
	float m_fShotTimer;
	float m_fPlaceTimer;
	float m_nMaxHealth;
	float m_nCurrHealth;
	int m_nCurrWeapon = 0;
	int m_nCurrPowerup;
	int m_nCurrPlaceable;
	unsigned int m_unScore;
	unsigned int m_unEnemiesKilled;
	float m_fSpeed;
	float m_fScoreMultiplier;
	float m_fTimeAlive;
	Inventory* m_pInventory;
	Weapon* m_pWeapons;
	ZombieFactory* m_pZombieWave;
	EntityManager* m_pEntityManager;

private:

	int** m_nNodeChart;
	bool CheckLegalPlacement(Node end, Node block);
};

