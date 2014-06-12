/***************************************************************
|	File:		Shop.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle the shop, it's prices and 
|				functionality
***************************************************************/

#pragma once

#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio

#include <string>
using namespace std;

class BitmapFont;
class Inventory;
class Entity;

class Shop
{

public:
	// Contructor and Deconstructor
	Shop() = default;
	virtual ~Shop() = default;

	// Accessors
	int GetShotgunDamage() { return m_nShotgunDamage; }
	int GetARDamage() { return m_nARDamage; }
	int GetRLDamage() { return m_nRLDamage; }
	int GetHTDamage() { return m_nHattrickDamage; }

	// Mutators
	void SetShotgunDamage(int dmg) { m_nShotgunDamage = dmg; }
	void SetARDamage(int dmg) { m_nARDamage = dmg; }
	void SetRLDamage(int dmg) { m_nRLDamage = dmg; }
	void SetHTDamage(int dmg) { m_nHattrickDamage = dmg; }

	// Enter
	void Enter(Entity* player);

	// Exit
	void Exit();

	// Render
	void Render();

	// Input
	bool Input();

	// Buy
	bool Buy(int parcel, int shopSection);

	// Sell
	void Sell(int parcel, int shopSection);

	// GivePurchase
	void GivePurchase(int parcel, int shopSection);

	// Load Prices
	void LoadPrices(string xmlFileName);

	// -- Price enumeration --

	// Item prices
	enum ItemPrices 
	{
		ITEM_PRICE_WALL, 
		ITEM_PRICE_WINDOW, 
		BEARTRAP,
		MINE, 
		GRENADE, 
		AMMO, 
		ITEMGB, 
		TOTAL_ITEMS
	};

	// Upgrade prices
	enum UpgradePrices
	{ 
		SHOTGUN_ROF, 
		SHOTGUN_DMG,
		SHOTGUN_MAXAMMO,
		AR_ROF, 
		AR_DMG, 
		AR_MAXAMMO,
		RL_ROF, 
		RL_DMG,
		RL_MAXAMMO,
		TRICKSHOT, 
		UPGRADE_GOBACK, 
		TOTAL_UPGRADES
	};

	// Tower prices
	enum TowerPrices
	{ 
		MG, 
		MAPLE_SYRUP, 
		HOCKEY_STICK, 
		LASER, 
		TOWERGB, 
		TOTAL_TOWERS
	};


	// Accessors
	bool IsOpen();
	unsigned int GetTowerPrice(int tower);

	// Mutators
	void SetShopStatus(bool isOpen);


private:

	// Data members
	
	// Is the shop open?
	bool m_bIsOpen;

	// --- Items ---
	string itemNames[TOTAL_ITEMS];
	string itemDescs[TOTAL_ITEMS];
	unsigned int itemPrices[TOTAL_ITEMS];
	unsigned int itemAmountToAdd[TOTAL_ITEMS];

	// --- Weapon Upgrades ---
	string upgradeNames[TOTAL_UPGRADES];
	string upgradeDescs[TOTAL_UPGRADES];
	unsigned int upgradePrices[TOTAL_UPGRADES];
	unsigned int upgradeAmountToAdd[TOTAL_UPGRADES];


	// --- Towers ---
	string towerNames[TOTAL_TOWERS];
	string towerDescs[TOTAL_TOWERS];
	unsigned int towerPrices[TOTAL_TOWERS];
	unsigned int towerAmountToAdd[TOTAL_TOWERS];

	// Textures
	// - Backgrounds
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBackgroundMain = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTowerMain = SGD::INVALID_HANDLE;

	SGD::HTexture m_hShopItems;		// TODO: Make shop sheet

	//Audio
	SGD::HAudio m_hShopMusic;

	// Menu cursor
	int m_nCursor;
	int m_nMenuTab;

	// BitmapFont
	BitmapFont* m_pFont;

	// Player
	Entity* m_pPlayer;

	// Weapon Attributes
	int m_nShotgunDamage = 20;
	int m_nARDamage = 20;
	int m_nRLDamage = 150;
	int m_nHattrickDamage = 75;

	// Shop tabs
	enum ShopTabs 
	{ 
		MAIN_TAB, 
		ITEMS_TAB, 
		UPGRADES_TAB, 
		TOWERS_TAB 
	};

	// Shop's main tab options
	enum MainOptions 
	{ 
		OPTIONS_ITEMS, 
		OPTIONS_UPGRADES, 
		OPTIONS_TOWERS, 
		OPTIONS_EXITSHOP 
	};

	// Shop's items tab options
	enum ItemsOptions 
	{ 
		ITEM_WALL, 
		ITEM_WINDOW, 
		ITEM_BEARTRAP, 
		ITEM_MINE, 
		ITEM_GRENADE, 
		ITEM_AMMO, 
		ITEM_GOBACK 
	};

	// Shop's upgrade tab options
	enum UpgradesOptions
	{
		UG_SHOTGUN_ROF, 
		UG_SHOTGUN_DAMAGE,
		UG_SHOTGUN_AMMO,
		UG_AR_ROF, 
		UG_AR_DAMAGE, 
		UG_AR_AMMO,
		UG_LAUNCHER_ROF, 
		UG_LAUNCHER_DAMAGE,
		UG_LAUNCHER_AMMO,
		UG_HATTRICK, 
		UG_GOBACK 
	};

	// Shop's towers tab options
	enum TowersOptions 
	{ 
		TOWER_MG, 
		TOWER_MAPLESYRUP, 
		TOWER_HOCKEYSTICK, 
		TOWER_LASER, 
		TOWER_GOBACK
	};


};

