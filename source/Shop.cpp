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
#include "GameplayState.h"
#include "StatTracker.h"
#include "Drone.h"
#include "CreateDroneMessage.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "BitmapFont.h"

#define SELL_DISCOUNT 0.75f
#define BUY_INCREASE 0.75f

#define AR_CURAMMO 200
#define SH_CURAMMO 200
#define RL_CURAMMO 20
#define HT_CURAMMO 50


// Enter
void Shop::Enter(Entity* player)
{
	Game* pGame = Game::GetInstance();

	m_pPlayer = player;

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	m_hBackgroundMain = pGraphics->LoadTexture("resource/images/menus/ShopMainBG.png");
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/ShopBG.png");
	m_hTowerMain = pGraphics->LoadTexture("resource/images/menus/ShopTowersBG.png");
	m_hShopItems = pGraphics->LoadTexture("resource/images/menus/shopsheet.png");

	// Load the audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	/*if (pAudio->IsAudioPlaying(m_hShopMusic) == false)
	{
	pAudio->PlayAudio(m_hShopMusic, true);
	}
	if (pAudio->IsAudioPlaying(m_hWelcome) == false)
	{
	pAudio->PlayAudio(m_hWelcome);
	}*/
	// Bitmap Font
	m_pFont = new BitmapFont;
	m_pFont->Initialize("resource/images/fonts/BitmapFont_Roboto_0.png", "resource/data/BitmapFont_Roboto.fnt");

	m_nCursor = 0;
	m_nMenuTab = MAIN_TAB;

	menuRectangles[MainOptions::OPTIONS_WEAPONS] = SGD::Rectangle(SGD::Point(200, 150), SGD::Size(415, 50));
	menuRectangles[MainOptions::OPTIONS_ITEMS] = SGD::Rectangle(SGD::Point(200, 200), SGD::Size(415, 50));
	menuRectangles[MainOptions::OPTIONS_UPGRADES] = SGD::Rectangle(SGD::Point(200, 250), SGD::Size(415, 50));
	menuRectangles[MainOptions::OPTIONS_FORTIFICATIONS] = SGD::Rectangle(SGD::Point(200, 300), SGD::Size(415, 50));
	menuRectangles[MainOptions::OPTIONS_EXITSHOP] = SGD::Rectangle(SGD::Point(200, 350), SGD::Size(415, 50));

	weaponRectangles[WEAP_AR] = SGD::Rectangle(SGD::Point(55, 70), SGD::Size(310, 40));
	weaponRectangles[WEAP_SHOTGUN] = SGD::Rectangle(SGD::Point(55, 110), SGD::Size(310, 40));
	weaponRectangles[WEAP_ROCKETLAUNCHER] = SGD::Rectangle(SGD::Point(55, 150), SGD::Size(310, 40));
	weaponRectangles[WEAP_HATTRICK] = SGD::Rectangle(SGD::Point(55, 190), SGD::Size(310, 40));
	weaponRectangles[WEAP_GOBACK] = SGD::Rectangle(SGD::Point(55, 230), SGD::Size(310, 40));

	itemRectangles[ITEM_BEARTRAP] = SGD::Rectangle(SGD::Point(55, 70), SGD::Size(310, 40));
	itemRectangles[ITEM_MINE] = SGD::Rectangle(SGD::Point(55, 110), SGD::Size(310, 40));
	itemRectangles[ITEM_WALL] = SGD::Rectangle(SGD::Point(55, 150), SGD::Size(310, 40));
	itemRectangles[ITEM_WINDOW] = SGD::Rectangle(SGD::Point(55, 190), SGD::Size(310, 40));
	itemRectangles[ITEM_GRENADE] = SGD::Rectangle(SGD::Point(55, 230), SGD::Size(310, 40));
	itemRectangles[ITEM_AMMO] = SGD::Rectangle(SGD::Point(55, 270), SGD::Size(310, 40));
	itemRectangles[ITEM_DRONE] = SGD::Rectangle(SGD::Point(55, 310), SGD::Size(310, 40));
	itemRectangles[ITEM_GOBACK] = SGD::Rectangle(SGD::Point(55, 350), SGD::Size(310, 40));

	upgradeRectangles[UG_AR_DAMAGE] = SGD::Rectangle(SGD::Point(55, 70), SGD::Size(310, 40));
	upgradeRectangles[UG_AR_ROF] = SGD::Rectangle(SGD::Point(55, 110), SGD::Size(310, 40));
	upgradeRectangles[UG_AR_AMMO] = SGD::Rectangle(SGD::Point(55, 150), SGD::Size(310, 40));
	upgradeRectangles[UG_SHOTGUN_DAMAGE] = SGD::Rectangle(SGD::Point(55, 190), SGD::Size(310, 40));
	upgradeRectangles[UG_SHOTGUN_ROF] = SGD::Rectangle(SGD::Point(55, 230), SGD::Size(310, 40));
	upgradeRectangles[UG_SHOTGUN_AMMO] = SGD::Rectangle(SGD::Point(55, 270), SGD::Size(310, 40));
	upgradeRectangles[UG_LAUNCHER_DAMAGE] = SGD::Rectangle(SGD::Point(55, 310), SGD::Size(310, 40));
	upgradeRectangles[UG_LAUNCHER_ROF] = SGD::Rectangle(SGD::Point(55, 350), SGD::Size(310, 40));
	upgradeRectangles[UG_LAUNCHER_AMMO] = SGD::Rectangle(SGD::Point(55, 390), SGD::Size(310, 40));
	upgradeRectangles[UG_GOBACK] = SGD::Rectangle(SGD::Point(55, 430), SGD::Size(310, 40));

	fortificationRectangles[FORT_MG] = SGD::Rectangle(SGD::Point(55, 70), SGD::Size(310, 40));
	fortificationRectangles[FORT_MAPLESYRUP] = SGD::Rectangle(SGD::Point(55, 110), SGD::Size(310, 40));
	fortificationRectangles[FORT_LASER] = SGD::Rectangle(SGD::Point(55, 150), SGD::Size(310, 40));
	fortificationRectangles[FORT_SPIKETRAP] = SGD::Rectangle(SGD::Point(55, 190), SGD::Size(310, 40));
	fortificationRectangles[FORT_LAVATRAP] = SGD::Rectangle(SGD::Point(55, 230), SGD::Size(310, 40));
	fortificationRectangles[FORT_HOCKEYSTICK] = SGD::Rectangle(SGD::Point(55, 270), SGD::Size(310, 40));
	fortificationRectangles[FORT_GOBACK] = SGD::Rectangle(SGD::Point(55, 310), SGD::Size(310, 40));
}
//// enter shop DELETE ME AFTER SHOP FUNCTIONS PROPERLY
//if (pInput->IsKeyPressed(SGD::Key::Backspace))
//{
//	pAudio->StopAudio(m_hBackgroundMus);
//	// to stop audio from playing after every backspace
//	
//	m_pShop->SetShopStatus(true);
//}
// Exit
void Shop::Exit()
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Unload background
	pGraphics->UnloadTexture(m_hBackground);
	pGraphics->UnloadTexture(m_hBackgroundMain);
	pGraphics->UnloadTexture(m_hTowerMain);
	pGraphics->UnloadTexture(m_hShopItems);

	m_nMenuTab = MAIN_TAB;

	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	//pAudio->StopAudio(m_hShopMusic);

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

unsigned int Shop::GetTowerPrice(int _tower)
{
	return fortPrices[_tower];
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
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

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
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
			
			// Wrap around the options
			if (m_nCursor > MainOptions::OPTIONS_EXITSHOP)
				m_nCursor = MainOptions::OPTIONS_WEAPONS;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor < MainOptions::OPTIONS_WEAPONS)
				m_nCursor = MainOptions::OPTIONS_EXITSHOP;
		}

		// Mouse Selection
		if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
		{
			for (int i = 0; i < OPTIONS_TOTAL; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(menuRectangles[i]))
				{
					if (i != m_nCursor)
					{
						m_nCursor = i;
						pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
					}
				}
			}
		}
		bool m_bTHEBOOL = false;
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			for (int i = 0; i < 5; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(menuRectangles[i]))
				{
					m_bTHEBOOL = true;
				}
			}
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A) || m_bTHEBOOL)
		{
			// Switch table for the item selected
			switch (m_nCursor)
			{

			case MainOptions::OPTIONS_WEAPONS:
				m_nCursor = 0;
				m_nMenuTab = WEAPONS_TAB;
				break;

			case MainOptions::OPTIONS_ITEMS:
				m_nCursor = 0;
				m_nMenuTab = ITEMS_TAB;
				break;

			case MainOptions::OPTIONS_UPGRADES:
				m_nCursor = 0;
				m_nMenuTab = UPGRADES_TAB;
				break;

			case MainOptions::OPTIONS_FORTIFICATIONS:
				m_nCursor = 0;
				m_nMenuTab = FORTIFICATIONS_TAB;
				break;

			case MainOptions::OPTIONS_EXITSHOP:
				m_nCursor = 0;
				m_nMenuTab = MAIN_TAB;
				SetShopStatus(false);
				
				dynamic_cast<Player*>(m_pPlayer)->SetDelay(1);
				dynamic_cast<Player*>(m_pPlayer)->hasClosedShop = true;
				break;
			}
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
		}
	}

	// << Weapons Tab >>
	else if (m_nMenuTab == WEAPONS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor > WeaponsOptions::WEAPON_GOBACK)
				m_nCursor = WeaponsOptions::WEAPON_AR;
		}

		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor < WeaponsOptions::WEAPON_AR)
				m_nCursor = WeaponsOptions::WEAPON_GOBACK;
		}

		// Mouse Selection
		if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
		{
			for (int i = 0; i < TOTAL_WEAPONS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(weaponRectangles[i]))
				{
					if (i != m_nCursor)
					{
						m_nCursor = i;
						pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
					}
				}
			}
		}
		bool m_bTHEBOOL = false;
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			for (int i = 0; i < TOTAL_WEAPONS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(weaponRectangles[i]))
				{
					m_bTHEBOOL = true;
				}
			}
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A) || m_bTHEBOOL)
		{
			// If we're going back
			if (m_nCursor == WeaponsOptions::WEAPON_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_WEAPONS;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
				pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 0))
					// Give the item
					GivePurchase(m_nCursor, 0);
				else
					pAudio->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
			}
		}

	}
	// << Items Tab >>
	else if (m_nMenuTab == ITEMS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor > ItemsOptions::ITEM_GOBACK)
				m_nCursor = ItemsOptions::ITEM_BEARTRAP;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor < ItemsOptions::ITEM_BEARTRAP)
				m_nCursor = ItemsOptions::ITEM_GOBACK;
		}

		// Mouse Selection
		if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
		{
			for (int i = 0; i < TOTAL_ITEMS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(itemRectangles[i]))
				{
					if (i != m_nCursor)
					{
						m_nCursor = i;
						pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
					}
				}
			}
		}
		bool m_bTHEBOOL = false;
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			for (int i = 0; i < TOTAL_ITEMS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(itemRectangles[i]))
				{
					m_bTHEBOOL = true;
				}
			}
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A) || m_bTHEBOOL)
		{
			// If we're going back
			if (m_nCursor == ItemsOptions::ITEM_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_ITEMS;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
				pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 1))
					// Give the item
					GivePurchase(m_nCursor, 1);
				else
					pAudio->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
			}
		}
	}
	// << Upgrades Tab >>
	else if (m_nMenuTab == UPGRADES_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor > UpgradesOptions::UG_GOBACK)
				m_nCursor = UpgradesOptions::UG_AR_DAMAGE;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor < UpgradesOptions::UG_AR_DAMAGE)
				m_nCursor = UpgradesOptions::UG_GOBACK;
		}

		// Mouse Selection
		if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
		{
			for (int i = 0; i < TOTAL_UPGRADES; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(upgradeRectangles[i]))
				{
					if (i != m_nCursor)
					{
						m_nCursor = i;
						pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
					}
				}
			}
		}
		bool m_bTHEBOOL = false;
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			for (int i = 0; i < TOTAL_UPGRADES; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(upgradeRectangles[i]))
				{
					m_bTHEBOOL = true;
				}
			}
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A) || m_bTHEBOOL)
		{
			// If we're going back
			if (m_nCursor == UpgradesOptions::UG_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_UPGRADES;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
				pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 2))
					// Give the item
					GivePurchase(m_nCursor, 2);
				else
					pAudio->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
			}
		}

	}
	// << Fortifications Tab >>
	else if (m_nMenuTab == FORTIFICATIONS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor > FortificationOptions::FORT_GOBACK)
				m_nCursor = FortificationOptions::FORT_MG;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;
			pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());

			// Wrap around the options
			if (m_nCursor < FortificationOptions::FORT_MG)
				m_nCursor = FortificationOptions::FORT_GOBACK;
		}

		// Mouse Selection
		if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
		{
			for (int i = 0; i < TOTAL_TOWERS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(fortificationRectangles[i]))
				{
					if (i != m_nCursor)
					{
						m_nCursor = i;
						pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
					}
				}
			}
		}
		bool m_bTHEBOOL = false;
		if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			for (int i = 0; i < TOTAL_TOWERS; i++)
			{
				if (pInput->GetMousePosition().IsWithinRectangle(fortificationRectangles[i]))
				{
					m_bTHEBOOL = true;
				}
			}
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A) || m_bTHEBOOL)
		{
			// If we're going back
			if (m_nCursor == FortificationOptions::FORT_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_FORTIFICATIONS;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
				pAudio->PlayAudio(m_pTowerFlyweight->GetClickSound());
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 3))
					// Give the item
					GivePurchase(m_nCursor, 3);
				else
					pAudio->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
			}
		}
		// Sell a fortification
		if (pInput->IsKeyPressed(SGD::Key::Backspace) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::X))
			Sell(m_nCursor, 3);
	}
	return true;
}

// UpdateItemStatus
// - update if an item is maxed or bought
// - change the item's name
void Shop::UpdateItemStatus()
{
	// Grab the player
	Player* player = dynamic_cast<Player*>(m_pPlayer);


	// Update Weapon Names if they're bought
	(player->HasAR()) ? weapNames[WEAP_AR] = "Assault Rifle [OWNED]" : weapNames[WEAP_AR] = "Assault Rifle";
	isWeapBought[WEAP_AR] = (player->HasAR());

	(player->HasShotty()) ? weapNames[WEAP_SHOTGUN] = "Shotgun [OWNED]" : weapNames[WEAP_SHOTGUN] = "Shotgun";
	isWeapBought[WEAP_SHOTGUN] = (player->HasShotty());

	(player->HasRocketLauncher()) ? weapNames[WEAP_ROCKETLAUNCHER] = "RPG [OWNED]" : weapNames[WEAP_ROCKETLAUNCHER] = "RPG";
	isWeapBought[WEAP_ROCKETLAUNCHER] = (player->HasRocketLauncher());

	(player->HasHatTrick()) ? weapNames[WEAP_HATTRICK] = "Hat Trick [OWNED]" : weapNames[WEAP_HATTRICK] = "Hat Trick";
	isWeapBought[WEAP_HATTRICK] = (player->HasHatTrick());


	switch (m_nMenuTab)
	{

	case ITEMS_TAB:
	{
		Inventory* inv = player->GetInventory();

		// The power of ternary operators and boolean returns ~ 
		(inv->GetWalls() >= inv->m_unWallsMAX) ? itemNames[ITEM_WALL] = "Walls [MAXED]" : itemNames[ITEM_WALL] = "Walls +50";
		isItemMaxed[ITEM_WALL] = (inv->GetWalls() >= inv->m_unWallsMAX);

		(inv->GetWindows() >= inv->m_unWindowsMAX) ? itemNames[ITEM_WINDOW] = "Windows [MAXED OUT]" : itemNames[ITEM_WINDOW] = "Windows +10";
		isItemMaxed[ITEM_WINDOW] = (inv->GetWindows() >= inv->m_unWindowsMAX);

		(inv->GetMines() >= inv->m_unMinesMAX) ? itemNames[ITEM_MINE] = "A-Z Mine [MAXED OUT]" : itemNames[ITEM_MINE] = "A-Z Mine +1";
		isItemMaxed[ITEM_MINE] = (inv->GetMines() >= inv->m_unMinesMAX);

		(inv->GetGrenades() >= inv->m_unGrenadesMAX) ? itemNames[ITEM_GRENADE] = "Frag Grenades [MAXED OUT]" : itemNames[ITEM_GRENADE] = "Frag Grenades +3";
		isItemMaxed[ITEM_GRENADE] = (inv->GetGrenades() >= inv->m_unGrenadesMAX);

		(inv->GetBearTraps() >= inv->m_unBearTrapsMAX) ? itemNames[ITEM_BEARTRAP] = "Beartrap [MAXED OUT]" : itemNames[ITEM_BEARTRAP] = "Beartrap +1";
		isItemMaxed[ITEM_BEARTRAP] = (inv->GetBearTraps() >= inv->m_unBearTrapsMAX);

		(inv->GetDroneCount() >= inv->m_unDronesMAX) ? itemNames[ITEM_DRONE] = "Drone [MAXED OUT]" : itemNames[ITEM_DRONE] = "Drone +1";
		isItemMaxed[ITEM_DRONE] = (inv->GetDroneCount() >= inv->m_unDronesMAX);
	}
		break;

	case UPGRADES_TAB:
	{
		Weapon* weapons = player->GetWeapons();

		// Ammos
		(weapons[0].GetMaxAmmo() >= weapons[0].GetMaxAmmoCap()) ? upgradeNames[UG_AR_AMMO] = "AR Max Ammo [MAXED OUT]" : upgradeNames[UG_AR_AMMO] = "AR Max Ammo +100";
		isUpgradeMaxed[UG_AR_AMMO] = (weapons[0].GetMaxAmmo() >= weapons[0].GetMaxAmmoCap());
		(!player->HasAR()) ? upgradeNames[UG_AR_AMMO] = "AR Max Ammo [N/A]" : upgradeNames[UG_AR_AMMO] = "AR Max Ammo +100";

		(weapons[1].GetMaxAmmo() >= weapons[1].GetMaxAmmoCap()) ? upgradeNames[UG_SHOTGUN_AMMO] = "Shotgun Max Ammo [MAXED OUT]" : upgradeNames[UG_SHOTGUN_AMMO] = "Shotgun Max Ammo +100";
		isUpgradeMaxed[UG_SHOTGUN_AMMO] = (weapons[1].GetMaxAmmo() >= weapons[1].GetMaxAmmoCap());
		(!player->HasShotty()) ? upgradeNames[UG_SHOTGUN_AMMO] = "Shotgun Max Ammo [N/A]" : upgradeNames[UG_SHOTGUN_AMMO] = "Shotgun Max Ammo +100";

		(weapons[2].GetMaxAmmo() >= weapons[2].GetMaxAmmoCap()) ? upgradeNames[UG_LAUNCHER_AMMO] = "RPG Max Ammo [MAXED OUT]" : upgradeNames[UG_LAUNCHER_AMMO] = "RPG Max Ammo +100";
		isUpgradeMaxed[UG_LAUNCHER_AMMO] = (weapons[2].GetMaxAmmo() >= weapons[2].GetMaxAmmoCap());
		(!player->HasRocketLauncher()) ? upgradeNames[UG_LAUNCHER_AMMO] = "RPG Max Ammo [N/A]" : upgradeNames[UG_LAUNCHER_AMMO] = "RPG Max Ammo +100";


		// Fire Rate
		(weapons[0].GetFireRate() <= weapons[0].GetFireRateCap()) ? upgradeNames[UG_AR_ROF] = "AR Fire Rate [MAXED OUT]" : upgradeNames[UG_AR_ROF] = "AR Fire Rate -0.2";
		isUpgradeMaxed[UG_AR_ROF] = (weapons[0].GetFireRate() <= weapons[0].GetFireRateCap());
		(!player->HasAR()) ? upgradeNames[UG_AR_ROF] = "AR Fire Rate [N/A]" : upgradeNames[UG_AR_ROF] = "AR Fire Rate -0.2";


		(weapons[1].GetFireRate() <= weapons[1].GetFireRateCap()) ? upgradeNames[UG_SHOTGUN_ROF] = "Shotgun Fire Rate [MAXED OUT]" : upgradeNames[UG_SHOTGUN_ROF] = "Shotgun Fire Rate -0.1";
		isUpgradeMaxed[UG_SHOTGUN_ROF] = (weapons[1].GetFireRate() <= weapons[1].GetFireRateCap());
		(!player->HasShotty()) ? upgradeNames[UG_SHOTGUN_ROF] = "Shotgun Fire Rate [N/A]" : upgradeNames[UG_SHOTGUN_ROF] = "Shotgun Fire Rate -0.1";


		(weapons[2].GetFireRate() <= weapons[2].GetFireRateCap()) ? upgradeNames[UG_LAUNCHER_ROF] = "RPG Fire Rate [MAXED OUT]" : upgradeNames[UG_LAUNCHER_ROF] = "RPG Fire Rate -0.2";
		isUpgradeMaxed[UG_LAUNCHER_ROF] = (weapons[2].GetFireRate() <= weapons[2].GetFireRateCap());
		(!player->HasRocketLauncher()) ? upgradeNames[UG_LAUNCHER_ROF] = "RPG Fire Rate [N/A]" : upgradeNames[UG_LAUNCHER_ROF] = "RPG Fire Rate -0.2";



		// Damage
		(m_nARDamage >= m_nARDamageMax) ? upgradeNames[UG_AR_DAMAGE] = "AR Damage [MAXED OUT]" : upgradeNames[UG_AR_DAMAGE] = "AR Damage +20";
		isUpgradeMaxed[UG_AR_DAMAGE] = (m_nARDamage >= m_nARDamageMax);
		(!player->HasAR()) ? upgradeNames[UG_AR_AMMO] = "AR Damage [N/A]" : upgradeNames[UG_AR_AMMO] = "AR Damage +20";


		(m_nShotgunDamage >= m_nSHDamageMax) ? upgradeNames[UG_SHOTGUN_DAMAGE] = "Shotgun Damage [MAXED OUT]" : upgradeNames[UG_SHOTGUN_DAMAGE] = "Shotgun Damage +20";
		isUpgradeMaxed[UG_SHOTGUN_DAMAGE] = (m_nShotgunDamage >= m_nSHDamageMax);
		(!player->HasShotty()) ? upgradeNames[UG_SHOTGUN_DAMAGE] = "Shotgun Damage [N/A]" : upgradeNames[UG_SHOTGUN_DAMAGE] = "Shotgun Damage +20";


		(m_nRLDamage >= m_nRLDamageMax) ? upgradeNames[UG_LAUNCHER_DAMAGE] = "RPG Damage [MAXED OUT]" : upgradeNames[UG_LAUNCHER_DAMAGE] = "RPG Damage +20";
		isUpgradeMaxed[UG_LAUNCHER_DAMAGE] = (m_nRLDamage >= m_nRLDamageMax);
		(!player->HasRocketLauncher()) ? upgradeNames[UG_LAUNCHER_DAMAGE] = "RPG Damage [N/A]" : upgradeNames[UG_LAUNCHER_DAMAGE] = "RPG Damage +20";


	}
		break;

	case FORTIFICATIONS_TAB:
	{
		Inventory* inv = player->GetInventory();

		(inv->GetMachineGunTowers() >= inv->m_unMachineGunTowersMAX) ? fortNames[FORT_MG] = "Machine Gun Tower [MAXED OUT]" : fortNames[FORT_MG] = "Machine Gun Tower";
		isFortMaxed[FORT_MG] = (inv->GetMachineGunTowers() >= inv->m_unMachineGunTowersMAX);

		(inv->GetMapleSyrupTowers() >= inv->m_unMapleSyrupTowersMAX) ? fortNames[FORT_MAPLESYRUP] = "Maple Syrup Tower [MAXED OUT]" : fortNames[FORT_MAPLESYRUP] = "Maple Syrup Tower";
		isFortMaxed[FORT_MAPLESYRUP] = (inv->GetMapleSyrupTowers() >= inv->m_unMapleSyrupTowersMAX);

		(inv->GetHockeyStickTowers() >= inv->m_unHockeyStickTowersMAX) ? fortNames[FORT_HOCKEYSTICK] = "Hockey Stick Tower [MAXED OUT]" : fortNames[FORT_HOCKEYSTICK] = "Hockey Stick Tower";
		isFortMaxed[FORT_HOCKEYSTICK] = (inv->GetHockeyStickTowers() >= inv->m_unHockeyStickTowersMAX);

		(inv->GetLaserTowers() >= inv->m_unLaserTowersMAX) ? fortNames[FORT_LASER] = "Laser Tower [MAXED OUT]" : fortNames[FORT_LASER] = "Laser Tower";
		isFortMaxed[FORT_LASER] = (inv->GetLaserTowers() >= inv->m_unLaserTowersMAX);

		(inv->GetLavaTraps() >= inv->m_unLavaTrapsMAX) ? fortNames[FORT_LAVATRAP] = "Lava Trap [MAXED OUT]" : fortNames[FORT_LAVATRAP] = "Lava Trap +1";
		isFortMaxed[FORT_LAVATRAP] = (inv->GetLavaTraps() >= inv->m_unLavaTrapsMAX);

		(inv->GetSpikeTraps() >= inv->m_unSpikeTrapsMAX) ? fortNames[FORT_SPIKETRAP] = "Spike Trap [MAXED OUT]" : fortNames[FORT_SPIKETRAP] = "Spike Trap +1";
		isFortMaxed[FORT_SPIKETRAP] = (inv->GetSpikeTraps() >= inv->m_unSpikeTrapsMAX);

	}
		break;

	}
}

// Render
//	- draw the shop and it's items as well as it's descriptions
void Shop::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// If it's open
	if (IsOpen())
	{

		// Check and update the item's status
		UpdateItemStatus();

		if (m_nMenuTab == MAIN_TAB)
		{
			// Draw Main Tab background
			pGraphics->DrawTexture(m_hBackgroundMain, { 0, 0 });

			// Draw options
			if (m_nCursor == MainOptions::OPTIONS_WEAPONS)
				m_pFont->Draw("Gun Rack", 200, 150, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Gun Rack", 200, 150, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_ITEMS)
				m_pFont->Draw("Items Store", 200, 200, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Items Store", 200, 200, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_UPGRADES)
				m_pFont->Draw("Upgrade Center", 200, 250, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Upgrade Center", 200, 250, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_FORTIFICATIONS)
				m_pFont->Draw("Fortification Depot", 200, 300, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Fortification Depot", 200, 300, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_EXITSHOP)
				m_pFont->Draw("Exit Shop", 200, 350, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Exit Shop", 200, 350, 1, { 0, 0, 0 });
		}

		else if (m_nMenuTab == WEAPONS_TAB)
		{

			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the weapon's descriptions --
			m_pFont->Draw(weapDescs[m_nCursor].c_str(), 416, 208, 0.4f, { 0, 0, 0 });

			// -- Draw the weapon's price --
			string price = "Price: ";
			price += std::to_string(weapPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the weapon's picture --
			SGD::Point shitfucks = weapTopLeft[m_nCursor];
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(weapTopLeft[m_nCursor], SGD::Size(100, 100)));

			unsigned int score = dynamic_cast<Player*>(m_pPlayer)->GetScore();

			// -- Draw the weapon's Names and highlighting --
			for (int i = 0; i < TOTAL_WEAPONS; i++)
			{
				// If we're currently selected
				if (m_nCursor == i)
				{
					// -- Draw it in green if we don't own it --

					// Based on the cursor selection
					switch (i)
					{

					case WEAP_AR:
					{
						// If we have the weapon and it's selected and we either have the weapon already OR we can't afford it.
						if (dynamic_cast<Player*>(m_pPlayer)->HasAR() || score < weapPrices[i])
							// Draw it in red to show it can't be bought.
						m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							// Draw it in green if it can bought
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}


					case WEAP_SHOTGUN:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasShotty() || score < weapPrices[i])
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}
					case WEAP_ROCKETLAUNCHER:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasRocketLauncher() || score < weapPrices[i])
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}

					case WEAP_HATTRICK:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasHatTrick() || score < weapPrices[i])
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}

					case WEAP_GOBACK:
					{
						m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}

					default:
						break;

					}

				}

				// NOT SELECTED
				else
				{
					// Draw it in gray if we're not selected and it can't be purchased
					if (score < weapPrices[i] || isWeapBought[i])
						m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });
					else
						// Draw it in black if it's available.
					m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });

			}

		}
		}

		else if (m_nMenuTab == ITEMS_TAB)
		{
			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the stats of the items
			Inventory* inv = dynamic_cast<Player*>(m_pPlayer)->GetInventory();
			string stuff;
			stuff = "Beartraps: ";
			stuff += std::to_string(inv->GetBearTraps());
			m_pFont->Draw(stuff.c_str(), 414, 260, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "A-Z Mines: ";
			stuff += std::to_string(inv->GetMines());
			m_pFont->Draw(stuff.c_str(), 414, 280, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Walls: ";
			stuff += std::to_string(inv->GetWalls());
			m_pFont->Draw(stuff.c_str(), 414, 300, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Windows: ";
			stuff += std::to_string(inv->GetWindows());
			m_pFont->Draw(stuff.c_str(), 414, 320, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Grenades: ";
			stuff += std::to_string(inv->GetGrenades());
			m_pFont->Draw(stuff.c_str(), 414, 340, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Drones: ";
			stuff += std::to_string(inv->GetDroneCount());
			m_pFont->Draw(stuff.c_str(), 414, 360, 0.4f, { 255, 255, 0 });
			stuff.clear();

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the item's descriptions --
			m_pFont->Draw(itemDescs[m_nCursor].c_str(), 416, 208, 0.4f, { 0, 0, 0 });

			// -- Draw the item's price --
			string price = "Price: ";
			price += std::to_string(itemPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the item's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(itemTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the item's Names and highlighting --
			for (int i = 0; i < TOTAL_ITEMS; i++)
			{
				unsigned int score = dynamic_cast<Player*>(m_pPlayer)->GetScore();
				Inventory* inv = dynamic_cast<Player*>(m_pPlayer)->GetInventory();


				// If our object is at max and we're selected
				if (m_nCursor == i && isItemMaxed[i])
					// Draw red
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
				else if (m_nCursor != i && isItemMaxed[i])
					// Draw gray
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });


				else if (m_nCursor == i && score < itemPrices[i])
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
				else if (m_nCursor == i && score >= itemPrices[i])
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });

				else if (m_nCursor != i && score >= itemPrices[i])
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
				else if (m_nCursor != i && score < itemPrices[i])
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });
				
			}
		}

		else if (m_nMenuTab == UPGRADES_TAB)
		{
			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the weapon's statszors
			Weapon* weapons = dynamic_cast<Player*>(m_pPlayer)->GetWeapons();
			string stuff = "AR Rate of Fire: ";
			stuff += std::to_string(weapons[0].GetFireRate());
			m_pFont->Draw(stuff.c_str(), 414, 240, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "AR Damage: ";
			stuff += std::to_string(GetARDamage());
			m_pFont->Draw(stuff.c_str(), 414, 260, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "AR Maximum Ammo: ";
			stuff += std::to_string(weapons[0].GetMaxAmmo());
			m_pFont->Draw(stuff.c_str(), 414, 280, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Shotgun Rate of Fire: ";
			stuff += std::to_string(weapons[1].GetFireRate());
			m_pFont->Draw(stuff.c_str(), 414, 300, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Shotgun Damage: ";
			stuff += std::to_string(GetShotgunDamage());
			m_pFont->Draw(stuff.c_str(), 414, 320, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Shotgun Maximum Ammo: ";
			stuff += std::to_string(weapons[1].GetMaxAmmo());
			m_pFont->Draw(stuff.c_str(), 414, 340, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Rocket Launcher Rate of Fire: ";
			stuff += std::to_string(weapons[2].GetFireRate());
			m_pFont->Draw(stuff.c_str(), 414, 360, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Rocket Launcher Damage: ";
			stuff += std::to_string(GetRLDamage());
			m_pFont->Draw(stuff.c_str(), 414, 380, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Rocket Launcher Maximum Ammo: ";
			stuff += std::to_string(weapons[2].GetMaxAmmo());
			m_pFont->Draw(stuff.c_str(), 414, 400, 0.4f, { 255, 255, 0 });

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the upgrade's descriptions --
			m_pFont->Draw(upgradeDescs[m_nCursor].c_str(), 416, 208, 0.4f, { 0, 0, 0 });

			// -- Draw the upgrade's price --
			string price = "Price: ";
			price += std::to_string(upgradePrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the upgrade's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(upgradeTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the upgrades names and highlighting --
			for (int i = 0; i < TOTAL_UPGRADES; i++)
			{
				unsigned int score = dynamic_cast<Player*>(m_pPlayer)->GetScore();

				if (i == m_nCursor && m_nCursor == UG_AR_AMMO && !isWeapBought[WEAP_AR])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_AR_DAMAGE && !isWeapBought[WEAP_AR])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_AR_ROF && !isWeapBought[WEAP_AR])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_SHOTGUN_AMMO && !isWeapBought[WEAP_SHOTGUN])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_SHOTGUN_DAMAGE && !isWeapBought[WEAP_SHOTGUN])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_SHOTGUN_ROF && !isWeapBought[WEAP_SHOTGUN])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_LAUNCHER_AMMO && !isWeapBought[WEAP_ROCKETLAUNCHER])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_LAUNCHER_DAMAGE && !isWeapBought[WEAP_ROCKETLAUNCHER])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				if (i == m_nCursor && m_nCursor == UG_LAUNCHER_ROF && !isWeapBought[WEAP_ROCKETLAUNCHER])
				{
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
					continue;
				}

				// If our object is at max and we're selected
				if (m_nCursor == i && isUpgradeMaxed[i])
					// Draw red
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });

				else if (m_nCursor != i && isUpgradeMaxed[i])
					// Draw gray
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });


				else if (m_nCursor == i && score < upgradePrices[i])
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
				else if (m_nCursor == i && score >= upgradePrices[i])
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });

				else if (m_nCursor != i && score >= upgradePrices[i])
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
				else if (m_nCursor != i && score < upgradePrices[i])
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });
			}

		}
		else if (m_nMenuTab == FORTIFICATIONS_TAB)
		{
			// Draw the towers items background
			pGraphics->DrawTexture(m_hTowerMain, { 0, 0 });

			// Draw the number of towers had
			Inventory* inv = dynamic_cast<Player*>(m_pPlayer)->GetInventory();

			string stuff = "MG Towers: ";
			stuff += std::to_string(inv->GetMachineGunTowers());
			m_pFont->Draw(stuff.c_str(), 414, 256, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Syrup Towers: ";
			stuff += std::to_string(inv->GetMapleSyrupTowers());
			m_pFont->Draw(stuff.c_str(), 414, 276, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Hockey Stick Towers: ";
			stuff += std::to_string(inv->GetHockeyStickTowers());
			m_pFont->Draw(stuff.c_str(), 414, 296, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Laser Towers: ";
			stuff += std::to_string(inv->GetLaserTowers());
			m_pFont->Draw(stuff.c_str(), 414, 316, 0.4f, { 255, 255, 0 });
			stuff = "Lava Traps: ";
			stuff += std::to_string(inv->GetLavaTraps());
			m_pFont->Draw(stuff.c_str(), 414, 336, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Spike Traps: ";
			stuff += std::to_string(inv->GetSpikeTraps());
			m_pFont->Draw(stuff.c_str(), 414, 356, 0.4f, { 255, 255, 0 });

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the item's descriptions --
			m_pFont->Draw(fortDescs[m_nCursor].c_str(), 416, 208, 0.4f, { 0, 0, 0 });

			// -- Draw the item's price --
			string price = "Price: ";
			price += std::to_string(fortPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the tower's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(fortTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the item's Names and highlighting --
			for (int i = 0; i < TOTAL_TOWERS; i++)
			{
				unsigned int score = dynamic_cast<Player*>(m_pPlayer)->GetScore();


				// If our object is at max and we're selected
				if (m_nCursor == i && isFortMaxed[i])
					// Draw red
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.42f, { 255, 0, 0 });
				else if (m_nCursor != i && isFortMaxed[i])
					// Draw gray
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.42f, { 54, 54, 54 });

				else if (m_nCursor == i && score < fortPrices[i])
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
				else if (m_nCursor == i && score >= fortPrices[i])
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
				else if (m_nCursor != i && score >= fortPrices[i])
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
				else if (m_nCursor != i && score < fortPrices[i])
					m_pFont->Draw(fortNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 54, 54, 54 });
			}

		}
	}
}

// Buy
// - check if the player has enough money
// [in] parcel - the number of the item
// [in] shopSection - 0 (Weapons) 1 (Items) 2 (Upgrades) 3 (Towers)
// [out] return true - has sufficient funds
// [out] return false - has insufficient funds
bool Shop::Buy(int parcel, int shopSection)
{
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	StatTracker* tracker = StatTracker::GetInstance();
	GameplayState* gamep = GameplayState::GetInstance();
	int curMoney = player->GetScore();

	enum { WEAPONS, ITEMS, UPGRADES, FORTIFICATIONS };

	switch (shopSection)
	{

	case WEAPONS:
	{
		switch (parcel)
		{
		case 0:
		{
			if (player->HasAR())
				return false;

			break;
		}

		case 1:
		{
			if (player->HasShotty())
				return false;

			break;
		}

		case 2:
		{
				
			if (player->HasRocketLauncher())
				return false;

			break;
		}

		case 3:
		{
			if (player->HasHatTrick())
				return false;

			break;
		}

		}

		if (curMoney >= (int)weapPrices[parcel])
		{
			// Subtract money
			player->SetScore(curMoney -= weapPrices[parcel]);
			tracker->SpendItUp(weapPrices[parcel]);
			return true;
		}
		else
			return false;
	}
		break;

	case ITEMS:
	{
		// If the player has the money for the item
		if (curMoney >= (int)itemPrices[parcel])
		{
			// Subtract money
			player->SetScore(curMoney -= itemPrices[parcel]);
			tracker->SpendItUp(itemPrices[parcel]);
			return true;
		}
		else
			return false;
	}

		break;
	case UPGRADES:
	{
		// Prevent weapon upgrades being purchased if they dont own the weapon

		if (parcel == UG_AR_AMMO || parcel == UG_AR_DAMAGE || parcel == UG_AR_ROF)
		{
			if (!player->HasAR())
				return false;

		}
		else if (parcel == UG_SHOTGUN_AMMO || parcel == UG_SHOTGUN_DAMAGE || parcel == UG_SHOTGUN_ROF)
		{
			if (!player->HasShotty())
				return false;
		}
		else if (parcel == UG_LAUNCHER_AMMO || parcel == UG_LAUNCHER_DAMAGE || parcel == UG_LAUNCHER_ROF)
		{
			if (!player->HasRocketLauncher())
				return false;
		}
		// If the player has the money for the upgrade
		if (curMoney >= (int)upgradePrices[parcel])
		{
			if (!isUpgradeMaxed[parcel])
			{
				player->SetScore(curMoney -= upgradePrices[parcel]);
				tracker->SpendItUp(itemPrices[parcel]);
				return true;
			}
			return false;
		}
		else
			return false;
	}
		break;

	case FORTIFICATIONS:
	{
		// If the player has the money for the fortification
		if (curMoney >= (int)fortPrices[parcel])
		{
			player->SetScore(curMoney -= fortPrices[parcel]);
			tracker->SpendItUp(fortPrices[parcel]);

			// Handle the stattracker
			if (parcel == FORT_MG || parcel == FORT_MAPLESYRUP || parcel == FORT_HOCKEYSTICK || parcel == FORT_LASER)
			tracker->TowerExchange(true);
			else if (parcel == FORT_LAVATRAP || parcel == FORT_SPIKETRAP)
				tracker->TrapExchange(true);

			return true;
		}
		else
			return false;
	}
		break;

	}

	return true;
}

// Sell
// - Remove the selected parcel
// - Give back % of the original price
// - Add money
// [in] parcel - the number of the item
// [in] shopSection - 0 (Weapons) 1 (Items) 2 (Upgrades) 3 (Towers)
void Shop::Sell(int parcel, int shopSection)
{
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	Inventory* inv = player->GetInventory();
	StatTracker* tracker = StatTracker::GetInstance();
	int curMoney = player->GetScore();

	enum { WEAPONS, ITEMS, UPGRADES, FORTIFICATIONS };

	switch (shopSection)
	{
	case WEAPONS:
	{
		player->SetScore(curMoney += int(weapPrices[parcel] * SELL_DISCOUNT));
	}
		break;

	case ITEMS:
	{
		player->SetScore(curMoney += int(itemPrices[parcel] * SELL_DISCOUNT));
	}
		break;

	case UPGRADES:		// upgrades
	{
		
	}
		break;

	case FORTIFICATIONS: // fortifications
	{

		if (parcel == FORT_MG && inv->GetMachineGunTowers() > 0)
		{
			player->SetScore(curMoney += int(fortPrices[parcel] * SELL_DISCOUNT));
			inv->SetMachineGunTowers(inv->GetMachineGunTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == FORT_MAPLESYRUP && inv->GetMapleSyrupTowers() > 0)
		{
			player->SetScore(curMoney += int(fortPrices[parcel] * SELL_DISCOUNT));
			inv->SetMapleSyrupTowers(inv->GetMapleSyrupTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == FORT_HOCKEYSTICK && inv->GetHockeyStickTowers() > 0)
		{
			player->SetScore(curMoney += int(fortPrices[parcel] * SELL_DISCOUNT));
			inv->SetHockeyStickTowers(inv->GetHockeyStickTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == FORT_LASER && inv->GetLaserTowers() > 0)
		{
			player->SetScore(curMoney += int(fortPrices[parcel] * SELL_DISCOUNT));
			inv->SetLaserTowers(inv->GetLaserTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == FORT_GOBACK)
			break;

	}
		break;
	}
}

// GivePurchase
// - give the player the item they purchased
// [in] parcel - the item they want
// [in] shopSection - 0 (Weapons) 1 (Items) 2 (Upgrades) 3 (Towers)
void Shop::GivePurchase(int parcel, int shopSection)
{
	// Grab the player's inventory
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	Inventory* inventory = player->GetInventory();
	Game* game = Game::GetInstance();
	StatTracker* tracker = StatTracker::GetInstance();
	GameplayState* gamep = GameplayState::GetInstance();
	int curMoney = player->GetScore();

	enum { WEAPONS, ITEMS, UPGRADES, TOWERS };

	// Grab the player's weapons (4)
	Weapon* weapons = player->GetWeapons();

	SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetPurchaseSound());


	// << WEAPONS >>

	if (shopSection == WEAPONS)
	{

		if (parcel == WEAP_AR)
		{
			player->SetAR(true);
			isWeapBought[WEAP_AR] = true;

			/*if (gamep->GetGameMode() == 3)
			{
			player->SetAR(true);
			weapons[0].SetCurrAmmo(AR_CURAMMO);
			weapons[1].SetCurrAmmo(0);
			weapons[2].SetCurrAmmo(0);
			weapons[3].SetCurrAmmo(0);
			}
			else
			{*/

			weapons[0].SetCurrAmmo(AR_CURAMMO);

		}

		if (parcel == WEAP_SHOTGUN)
		{

			player->SetShotgun(true);
			isWeapBought[WEAP_SHOTGUN] = true;

			/*if (gamep->GetGameMode() == 3)
			{
			player->SetShotgun(true);
			weapons[0].SetCurrAmmo(0);
			weapons[1].SetCurrAmmo(SH_CURAMMO);
			weapons[2].SetCurrAmmo(0);
			weapons[3].SetCurrAmmo(0);
			}
			else
			{*/

			weapons[1].SetCurrAmmo(SH_CURAMMO);
		}

		if (parcel == WEAP_ROCKETLAUNCHER)
		{

			player->SetRocketLauncher(true);
			isWeapBought[WEAP_ROCKETLAUNCHER] = true;

			/*if (gamep->GetGameMode() == 3)
			{
			player->SetRocketLauncher(true);
			weapons[0].SetCurrAmmo(0);
			weapons[1].SetCurrAmmo(0);
			weapons[2].SetCurrAmmo(RL_CURAMMO);
			weapons[3].SetCurrAmmo(0);
			}
			else
			{*/

			weapons[2].SetCurrAmmo(RL_CURAMMO);


		}

		if (parcel == WEAP_HATTRICK)
		{
			player->SetHatTrick(true);
			isWeapBought[WEAP_HATTRICK] = true;

			/*if (gamep->GetGameMode() == 3)
			{
			player->SetHatTrick(true);
			weapons[0].SetCurrAmmo(0);
			weapons[1].SetCurrAmmo(0);
			weapons[2].SetCurrAmmo(0);
			weapons[3].SetCurrAmmo(HT_CURAMMO);
			}
			else
			{*/

			weapons[3].SetCurrAmmo(HT_CURAMMO);
		}
	}

	// << ITEMS >>

	else if (shopSection == ITEMS)
	{
		if (parcel == ITEM_PRICE_WALL)
		{
			if (inventory->SetWalls(inventory->GetWalls() + itemAmountToAdd[ITEM_PRICE_WALL]));
			else
				player->SetScore(curMoney += itemPrices[parcel]);

		}
		if (parcel == ITEM_PRICE_WINDOW)
		{
			if(inventory->SetWindows(inventory->GetWindows() + itemAmountToAdd[ITEM_PRICE_WINDOW]));
			else
				player->SetScore(curMoney += itemPrices[parcel]);

		}
		if (parcel == ITEM_BEARTRAP)
		{
			if (inventory->SetBearTraps(inventory->GetBearTraps() + itemAmountToAdd[ITEM_BEARTRAP]))
				tracker->TrapExchange(true);
			else
				player->SetScore(curMoney += itemPrices[parcel]);
		}
		if (parcel == ITEM_MINE)
		{
			if (inventory->SetMines(inventory->GetMines() + itemAmountToAdd[ITEM_MINE]))
				tracker->TrapExchange ( true );
			else
				player->SetScore(curMoney += itemPrices[parcel]);
		}
		if (parcel == ITEM_GRENADE)
			if (inventory->SetGrenades(inventory->GetGrenades() + itemAmountToAdd[ITEM_GRENADE]));
			else
				player->SetScore(curMoney += itemPrices[parcel]);
		if (parcel == ITEM_AMMO)
		{
			// Refill the player's ammo
			weapons[0].SetCurrAmmo(weapons[0].GetMaxAmmo());
			weapons[1].SetCurrAmmo(weapons[1].GetMaxAmmo());
			weapons[2].SetCurrAmmo(weapons[2].GetMaxAmmo());
			weapons[3].SetCurrAmmo(weapons[3].GetMaxAmmo());
		}

		if (parcel == ITEM_DRONE)
		{
			if (inventory->SetDroneCount(inventory->GetDroneCount() + itemAmountToAdd[ITEM_DRONE]))
			{
				CreateDroneMessage* pMsg = new CreateDroneMessage();
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			else
				player->SetScore(curMoney += itemPrices[parcel]);
		}



	}

	// << UPGRADES >>

	if (shopSection == UPGRADES)
	{

		if (!isUpgradeMaxed[parcel])
		{
			if (parcel == UG_SHOTGUN_ROF)
				weapons[1].SetFireRate(weapons[1].GetFireRate() - upgradeAmountToAdd[UG_SHOTGUN_ROF]);

			if (parcel == UG_SHOTGUN_DAMAGE)
				SetShotgunDamage(GetShotgunDamage() + (int)upgradeAmountToAdd[UG_SHOTGUN_DAMAGE]);

			if (parcel == UG_SHOTGUN_AMMO)
				weapons[1].SetMaxAmmo(weapons[1].GetMaxAmmo() + (int)upgradeAmountToAdd[UG_SHOTGUN_AMMO]);


			if (parcel == UG_AR_ROF)
				weapons[0].SetFireRate(weapons[0].GetFireRate() - upgradeAmountToAdd[UG_AR_ROF]);

			if (parcel == UG_AR_DAMAGE)
				SetARDamage(GetARDamage() + (int)upgradeAmountToAdd[UG_AR_DAMAGE]);

			if (parcel == UG_AR_AMMO)
				weapons[0].SetMaxAmmo(weapons[0].GetMaxAmmo() + (int)upgradeAmountToAdd[UG_AR_AMMO]);


			if (parcel == UG_LAUNCHER_ROF)
				weapons[2].SetFireRate(weapons[2].GetFireRate() - upgradeAmountToAdd[UG_LAUNCHER_ROF]);

			if (parcel == UG_LAUNCHER_DAMAGE)
				SetRLDamage(GetRLDamage() + (int)upgradeAmountToAdd[UG_LAUNCHER_DAMAGE]);

			if (parcel == UG_LAUNCHER_AMMO)
				weapons[2].SetMaxAmmo(weapons[2].GetMaxAmmo() + (int)upgradeAmountToAdd[UG_LAUNCHER_AMMO]);
		}


		// Check if we have progressive prices and the upgrade isn't maxed
		if (isUpgradeProgressive[parcel] && !isUpgradeMaxed[parcel])
			upgradePrices[parcel] = (unsigned int)(upgradePrices[parcel] + upgradePrices[parcel] * BUY_INCREASE);
	}


	// << FORTIFICATIONS >>

	if (shopSection == TOWERS)
	{
		if (parcel == FORT_MG)
		{
			if (inventory->SetMachineGunTowers(inventory->GetMachineGunTowers() + 1));
			else
				player->SetScore(curMoney += fortPrices[parcel]);
		}
		if (parcel == FORT_MAPLESYRUP)
		{
			if (inventory->SetMapleSyrupTowers(inventory->GetMapleSyrupTowers() + 1));
			else
				player->SetScore(curMoney += fortPrices[parcel]);
		}
		if (parcel == FORT_HOCKEYSTICK)
		{
			if (inventory->SetHockeyStickTowers(inventory->GetHockeyStickTowers() + 1));
			else
				player->SetScore(curMoney += fortPrices[parcel]);
		}
		if (parcel == FORT_LASER)
		{
			if (inventory->SetLaserTowers(inventory->GetLaserTowers() + 1));
			else
				player->SetScore(curMoney += fortPrices[parcel]);
		}

		if (parcel == FORT_LAVATRAP)
		{
			if (inventory->SetLavaTraps(inventory->GetLavaTraps() + fortAmountToAdd[FORT_LAVATRAP]))
				tracker->TrapExchange(true);
			else
				player->SetScore(curMoney += fortPrices[parcel]);

	}
		if (parcel == FORT_SPIKETRAP)
		{
			if (inventory->SetSpikeTraps(inventory->GetSpikeTraps() + fortAmountToAdd[FORT_SPIKETRAP]))
				tracker->TrapExchange(true);
			else
				player->SetScore(curMoney += fortPrices[parcel]);
}

	}
}

// LoadPrices
// - load in all the shop's data
// [in] xmlFileName - the xml file to load the data from
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

	// -- Read Weapons --
	TiXmlElement* pWeapons = pRoot->FirstChildElement("weapons");

	// Grab the number of weapons
	int weapCount;
	pWeapons->Attribute("count", &weapCount);

	// Grab the 'weapon' element
	TiXmlElement* pWeapon = pWeapons->FirstChildElement("weapon");

	// Loop through all the weapons
	for (int i = 0; i < weapCount; i++)
	{
		// Store the name
		weapNames[i] = pWeapon->Attribute("name");

		// Grab the price
		int price = 0;
		pWeapon->Attribute("price", &price);
		weapPrices[i] = (unsigned int)price;

		// Grab the description
		weapDescs[i] = pWeapon->Attribute("description");

		// Grab the top-left of the images
		pWeapon->QueryFloatAttribute("top", &weapTopLeft[i].x);
		pWeapon->QueryFloatAttribute("left", &weapTopLeft[i].y);

		if (pWeapon->Attribute("isPro"))
		{
			// Grab if we're progressively increasing the price
			int isPro = 0;
			pWeapon->Attribute("isPro", &isPro);
			(isPro) ? isWeapProgressive[i] = true : isWeapProgressive[i] = false;
		}

		// Move down to the next element of 'weapon'
		pWeapon = pWeapon->NextSiblingElement("weapon");
	}


	// -- Read Items --

	// Get the element for the items
	TiXmlElement* pItems = pRoot->FirstChildElement("items");

	// Grab the number of items
	int itemCount;
	pItems->Attribute("count", &itemCount);

	// Grab the 'item' element
	TiXmlElement* pItem = pItems->FirstChildElement("item");

	// Loop through all those items
	for (int i = 0; i < itemCount; i++)
	{
		// Store the name
		itemNames[i] = pItem->Attribute("name");

		// Store the price

		// -- Note, if anyone on this team can do this easier. Please tell me. It's 12:08 AM on Red Bull.
		// Since TinyXML is a cunt and can't do unsigned ints.
		// Lets do this
		int fuckTinyXML = 0;
		pItem->Attribute("price", &fuckTinyXML);
		itemPrices[i] = (unsigned int)fuckTinyXML;

		// Store the description
		itemDescs[i] = pItem->Attribute("description");

		// Store the amount to add if it exists
		if (pItem->Attribute("amountToAdd"))
		{
			fuckTinyXML = 0;
			pItem->Attribute("amountToAdd", &fuckTinyXML);
			itemAmountToAdd[i] = (unsigned int)fuckTinyXML;
		}
		else
			// Make this the number to tell we don't have anything to add
			// since unsigned can't be negative.
			itemAmountToAdd[i] = 0xDEAD;

		// Grab the top-left of the images
		pItem->QueryFloatAttribute("top", &itemTopLeft[i].x);
		pItem->QueryFloatAttribute("left", &itemTopLeft[i].y);

		if (pItem->Attribute("isPro"))
		{
			// Grab if we're progressively increasing the price
			int isPro = 0;
			pItem->Attribute("isPro", &isPro);
			(isPro) ? isItemProgressive[i] = true : isItemProgressive[i] = false;
		}

		// Move down to the next element of 'item'
		pItem = pItem->NextSiblingElement("item");
	}

	// -- Read Upgrades --

	// Get the element for upgrades
	TiXmlElement* pUpgrades = pRoot->FirstChildElement("weaponUpgrades");

	// Grab the upgrade count
	int upgradeCount;
	pUpgrades->Attribute("count", &upgradeCount);

	// Grab the 'upgrade' element
	TiXmlElement* pUpgrade = pUpgrades->FirstChildElement("weaponUpgrade");

	// Loop through all the upgrades
	for (int i = 0; i < upgradeCount; i++)
	{
		// Store the name
		upgradeNames[i] = pUpgrade->Attribute("name");

		// Store the price
		int fuckTinyXML = 0;
		pUpgrade->Attribute("price", &fuckTinyXML);
		upgradePrices[i] = (unsigned int)fuckTinyXML;

		// Store the description
		upgradeDescs[i] = pUpgrade->Attribute("description");

		// If amount to add exists
		if (pUpgrade->Attribute("amountToAdd"))
		{
			pUpgrade->QueryFloatAttribute("amountToAdd", &upgradeAmountToAdd[i]);
		}
		else
			upgradeAmountToAdd[i] = 0xDEAD;

		// Grab the top-left of the images
		pUpgrade->QueryFloatAttribute("top", &upgradeTopLeft[i].x);
		pUpgrade->QueryFloatAttribute("left", &upgradeTopLeft[i].y);

		if (pUpgrade->Attribute("isPro"))
		{
			// Grab if we're progressively increasing the price
			int isPro = 0;
			pUpgrade->Attribute("isPro", &isPro);
			(isPro) ? isUpgradeProgressive[i] = true : isUpgradeProgressive[i] = false;
		}

		// Go to the next upgrade
		pUpgrade = pUpgrade->NextSiblingElement("weaponUpgrade");
	}

	// -- Read Towers --

	// Get the element for upgrades
	TiXmlElement* pTowers = pRoot->FirstChildElement("towers");

	// Grab the tower count
	int towerCount;
	pTowers->Attribute("count", &towerCount);

	// Grab the 'tower' element
	TiXmlElement* pTower = pTowers->FirstChildElement("tower");

	// Loop through all the towers
	for (int i = 0; i < towerCount; i++)
	{
		// Store the name
		fortNames[i] = pTower->Attribute("name");

		// Store the price
		int fuckTinyXML = 0;
		pTower->Attribute("price", &fuckTinyXML);
		fortPrices[i] = (unsigned int)fuckTinyXML;

		// Store the description
		fortDescs[i] = pTower->Attribute("description");

		// Grab the top-left of the images
		pTower->QueryFloatAttribute("top", &fortTopLeft[i].x);
		pTower->QueryFloatAttribute("left", &fortTopLeft[i].y);

		if (pTower->Attribute("isPro"))
		{
			// Grab if we're progressively increasing the price
			int isPro = 0;
			pTower->Attribute("isPro", &isPro);
			(isPro) ? isFortProgressive[i] = true : isFortProgressive[i] = false;
		}

		// If amount to add exists
		if (pTower->Attribute("amountToAdd"))
		{
			int fuckTinyXML = 0;
			pTower->Attribute("amountToAdd", &fuckTinyXML);
			fortAmountToAdd[i] = (unsigned int)fuckTinyXML;
		}

		// Go to the next tower
		pTower = pTower->NextSiblingElement("tower");
	}

}

void Shop::SetTowerFlyweight(TowerFlyweight* _towerFlyweight)
{
	m_pTowerFlyweight = _towerFlyweight;
}
