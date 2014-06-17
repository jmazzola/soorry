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

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "BitmapFont.h"

#define SELL_DISCOUNT 0.75f

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
	return towerPrices[_tower];
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
				m_nCursor = MainOptions::OPTIONS_WEAPONS;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < MainOptions::OPTIONS_WEAPONS)
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

			case MainOptions::OPTIONS_TOWERS:
				m_nCursor = 0;
				m_nMenuTab = TOWERS_TAB;
				break;

			case MainOptions::OPTIONS_EXITSHOP:
				m_nCursor = 0;
				m_nMenuTab = MAIN_TAB;
				SetShopStatus(false);
				break;
			}
		}
	}

	// Weapons Tab
	else if (m_nMenuTab == WEAPONS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;

			// Wrap around the options
			if (m_nCursor > WeaponsOptions::WEAPON_GOBACK)
				m_nCursor = WeaponsOptions::WEAPON_AR;
		}

		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < WeaponsOptions::WEAPON_AR)
				m_nCursor = WeaponsOptions::WEAPON_GOBACK;
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			// If we're going back
			if (m_nCursor == WeaponsOptions::WEAPON_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_WEAPONS;
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
				if (Buy(m_nCursor, 1))
					// Give the item
					GivePurchase(m_nCursor, 1);
			}
		}
	}
	// Upgrades Tab
	else if (m_nMenuTab == UPGRADES_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;

			// Wrap around the options
			if (m_nCursor > UpgradesOptions::UG_GOBACK)
				m_nCursor = UpgradesOptions::UG_SHOTGUN_ROF;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < UpgradesOptions::UG_SHOTGUN_ROF)
				m_nCursor = UpgradesOptions::UG_GOBACK;
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			// If we're going back
			if (m_nCursor == UpgradesOptions::UG_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_UPGRADES;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 2))
					// Give the item
					GivePurchase(m_nCursor, 2);
			}
		}

	}
	// Towers Tab
	else if (m_nMenuTab == TOWERS_TAB)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nCursor;

			// Wrap around the options
			if (m_nCursor > TowersOptions::TOWER_GOBACK)
				m_nCursor = TowersOptions::TOWER_MG;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nCursor;

			// Wrap around the options
			if (m_nCursor < TowersOptions::TOWER_MG)
				m_nCursor = TowersOptions::TOWER_GOBACK;
		}

		// --- Selecting an option ---
		// If the enter key (PC) or A button (Xbox 360) are pressed
		// Select the item
		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			// If we're going back
			if (m_nCursor == TowersOptions::TOWER_GOBACK)
			{
				// Set it on the Items option
				m_nCursor = MainOptions::OPTIONS_TOWERS;
				// Go back to the main tab
				m_nMenuTab = MAIN_TAB;
			}
			else
			{
				// If we can buy the item
				if (Buy(m_nCursor, 3))
					// Give the item
					GivePurchase(m_nCursor, 3);
			}
		}
		// Sell a tower
		if (pInput->IsKeyPressed(SGD::Key::Backspace) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::X))
			Sell(m_nCursor, 3);

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

			if (m_nCursor == MainOptions::OPTIONS_TOWERS)
				m_pFont->Draw("Tower Shop", 200, 300, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Tower Shop", 200, 300, 1, { 0, 0, 0 });

			if (m_nCursor == MainOptions::OPTIONS_EXITSHOP)
				m_pFont->Draw("Exit Mall", 200, 350, 1, { 0, 255, 0 });
			else
				m_pFont->Draw("Exit Mall", 200, 350, 1, { 0, 0, 0 });
		}

		else if (m_nMenuTab == WEAPONS_TAB)
		{
			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the weapon's descriptions --
			m_pFont->Draw(weapDescs[m_nCursor].c_str(), 416, 208, 0.6f, { 0, 0, 0 });

			// -- Draw the weapon's price --
			string price = "Price: ";
			price += std::to_string(weapPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the weapon's picture --
			SGD::Point shitfucks = weapTopLeft[m_nCursor];
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(weapTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the weapon's Names and highlighting --
			for (int i = 0; i < TOTAL_WEAPONS; i++)
			{
				// If we're currently selected
				if (m_nCursor == i)
				{
					// Draw it in green if we don't own it
					switch (i)
					{
					case 0:
						m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						break;
					case 1:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasShotty() == true)
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					}
					case 2:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasRocketLauncher() == true)
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
					}
						break;
					case 3:
					{
						if (dynamic_cast<Player*>(m_pPlayer)->HasHatTrick() == true)
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 255, 0, 0 });
						else
							m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
					}
						break;
					case 4:
						m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
						break;
					default:
						break;
					}

				}
				else
					// Draw it in black
					m_pFont->Draw(weapNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
			}
		}

		else if (m_nMenuTab == ITEMS_TAB)
		{
			// Draw the menu items background
			pGraphics->DrawTexture(m_hBackground, { 0, 0 });

			// Draw the stats of the items
			Inventory* inv = dynamic_cast<Player*>(m_pPlayer)->GetInventory();
			string stuff = "Walls: ";
			stuff += std::to_string(inv->GetWalls());
			m_pFont->Draw(stuff.c_str(), 414, 260, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Windows: ";
			stuff += std::to_string(inv->GetWindows());
			m_pFont->Draw(stuff.c_str(), 414, 280, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Beartraps: ";
			stuff += std::to_string(inv->GetBearTraps());
			m_pFont->Draw(stuff.c_str(), 414, 300, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "A-T Mines: ";
			stuff += std::to_string(inv->GetMines());
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
			stuff = "Lava Traps: ";
			stuff += std::to_string(inv->GetLavaTraps());
			m_pFont->Draw(stuff.c_str(), 414, 380, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Spike Traps: ";
			stuff += std::to_string(inv->GetSpikeTraps());
			m_pFont->Draw(stuff.c_str(), 414, 400, 0.4f, { 255, 255, 0 });



			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the item's descriptions --
			m_pFont->Draw(itemDescs[m_nCursor].c_str(), 416, 208, 0.6f, { 0, 0, 0 });

			// -- Draw the item's price --
			string price = "Price: ";
			price += std::to_string(itemPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the item's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(itemTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the item's Names and highlighting --
			for (int i = 0; i < TOTAL_ITEMS; i++)
			{
				// If we're currently selected
				if (m_nCursor == i)
					// Draw it in green
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
				else
					// Draw it in black
					m_pFont->Draw(itemNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
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
			m_pFont->Draw(upgradeDescs[m_nCursor].c_str(), 416, 208, 0.6f, { 0, 0, 0 });

			// -- Draw the upgrade's price --
			string price = "Price: ";
			price += std::to_string(upgradePrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the upgrade's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(upgradeTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the upgrades names and highlighting --
			for (int i = 0; i < TOTAL_UPGRADES; i++)
			{
				// If we're currently selected
				if (m_nCursor == i)
					// Draw it in green
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
				else
					// Draw it in black
					m_pFont->Draw(upgradeNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
			}

		}
		else if (m_nMenuTab == TOWERS_TAB)
		{
			// Draw the towers items background
			pGraphics->DrawTexture(m_hTowerMain, { 0, 0 });

			// Draw the number of towers had
			Inventory* inv = dynamic_cast<Player*>(m_pPlayer)->GetInventory();
			string stuff = "MG Towers: ";
			stuff += std::to_string(inv->GetMachineGunTowers());
			m_pFont->Draw(stuff.c_str(), 414, 296, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Syrup Towers: ";
			stuff += std::to_string(inv->GetMapleSyrupTowers());
			m_pFont->Draw(stuff.c_str(), 414, 316, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Hockey Stick Towers: ";
			stuff += std::to_string(inv->GetHockeyStickTowers());
			m_pFont->Draw(stuff.c_str(), 414, 336, 0.4f, { 255, 255, 0 });
			stuff.clear();
			stuff = "Laser Towers: ";
			stuff += std::to_string(inv->GetLaserTowers());
			m_pFont->Draw(stuff.c_str(), 414, 356, 0.4f, { 255, 255, 0 });

			// Draw the mun-knee
			string money = "Money: " + std::to_string(dynamic_cast<Player*>(m_pPlayer)->GetScore());
			m_pFont->Draw(money.c_str(), 565, 60, 0.4f, { 255, 255, 255 });

			// -- Draw the item's descriptions --
			m_pFont->Draw(towerDescs[m_nCursor].c_str(), 416, 208, 0.6f, { 0, 0, 0 });

			// -- Draw the item's price --
			string price = "Price: ";
			price += std::to_string(towerPrices[m_nCursor]);
			m_pFont->Draw(price.c_str(), 573, 113, 0.5f, { 0, 0, 0 });

			// -- Draw the tower's picture --
			pGraphics->DrawTextureSection(m_hShopItems, { 424, 60 }, SGD::Rectangle(towerTopLeft[m_nCursor], SGD::Size(100, 100)));

			// -- Draw the item's Names and highlighting --
			for (int i = 0; i < TOTAL_TOWERS; i++)
			{
				// If we're currently selected
				if (m_nCursor == i)
					// Draw it in green
					m_pFont->Draw(towerNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 255, 0 });
				else
					// Draw it in black
					m_pFont->Draw(towerNames[i].c_str(), 55, 70 + 40 * i, 0.5f, { 0, 0, 0 });
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
	int curMoney = player->GetScore();

	enum { WEAPONS, ITEMS, UPGRADES, TOWERS };

	switch (shopSection)
	{

	case WEAPONS:
	{
		switch (parcel)
		{
		case 0:
			return false;
			break;
		case 1:
			if (player->HasShotty() == true)
				return false;
			break;
		case 2:
			if (player->HasRocketLauncher() == true)
				return false;
			break;
		case 3:
			if (player->HasHatTrick() == true)
				return false;
			break;
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
		// If the player has the money for the upgrade
		if (curMoney >= (int)upgradePrices[parcel])
		{
			player->SetScore(curMoney -= upgradePrices[parcel]);
			tracker->SpendItUp(itemPrices[parcel]);
			return true;
		}
		else
			return false;
	}
		break;

	case TOWERS:
	{
		// If the player has the money for the upgrade
		if (curMoney >= (int)towerPrices[parcel])
		{
			player->SetScore(curMoney -= towerPrices[parcel]);
			tracker->SpendItUp(itemPrices[parcel]);
			tracker->TowerExchange(true);
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
// - Give back 75% of the original price
// - Add money
// [in] parcel - the number of the item
// [in] shopSection - 0 (Weapons) 1 (Items) 2 (Upgrades) 3 (Towers)
void Shop::Sell(int parcel, int shopSection)
{
	Player* player = dynamic_cast<Player*>(m_pPlayer);
	Inventory* inv = player->GetInventory();
	StatTracker* tracker = StatTracker::GetInstance();
	int curMoney = player->GetScore();

	enum { WEAPONS, ITEMS, UPGRADES, TOWERS };

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
		player->SetScore(curMoney += int(upgradePrices[parcel] * SELL_DISCOUNT));
	}
		break;

	case TOWERS:		// towers
	{

		if (parcel == TOWER_MG && inv->GetMachineGunTowers() > 0)
		{
			player->SetScore(curMoney += int(towerPrices[parcel] * SELL_DISCOUNT));
			inv->SetMachineGunTowers(inv->GetMachineGunTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == TOWER_MAPLESYRUP && inv->GetMapleSyrupTowers() > 0)
		{
			player->SetScore(curMoney += int(towerPrices[parcel] * SELL_DISCOUNT));
			inv->SetMapleSyrupTowers(inv->GetMapleSyrupTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == TOWER_HOCKEYSTICK && inv->GetHockeyStickTowers() > 0)
		{
			player->SetScore(curMoney += int(towerPrices[parcel] * SELL_DISCOUNT));
			inv->SetHockeyStickTowers(inv->GetHockeyStickTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == TOWER_LASER && inv->GetLaserTowers() > 0)
		{
			player->SetScore(curMoney += int(towerPrices[parcel] * SELL_DISCOUNT));
			inv->SetLaserTowers(inv->GetLaserTowers() - 1);
			tracker->TowerExchange(false);
		}

		if (parcel == TOWER_GOBACK)
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

	enum { WEAPONS, ITEMS, UPGRADES, TOWERS };

	// Grab the player's weapons (4)
	Weapon* weapons = player->GetWeapons();

	if (shopSection == WEAPONS)
	{
		if (parcel == WEAP_AR)
		{
			if (player->HasAR())
				return;

			player->SetAR(true);
			weapons[0].SetCurrAmmo(200);

		}

		if (parcel == WEAP_SHOTGUN)
		{
			if (player->HasShotty())
				return;

			player->SetShotgun(true);
			weapons[1].SetCurrAmmo(200);
		}

		if (parcel == WEAP_ROCKETLAUNCHER)
		{
			if (player->HasRocketLauncher())
				return;

			player->SetRocketLauncher(true);
			weapons[2].SetCurrAmmo(20);
		}

		if (parcel == WEAP_HATTRICK)
		{
			if (player->HasHatTrick())
				return;

			player->SetHatTrick(true);
			weapons[3].SetCurrAmmo(50);
		}
	}

	// If we're in the items
	else if (shopSection == ITEMS)
	{
		if (parcel == ITEM_PRICE_WALL)
			inventory->SetWalls(inventory->GetWalls() + itemAmountToAdd[ITEM_PRICE_WALL]);
		if (parcel == ITEM_PRICE_WINDOW)
			inventory->SetWindows(inventory->GetWindows() + itemAmountToAdd[ITEM_PRICE_WINDOW]);
		if (parcel == BEARTRAP)
		{
			inventory->SetBearTraps(inventory->GetBearTraps() + itemAmountToAdd[BEARTRAP]);
			tracker->TrapExchange(true);
		}
		if (parcel == MINE)
		{
			inventory->SetMines(inventory->GetMines() + itemAmountToAdd[MINE]);
			tracker->TrapExchange(true);
		}
		if (parcel == GRENADE)
			inventory->SetGrenades(inventory->GetGrenades() + itemAmountToAdd[GRENADE]);
		if (parcel == AMMO)
		{
			// Refill the player's ammo
			weapons[0].SetCurrAmmo(weapons[0].GetMaxAmmo());
			weapons[1].SetCurrAmmo(weapons[1].GetMaxAmmo());
			weapons[2].SetCurrAmmo(weapons[2].GetMaxAmmo());
			weapons[3].SetCurrAmmo(weapons[3].GetMaxAmmo());
		}

		if (parcel == DRONE)
		{
			CreateDroneMessage* pMsg = new CreateDroneMessage();
			pMsg->QueueMessage();
			pMsg = nullptr;

			inventory->SetDroneCount(inventory->GetDroneCount() + itemAmountToAdd[DRONE]);
		}

		if (parcel == LAVATRAP)
		{
			inventory->SetLaserTowers(inventory->GetLavaTraps() + itemAmountToAdd[LAVATRAP]);
			tracker->TrapExchange(true);
		}
		if (parcel == SPIKETRAP)
		{
			inventory->SetSpikeTraps(inventory->GetSpikeTraps() + itemAmountToAdd[SPIKETRAP]);
			tracker->TrapExchange(true);
		}

	}

	if (shopSection == UPGRADES)
	{

		if (parcel == UG_SHOTGUN_ROF)
			weapons[1].SetFireRate(weapons[1].GetFireRate() - upgradeAmountToAdd[UG_SHOTGUN_ROF]);
		if (parcel == UG_SHOTGUN_DAMAGE)
			SetShotgunDamage(GetShotgunDamage() + upgradeAmountToAdd[UG_SHOTGUN_DAMAGE]);
		if (parcel == UG_SHOTGUN_AMMO)
			weapons[1].SetMaxAmmo(weapons[1].GetMaxAmmo() + upgradeAmountToAdd[UG_SHOTGUN_AMMO]);

		if (parcel == UG_AR_ROF)
			weapons[0].SetFireRate(weapons[0].GetFireRate() - upgradeAmountToAdd[UG_AR_ROF]);
		if (parcel == UG_AR_DAMAGE)
			SetARDamage(GetARDamage() + upgradeAmountToAdd[UG_AR_DAMAGE]);
		if (parcel == UG_AR_AMMO)
			weapons[0].SetMaxAmmo(weapons[0].GetMaxAmmo() + upgradeAmountToAdd[UG_AR_AMMO]);

		if (parcel == UG_LAUNCHER_ROF)
			weapons[2].SetFireRate(weapons[2].GetFireRate() - upgradeAmountToAdd[UG_LAUNCHER_ROF]);
		if (parcel == UG_LAUNCHER_DAMAGE)
			SetRLDamage(GetRLDamage() + upgradeAmountToAdd[UG_LAUNCHER_DAMAGE]);
		if (parcel == UG_LAUNCHER_AMMO)
			weapons[2].SetMaxAmmo(weapons[2].GetMaxAmmo() + upgradeAmountToAdd[UG_LAUNCHER_AMMO]);

		if (parcel != UG_GOBACK)
			upgradePrices[parcel] = (unsigned int)(upgradePrices[parcel] + upgradePrices[parcel] * SELL_DISCOUNT);
	}
	if (shopSection == TOWERS)
	{
		if (parcel == TOWER_MG)
			inventory->SetMachineGunTowers(inventory->GetMachineGunTowers() + 1);
		if (parcel == TOWER_MAPLESYRUP)
			inventory->SetMapleSyrupTowers(inventory->GetMapleSyrupTowers() + 1);
		if (parcel == TOWER_HOCKEYSTICK)
			inventory->SetHockeyStickTowers(inventory->GetHockeyStickTowers() + 1);
		if (parcel == TOWER_LASER)
			inventory->SetLaserTowers(inventory->GetLaserTowers() + 1);

	}
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
			int fuckTinyXML = 0;
			pUpgrade->Attribute("amountToAdd", &fuckTinyXML);
			upgradeAmountToAdd[i] = (unsigned int)fuckTinyXML;
		}
		else
			upgradeAmountToAdd[i] = 0xDEAD;

		// Grab the top-left of the images
		pUpgrade->QueryFloatAttribute("top", &upgradeTopLeft[i].x);
		pUpgrade->QueryFloatAttribute("left", &upgradeTopLeft[i].y);

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
		towerNames[i] = pTower->Attribute("name");

		// Store the price
		int fuckTinyXML = 0;
		pTower->Attribute("price", &fuckTinyXML);
		towerPrices[i] = (unsigned int)fuckTinyXML;

		// Store the description
		towerDescs[i] = pTower->Attribute("description");

		// Grab the top-left of the images
		pTower->QueryFloatAttribute("top", &towerTopLeft[i].x);
		pTower->QueryFloatAttribute("left", &towerTopLeft[i].y);

		// Go to the next tower
		pTower = pTower->NextSiblingElement("tower");
	}

}