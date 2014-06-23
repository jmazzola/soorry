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

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "EntityManager.h"
/**********************************************************/
// Forward Declarations
class Weapon;
class Inventory;
class Tower;

class Player : public Entity, public SGD::Listener
{
public:
	enum Placeables{ WALLS , WINDOWS , BEARTRAP , MINE , MGTOWER , MSTOWER , HSTOWER , LTOWER, LTRAP, STRAP, NUMPLACEABLES, };
	enum Weapons{ MACHINE_GUN, SHOT_GUN, ROCKET_LAUNCHER, TRICK_SHOT_GUN, TOTAL_GUNS, };

	Player();
	~Player();

	float m_fDroneRotation = 0;
	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render () override;
	virtual void PostRender() override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	bool Blockable(SGD::Point mouse);
	virtual void HandleEvent(const SGD::Event* pEvent);
	bool PlacementCheck(SGD::Point mouse, bool isPassable = false);
	void WeaponSwitch();
	/**********************************************************/
	// Accessors
	float GetMaxHealth() const;
	float GetCurrHealth() const;
	float GetSuperTimer() const;
	float GetSuperLength() const;
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
	bool IsRunningMan(void) const;

	bool HasAR() const { return m_bHasAssaultRifle; }
	bool HasShotty() const { return m_bHasShotty; }
	bool HasRocketLauncher() const { return m_bHasRocketz; }
	bool HasHatTrick() const { return m_bHasHatTrick; }
	float GetRotation() const { return m_fRotation; }
	/**********************************************************/
	// Mutators
	void SetMaxHealth(float maxHealth);
	void SetCurrHealth(float currHealth);
	void SetSuperTimer(float timer);
	void SetSuperLength(float timer);
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
	void SetSelectedTower(Tower* tower);
	void SetPlaceablesImage(SGD::HTexture placeablesImage);
	void SetRangeCirclesImage(SGD::HTexture rangeCirclesImage);
	void SetRunningMan(bool yes);

	void SetAR(bool ar) { m_bHasAssaultRifle = ar; }
	void SetShotgun(bool sh) { m_bHasShotty = sh; }
	void SetRocketLauncher(bool r) { m_bHasRocketz = r; }
	void SetHatTrick(bool h) { m_bHasHatTrick = h; }

	float m_fEscapeTimer;
protected:

	/**********************************************************/
	// Members
	bool m_bCanLeftClick;
	bool m_bCanLeftTrigger;
	bool m_bCanRightTrigger;
	float m_fGrenadeTimer;
	float m_fPlaceTimer;
	float m_nMaxHealth;
	float m_nCurrHealth;
	float m_fSuperTimer;
	float m_fSuperLength;
	float m_fCameraLerpTimer;
	int m_nCurrWeapon = 0;
	int m_nCurrPowerup;
	int m_nCurrPlaceable;
	float m_fPickupMessageTimer;
	std::string m_sPickupMessage;
	unsigned int m_unScore;
	unsigned int m_unEnemiesKilled;
	float m_fSpeed;
	float m_fScoreMultiplier;
	float m_fTimeAlive;
	Inventory* m_pInventory;
	Weapon* m_pWeapons;
	ZombieFactory* m_pZombieWave;
	EntityManager* m_pEntityManager;
	Tower* m_pSelectedTower;
	SGD::Vector m_vCameraStart;
	SGD::Vector m_vCameraEnd;
	SGD::Vector m_vCamera;
	SGD::HTexture m_hPlaceablesImage;
	SGD::HTexture m_hRangeCirclesImage;
	float m_fRotation;
	
	//Player sounds
	SGD::HAudio m_hBlockPlace = SGD::INVALID_HANDLE;
	SGD::HAudio m_hBlockBreak = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPickup = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWalking = SGD::INVALID_HANDLE;
	SGD::HAudio m_hGunClick	= SGD::INVALID_HANDLE;
	SGD::HAudio m_hPlayerGrunt = SGD::INVALID_HANDLE;

private:
	

	int** m_nNodeChart;
	bool CheckLegalPlacement(Node end, Node block);
	bool m_bStaticCamera;
	
	// RunningMan
	bool isRunningMan;
	SGD::Point m_ptLastPos;
	float m_fRunningManTimer;

	// ArcadeMode
	bool m_bTHEBOOL;
	bool m_bAccept;
	SGD::Vector m_vtStick;

	// Are we near the shop?
	bool m_bIsNearShop;

	// Do we haz weapons?
	bool m_bHasAssaultRifle;
	bool m_bHasShotty;
	bool m_bHasRocketz;
	bool m_bHasHatTrick;

	float m_fCanBlood;

};

