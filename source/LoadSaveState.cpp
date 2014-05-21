/***************************************************************
|	File:		LoadSaveState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	The state where the player will save and load
|				gamesaves
***************************************************************/
#include "LoadSaveState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "LoadingState.h"
#include "GameplayState.h"

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

#include <cstdlib>
#include <cassert>
#include <sstream>
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

	// Initialize the Event Manager
	m_pEvents = SGD::EventManager::GetInstance();
	m_pEvents->Initialize();

	// Initialize the Message Manager
	//m_pMessages = SGD::MessageManager::GetInstance();
	//m_pMessages->Initialize(&MessageProc);


	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/LoadSaveBG.png");


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
	m_pMainButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);

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

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;


	//m_pMessages->Terminate();
	//m_pMessages = nullptr;
	//SGD::MessageManager::DeleteInstance();


	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;

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


	// --- Scrolling through options ---
	// If the down arrow (PC), or down dpad (Xbox 360) are pressed
	// Move the cursor (selected item) down
	if (pInput->IsKeyPressed(SGD::Key::Down))
	{
		// TODO: Add sound fx for going up and down
		++m_nCursor;

		// Wrap around the options
		if (m_nCursor > MENU_GOBACK)
			m_nCursor = MENU_SLOT1;
	}
	// If the up arrow (PC), or up dpad (Xbox 360) are pressed
	// Move the cursor (selected item) up
	else if (pInput->IsKeyPressed(SGD::Key::Up))
	{
		--m_nCursor;

		// Wrap around the options
		if (m_nCursor < MENU_SLOT1)
			m_nCursor = MENU_GOBACK;
	}
	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
	if (pInput->IsKeyPressed(SGD::Key::Enter))
	{
		// Switch table for the item selected
		switch (m_nCursor)
		{
			// TODO: Make Slots 1-3 open and load the game with their loaded data
			// (thats why they're cascading)
			// until then, they're placeholders.
			case MENU_SLOT1:
			{
				// Load the gameplay state
				pGame->ChangeState(GameplayState::GetInstance());
				// Exit immediately
				return true;
			}

			case MENU_SLOT2:
			{
				pGame->ChangeState(GameplayState::GetInstance());
				return true;
			}

			case MENU_SLOT3:
			{
				pGame->ChangeState(GameplayState::GetInstance());
				return true;
			}

			case MENU_GOBACK:
			{
				// Go Back to Main Menu
				pGame->ChangeState(MainMenuState::GetInstance());
				// Exit immediately
				return true;
			}
				break;

		}
	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void LoadSaveState::Update(float elapsedTime)
{


	// Update the entities
	m_pEntities->UpdateAll(elapsedTime);


	// Process the events & messages
	m_pEvents->Update();
	//m_pMessages->Update();


	// Check collisions
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void LoadSaveState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render the background
	pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 0 });

	// Render the entities
	m_pEntities->RenderAll();

	// TODO: Add Strings to STRING TABLE for easy localization
	// Draw the buttons and text (Super JIT, later make a conditional for the selected color)

	if (m_nCursor == MENU_SLOT1)
		m_pMainButton->Draw("Slot 1 Save", { 180, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw("Slot 1 Save", { 180, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

	if (m_nCursor == MENU_SLOT2)
		m_pMainButton->Draw("Slot 2 Save", { 150, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw("Slot 2 Save", { 150, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

	if (m_nCursor == MENU_SLOT3)
		m_pMainButton->Draw("Slot 3 Save", { 190, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw("Slot 3 Save", { 190, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

	if (m_nCursor == MENU_GOBACK)
		m_pMainButton->Draw("Go Back", { 170, 470 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw("Go Back", { 170, 470 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
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