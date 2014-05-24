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
#include "Player.h"
#include "Inventory.h"
#include "Weapon.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "BitmapFont.h"

// Enter
void Shop::Enter(Entity* player)
{
	Game* pGame = Game::GetInstance();
	
	m_pPlayer = player;

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	m_hBackgroundMain = pGraphics->LoadTexture("resource/images/menus/ShopMainBG.png");
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/ShopBG.png");
	for (int i = 0; i < TOTAL_ITEMS; i++)
		m_hItem[i] = pGraphics->LoadTexture("resource/images/menus/picturePlacehold.png");

	for (int i = 0; i < TOTAL_UPGRADES; i++)
		m_hUpgrade[i] = pGraphics->LoadTexture("resource/images/menus/picturePlacehold.png");

	// Load the prices
	LoadPrices("resource/data/shop.xml");

	// Bitmap Font
	m_pFont = new BitmapFont;
	m_pFont->Initialize("resource/images/fonts/BitmapFont_Roboto_0.png", "resource/data/BitmapFont_Roboto.fnt");

	m_nCursor = 0;
	m_nMenuTab = MAIN_TAB;
}

// Exit
void Shop::Exit()
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	pGraphics->UnloadTexture(m_hBackground);
	pGraphics->UnloadTexture(m_hBackgroundMain);

	for (int i = 0; i < TOTAL_ITEMS; i++)
		pGraphics->UnloadTexture(m_hItem[i]);

	for (int i = 0; i < TOTAL_UPGRADES; i++)
		pGraphics->UnloadTexture(m_hUpgrade[i]);

	m_nMenuTab = MAIN_TAB;

	// Terminate and clean up bitmapfont
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;
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

	// Main Tab
	if (m_nMenuTab == MAIN_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;

			// Wrap around the options
			if (m_nCursor > MainOptions::OPTIONS_EXITSHOP)
				m_nCursor = MainOptions::OPTIONS_ITEMS;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < MainOptions::OPTIONS_ITEMS)
				m_nCursor = MainOptions::OPTIONS_EXITSHOP;
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			// Switch table for the item selected
			switch (m_nCursor)
			{

			case MainOptions::OPTIONS_ITEMS:
				m_nCursor = 0;
				m_nMenuTab = ITEMS_TAB;
				break;

			case MainOptions::OPTIONS_UPGRADES:
				m_nCursor = 0;
				m_nMenuTab = UPGRADES_TAB;
				break;

			case MainOptions::OPTIONS_EXITSHOP:
				m_nCursor = 0;
				m_nMenuTab = MAIN_TAB;
				SetShopStatus(false);
				break;
			}
		}
	}
	// Items Tab
	else if (m_nMenuTab == ITEMS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;

			// Wrap around the options
			if (m_nCursor > ItemsOptions::ITEM_GOBACK)
				m_nCursor = ItemsOptions::ITEM_WALL;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < ItemsOptions::ITEM_WALL)
				m_nCursor = ItemsOptions::ITEM_GOBACK;
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			// If we're going back
			if (m_nCursor == ItemsOptions::ITEM_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_ITEMS;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 0))
					// Give the item
					GivePurchase(m_nCursor, 0);
			}
		}
	}

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
		if (m_nMenuTab == MAIN_TAB)
		{
			// Draw Main Tab background
			pGraphics->DrawTexture(m_hBackgroundMain, { 0, 0 });

			// Draw options
			if (m_nCursor == MainOptions::OPTIONS_ITEMS)
				m_pFont->Draw("Buy Items", 200, 200, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Buy Items", 200, 200, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_UPGRADES)
				m_pFont->Draw("Buy Upgrades", 200, 290, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Buy Upgrades", 200, 290, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_EXITSHOP)
				m_pFont->Draw("Exit Shop", 200, 380, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Exit Shop", 200, 380, 1, {0, 0, 0});
		}
		else if (m_nMenuTab == ITEMS_TAB)
		{
			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the selected image
			pGraphics->DrawTextureSection(m_hItem[m_nCursor], { 418, 57 }, { 0, 0, 111, 111 });

			// TODO: Make the descriptions, images, AND prices be in shop.xml
			switch (m_nCursor)
			{
				case ItemsOptions::ITEM_WALL:
					m_pFont->Draw("Walls.", 416, 208, 0.8f, { 0, 0, 0 });
					break;

				case ItemsOptions::ITEM_WINDOW:
					m_pFont->Draw("Windows", 416, 208, 0.8f, { 0, 0, 0 });
					break;

				case ItemsOptions::ITEM_BEARTRAP:
					m_pFont->Draw("Beartrap.", 416, 208, 0.8f, { 0, 0, 0 });
					break;

				case ItemsOptions::ITEM_MINE:
					m_pFont->Draw("Mine.", 416, 208, 0.8f, { 0, 0, 0 });
					break;

				case ItemsOptions::ITEM_GRENADE:
					m_pFont->Draw("Shhh-Poom.", 416, 208, 0.8f, { 0, 0, 0 });
					break;

				case ItemsOptions::ITEM_AMMO:
					m_pFont->Draw("*chamber sound*", 416, 208, 0.8f, { 0, 0, 0 });
					break;
				case ItemsOptions::ITEM_GOBACK:
					m_pFont->Draw("Go Back", 416, 208, 0.8f, { 0, 0, 0 });
					break;
			}

			// Draw the highlighted option
			if (m_nCursor == ItemsOptions::ITEM_WALL)
				m_pFont->Draw("50x Walls\t100", 55, 70, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("50x Walls\t100", 55, 70, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_WINDOW)
				m_pFont->Draw("50x Windows\t200", 55, 110, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("50x Windows\t200", 55, 110, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_BEARTRAP)
				m_pFont->Draw("1x Beartrap\t250", 55, 150, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("1x Beartrap\t250", 55, 150, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_MINE)
				m_pFont->Draw("1x AT-Mine\t300", 55, 190, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("1x AT-Mine\t300", 55, 190, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_GRENADE)
				m_pFont->Draw("3x Grenades\t350", 55, 230, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("3x Grenades\t350", 55, 230, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_AMMO)
				m_pFont->Draw("Refill Ammo\t500", 55, 270, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("Refill Ammo\t500", 55, 270, 0.7f, { 0, 0, 0 });

			if (m_nCursor == ItemsOptions::ITEM_GOBACK)
				m_pFont->Draw("Go Back", 55, 310, 0.7f, { 0, 255, 0 });
			else
				m_pFont->Draw("Go Back", 55, 310, 0.7f, { 0, 0, 0 });
		}
	}
}

// Buy
// - check if the player has enough money
// [in] parcel - the number of the item
// [in] shopSection - 0 (Items) 1 (Upgrades)
// [out] return true - has sufficient funds
// [out] return false - has insufficient funds
bool Shop::Buy(int parcel, int shopSection)
{
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	
	switch (shopSection)
	{
		case 0:		// items
		{
			// If the player has the money for the item
			if (player->GetScore() >= itemPrices[parcel])
			{
				// Subtract money
				int curMoney = player->GetScore();
				player->SetScore(curMoney -= itemPrices[parcel]);
				return true;
			}
			else
				return false;
		}
			break;
		case 1:
		{
			// If the player has the money for the upgrade
			if (player->GetScore() >= upgradePrices[parcel])
			{
				int curMoney = player->GetScore();
				player->SetScore(curMoney -= upgradePrices[parcel]);
				return true;
			}
			else
				return false;
		}
	}

	return true;
}

// GivePurchase
// - give the player the item they purchased
// [in] parcel - the item they want
// [in] shopSection - 0 (Items) 1 (Upgrades)
void Shop::GivePurchase(int parcel, int shopSection)
{
	// Grab the player's inventory
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	Inventory* inventory = player->GetInventory();

	// Grab the player's weapons (4)
	Weapon* weapons = player->GetWeapons();


	int items[TOTAL_ITEMS] = { WALL, WINDOW, BEARTRAP, MINE, GRENADE, AMMO };
	int upgrades[TOTAL_UPGRADES] = { SHOTGUN, AR, LAUNCHER, GRENADEUPGRADE, FIREAXE };

	// If we're in the items
	if (shopSection == 0)
	{
		if (parcel == WALL)
			inventory->SetWalls(inventory->GetWalls() + 50);
		if (parcel == WINDOW)
			inventory->SetWindows(inventory->GetWindows() + 50);
		if (parcel == BEARTRAP)
			inventory->SetBearTraps(inventory->GetBearTraps() + 1);
		if (parcel == MINE)
			inventory->SetMines(inventory->GetMines() + 1);
		if (parcel == GRENADE)
			inventory->SetGrenads(inventory->GetGrenads() + 3);
		if (parcel == AMMO)
		{
			// Refill the player's ammo
			weapons[0].SetCurrAmmo(weapons[0].GetMaxAmmo());
			weapons[1].SetCurrAmmo(weapons[1].GetMaxAmmo());
			weapons[2].SetCurrAmmo(weapons[2].GetMaxAmmo());
			weapons[3].SetCurrAmmo(weapons[3].GetMaxAmmo());
			// Set the player's weapons/ammo
			player->SetWeapons(weapons);
		}

	}


	// Set the player's inventory
	player->SetInventory(inventory);


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