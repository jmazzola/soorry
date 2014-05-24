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

class Shop
{

public:
	// Contructor and Deconstructor
	Shop() = default;
	virtual ~Shop() = default;

	// Enter
	void Enter();

	// Exit
	void Exit();

	// Render
	void Render();

	// Update
	void Update(float dt);

	// Input
	bool Input();

	// Load Prices
	void LoadPrices(string xmlFileName);

	// Price enumeration
	enum ItemPrices { WALL, WINDOW, BEARTRAP, MINE, GRENADE, AMMO, TOTAL_ITEM_PRICES };
	enum UpgradePrices{ SHOTGUN, AR, LAUNCHER, GRENADEUPGRADE, FIREAXE, TOTAL_UPGRADE_PRICES};

	// Accessors
	bool IsOpen();

	// Mutators
	void SetShopStatus(bool isOpen);


private:

	// Data members
	
	// Is the shop open?
	bool m_bIsOpen;

	// Prices of the items
	unsigned int itemPrices[TOTAL_ITEM_PRICES];

	// Prices of the upgrades
	unsigned int upgradePrices[TOTAL_UPGRADE_PRICES];

	// Textures
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;

};

