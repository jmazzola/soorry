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

	// GivePurchase
	void GivePurchase(int parcel, int shopSection);

	// Load Prices
	void LoadPrices(string xmlFileName);

	// Price enumeration
	enum ItemPrices { WALL, WINDOW, BEARTRAP, MINE, GRENADE, AMMO, TOTAL_ITEMS };
	enum UpgradePrices{ SHOTGUN, AR, LAUNCHER, GRENADEUPGRADE, FIREAXE, TOTAL_UPGRADES};

	// Accessors
	bool IsOpen();

	// Mutators
	void SetShopStatus(bool isOpen);


private:

	// Data members
	
	// Is the shop open?
	bool m_bIsOpen;

	// Prices of the items
	unsigned int itemPrices[TOTAL_ITEMS];

	// Prices of the upgrades
	unsigned int upgradePrices[TOTAL_UPGRADES];

	// Textures
	// - Backgrounds
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBackgroundMain = SGD::INVALID_HANDLE;

	// - Items
	SGD::HTexture m_hItem[TOTAL_ITEMS];
	SGD::HTexture m_hUpgrade[TOTAL_UPGRADES];

	// Menu cursor
	int m_nCursor;
	int m_nMenuTab;

	// BitmapFont
	BitmapFont* m_pFont;

	// Player
	Entity* m_pPlayer;

	// Shop tabs
	enum ShopTabs { MAIN_TAB, ITEMS_TAB, UPGRADES_TAB };

	// Shop's main tab options
	enum MainOptions { OPTIONS_ITEMS, OPTIONS_UPGRADES, OPTIONS_EXITSHOP };
	enum ItemsOptions { ITEM_WALL, ITEM_WINDOW, ITEM_BEARTRAP, ITEM_MINE, ITEM_GRENADE, ITEM_AMMO, ITEM_GOBACK };


};

