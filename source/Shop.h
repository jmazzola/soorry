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

	// Render
	void Render();

	// Update
	void Update(float dt);

	// Load Prices
	void LoadPrices(string xmlFileName);

	// Price enumeration
	enum Prices { WALL, WINDOW, BEARTRAP, MINE, GRENADE, AMMO, TOTAL_PRICES };


private:

	// Data members
	
	// Is the shop open?
	bool m_bIsOpen;

	// Prices of the items
	unsigned int prices[TOTAL_PRICES];

	// Textures
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;

};

