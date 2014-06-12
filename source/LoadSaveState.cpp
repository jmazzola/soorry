/***************************************************************
|	File:		LoadSaveState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	The state where the player will save and load and
|				delete gamesaves
***************************************************************/
#include "LoadSaveState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "LoadingState.h"
#include "GameplayState.h"
#include "ModePickerState.h"

#include "Button.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

#include "BitmapFont.h"

#include "Entity.h"
#include "EntityManager.h"

#include "../TinyXML/tinyxml.h"

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <fstream>

#include <shlobj.h>
#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
using namespace std;


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cfloat>

/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ LoadSaveState* LoadSaveState::GetInstance(void)
{
	static LoadSaveState s_Instance;	// stored in global memory once
	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void LoadSaveState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	SetTransition(false);

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/LoadSaveBG.png");
	m_hMainMenuSnap = pGraphics->LoadTexture("resource/images/menus/MainMenuBG.png");


	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Set the cursor's index to the first option
	m_nCursor = 0;

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Setup the main button (text)
	m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/1405_RazorBalloon_BlankButton2.png", m_pFont);

	// Setup the directory names for the savefiles
	for (int i = 1; i < NUM_SLOTS + 1; i++)
	{
		m_szSaveFiles[i - 1].clear();

		// Grab the appdata path
		char* path = 0;
		size_t size = MAX_PATH;
		_dupenv_s(&path, &size, "APPDATA");
		m_szSaveFiles[i - 1] += path;

		// Go to our folder in appdata and set up the save name
		m_szSaveFiles[i - 1] += "\\RazorBalloon\\SoorrySaveGame_0";
		// Set 1 - 3
		m_szSaveFiles[i - 1] += std::to_string(i);
		// Set it as .xml since we read it in that way
		m_szSaveFiles[i - 1] += ".xml";
	}

	// Check if we have the files
	for (int i = 0; i < NUM_SLOTS; i++)
		CheckSlotExists(i);

	// Show the cursor
	if(pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();

	m_bTHEBOOL = false;

#if ARCADE_MODE
	bool m_bAccept = true;
	m_vtStick = SGD::Vector{0.0f, 0.0f};
#endif
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void LoadSaveState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hBackground);
	pGraphics->UnloadTexture(m_hMainMenuSnap);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;

	m_bAccept = true;
	m_bTHEBOOL = false;
	m_vtStick = SGD::Vector { 0.0f , 0.0f };
}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool LoadSaveState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

#if ARCADE_MODE
	m_vtStick = pInput->GetLeftJoystick(0);

	 if(abs(m_vtStick.x) < 0.2f)
		 m_vtStick.x = 0.0f;
	 if(abs(m_vtStick.y) < 0.2f)
		 m_vtStick.y = 0.0f;

	 if(m_vtStick == SGD::Vector{0.0f, 0.0f})
		 m_bAccept = true;
#endif

	// --- Scrolling through options ---
	// If the down arrow (PC), or down dpad (Xbox 360) are pressed
	// Move the cursor (selected item) down
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = m_vtStick.y > 0 && m_bAccept;
#endif
	if (m_bTHEBOOL)
	{
		// TODO: Add sound fx for going up and down
		++m_nCursor;

		// Wrap around the options
		if (m_nCursor > MENU_GOBACK)
			m_nCursor = MENU_SLOT1;
#if ARCADE_MODE
		m_bAccept = false;
#endif
	}
	// If the up arrow (PC), or up dpad (Xbox 360) are pressed
	// Move the cursor (selected item) up
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = m_vtStick.y < 0 && m_bAccept;
#endif
	if (m_bTHEBOOL)
	{
		--m_nCursor;

		// Wrap around the options
		if (m_nCursor < MENU_SLOT1)
			m_nCursor = MENU_GOBACK;
#if ARCADE_MODE
		m_bAccept = false;
#endif
	}
	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = pInput->IsButtonPressed(0, 0);
#endif
	if (m_bTHEBOOL)
	{
		// If we're on the first slot and the file exists (not a new game)
		if (m_nCursor == MENU_SLOT1 && m_bFileExists[0])
		{
			// Set the gameplay to slot 1
			GameplayState::GetInstance()->SetCurrentGameSlot(1);
			// Load the gameplay state
			pGame->ChangeState(GameplayState::GetInstance());
			// Exit immediately
			return true;
		}
		else if (m_nCursor == MENU_SLOT1 && !m_bFileExists[0])
		{
			// Go to the game mode picker
			GameplayState::GetInstance()->SetCurrentGameSlot(1);
			// Go choose a gamemode
			pGame->Transition(ModePickerState::GetInstance());
			// Exit immediately
			return true;
		}
		else if (m_nCursor == MENU_SLOT2 && m_bFileExists[1])
		{
			// Set the gameplay to slot 2
			GameplayState::GetInstance()->SetCurrentGameSlot(2);
			pGame->ChangeState(GameplayState::GetInstance());
			return true;
		}
		else if (m_nCursor == MENU_SLOT2 && !m_bFileExists[1])
		{
			GameplayState::GetInstance()->SetCurrentGameSlot(2);
			pGame->Transition(ModePickerState::GetInstance());
			return true;
		}
		else if (m_nCursor == MENU_SLOT3 && m_bFileExists[2])
		{
			// Set the gameplay to slot 3
			GameplayState::GetInstance()->SetCurrentGameSlot(3);
			pGame->ChangeState(GameplayState::GetInstance());
			return true;
		}
		else if (m_nCursor == MENU_SLOT3 && !m_bFileExists[2])
		{
			GameplayState::GetInstance()->SetCurrentGameSlot(3);
			pGame->Transition(ModePickerState::GetInstance());
			return true;
		}

		else if(m_nCursor == MENU_GOBACK)
		{
			pGame->Transition(MainMenuState::GetInstance());
			return true;

		}
	}

	// If we press Backspace
#if !ARCADE_MODE
	m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Backspace) || pInput->IsButtonPressed(0, (unsigned int)SGD::Button::X);
#endif
#if ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed(0, 5);
#endif
	if (m_bTHEBOOL)
	{
		// Delete the file
		remove(m_szSaveFiles[m_nCursor].c_str());

		// Check if we have the files again to refresh
		for (int i = 0; i < NUM_SLOTS; i++)
			CheckSlotExists(i);
	}
	// If we hit escape or B exit and go to main menu
	// Quick Exit Options
#if !ARCADE_MODE
	m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, (unsigned int)SGD::Button::B);
#endif
#if ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed(0, 6);
#endif
	if(m_bTHEBOOL)
	{
		//Go to Main Menu
		pGame->Transition(MainMenuState::GetInstance());
		// Exit immediately
		return true;
	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void LoadSaveState::Update(float elapsedTime)
{
	// If we're changing menus, count down the timer
	if (IsTransitioning())
	{
		m_fTransitionTime -= elapsedTime;

		if (m_fTransitionTime <= 0)
			SetTransition(false);
	}
	else
	{
		// Reset the transition time to allow for transitions again
		m_fTransitionTime = TRANSITION_TIME;
	}
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void LoadSaveState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// TODO: Add Strings to STRING TABLE for easy localization
	// Draw the buttons and text (Super JIT, later make a conditional for the selected color)
	// If we're transitioning
	if (IsTransitioning())
	{
		// Draw the main menu snapshot
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 800 / TRANSITION_TIME * m_fTransitionTime });
	}
	// When the transition is done
	else
	{
		// Render the background
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 0 });

		// If the first file exists
		if (m_bFileExists[0])
		{
			// Show that the save exists
			if (m_nCursor == MENU_SLOT1)
				m_pMainButton->Draw("Slot 1 Save", { 180, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Slot 1 Save", { 180, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}
		else
		{
			// Prompt a new game.
			if (m_nCursor == MENU_SLOT1)
				m_pMainButton->Draw("Make New Game", { 180, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Make New Game", { 180, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}

		if (m_bFileExists[1])
		{
			if (m_nCursor == MENU_SLOT2)
				m_pMainButton->Draw("Slot 2 Save", { 150, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Slot 2 Save", { 150, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}
		else
		{
			if (m_nCursor == MENU_SLOT2)
				m_pMainButton->Draw("Make New Game", { 150, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Make New Game", { 150, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}

		if (m_bFileExists[2])
		{
			if (m_nCursor == MENU_SLOT3)
				m_pMainButton->Draw("Slot 3 Save", { 190, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Slot 3 Save", { 190, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}
		else
		{
			if (m_nCursor == MENU_SLOT3)
				m_pMainButton->Draw("Make New Game", { 190, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Make New Game", { 190, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}

		if (m_nCursor == MENU_GOBACK)
			m_pMainButton->Draw("Go Back", { 170, 470 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
		else
			m_pMainButton->Draw("Go Back", { 170, 470 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

		// Render game info (basic info for now for selected option)
		m_pFont->Draw(LoadFileInfo(m_nCursor), 560, 498, 0.4f, { 0, 0, 0 });
	}
}

/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* LoadSaveState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}

/********************************************/
// LoadSavegame
//  - check if the file exists, set that slot with that data
// [in] slot - The number of the slot to check
// [out] returns true if a savefile exists
// [out] returns false if a savefile does not exist
bool LoadSaveState::CheckSlotExists(int slot)
{
	// Does the file exist?
	m_bFileExists[slot] = PathFileExistsA(m_szSaveFiles[slot].c_str()) ? TRUE : FALSE;

	// true = exists, false = doesn't exist
	return m_bFileExists[slot];
}

// LoadFileInfo
// - return a string containing info about the savegame
// [in] slot - the number slot you're loading
// [out] string - text to display
string LoadSaveState::LoadFileInfo(int slot)
{
	// TODO, redo the load/save screen to have space for the game info, such as:
	// Date and Time the gamesave was last modified, wave number, upgrades, and money.

	TiXmlDocument doc;
	// Attempt to load the file, if not gtfo
	if (!doc.LoadFile(m_szSaveFiles[slot].c_str()))
		return "Can't load XML file";

	// Access the root element (volume)
	TiXmlElement* pRoot = doc.RootElement();

	// Is the root there, if not, gtfo
	if (pRoot == nullptr)
		return "Root cannot be null!";

	float x = float(atoi(pRoot->Attribute("x")));
	float y = float(atoi(pRoot->Attribute("y")));

	// Get the stats
	TiXmlElement* pStats = pRoot->NextSiblingElement("stats");

	// Grab the money
	int money = int(atoi(pStats->Attribute("money")));

	string returnString = "Money: ";
	return returnString + std::to_string(money);
}