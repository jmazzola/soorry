/***************************************************************
|	File:		Shop.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle the shop, it's prices and
|				functionality
***************************************************************/

#include "Shop.h"
#include "../TinyXML/tinyxml.h"

#include "Game.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

// Enter
void Shop::Enter()
{
	Game* pGame = Game::GetInstance();

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/ShopBG.png");
}

// Exit
void Shop::Exit()
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hBackground);
}

// IsOpen
// - Is the shop open?
bool Shop::IsOpen()
{
	return m_bIsOpen;
}

// SetShopStatus
// - Set if the shop is open or not
void Shop::SetShopStatus(bool isOpen)
{
	m_bIsOpen = isOpen;
}

// Input
//	- handle player input
bool Shop::Input()
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	return true;
}

// Render
//	- draw the shop and it's items
void Shop::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	
	// If it's open
	if (IsOpen())
	{
		pGraphics->DrawTexture(m_hBackground, { 0, 0 });
	}
}

// Update
// - update the shop's items
void Shop::Update(float dt)
{

}

// LoadPrices
// - load in the prices from an xml file
// [in] xmlFileName - the xml file to load the prices from
void Shop::LoadPrices(string xmlFileName)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load the file, if not gtfo
	if (!doc.LoadFile(xmlFileName.c_str()))
		return;

	// Access the root element (shop)
	TiXmlElement* pRoot = doc.RootElement();

	// If the root isn't there, if not gtfo
	if (pRoot == nullptr)
		return;

	// Get the element for the item prices
	TiXmlElement* pItems = pRoot->FirstChildElement("items");

	// Set the item prices to the array
	itemPrices[WALL] = atoi(pItems->Attribute("wall"));
	itemPrices[WINDOW] = atoi(pItems->Attribute("window"));
	itemPrices[BEARTRAP] = atoi(pItems->Attribute("beartrap"));
	itemPrices[MINE] = atoi(pItems->Attribute("mine"));
	itemPrices[GRENADE] = atoi(pItems->Attribute("grenade"));
	itemPrices[AMMO] = atoi(pItems->Attribute("ammo"));

	// Grab the element for the upgrade prices
	TiXmlElement* pUpgrades = pItems->NextSiblingElement("upgrades");

	// Set the upgrades prices to the array
	upgradePrices[SHOTGUN] = atoi(pUpgrades->Attribute("shotgun"));
	upgradePrices[AR] = atoi(pUpgrades->Attribute("ar"));
	upgradePrices[LAUNCHER] = atoi(pUpgrades->Attribute("rocketlauncher"));
	upgradePrices[GRENADEUPGRADE] = atoi(pUpgrades->Attribute("grenade"));
	upgradePrices[FIREAXE] = atoi(pUpgrades->Attribute("fireaxe"));
}