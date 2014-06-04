/***************************************************************
|	File:		MainMenuState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will be the main menu of the game,
|				players will be shown a menu to allow for them to
|				enter the options, load/save, or leave the game.
***************************************************************/

#include "MainMenuState.h"

#include "Game.h"
#include "Entity.h"

#include "CreditsState.h"
#include "GameplayState.h"
#include "IntroState.h"
#include "LoadingState.h"
#include "LoadSaveState.h"
#include "OptionsState.h"

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
#include <Windows.h>
#include <XInput.h>
using namespace std;


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cfloat>




/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ MainMenuState* MainMenuState::GetInstance(void)
{
	static MainMenuState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void MainMenuState::Enter(void)
{
	// Game singleton
	Game* pGame = Game::GetInstance();

	SetTransition(false);

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/mainMenuBG.png");
	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Set the cursor's index to the first option
	m_nCursor = 0;

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Setup the universal button
	m_pButton = CreateButton();
	m_pButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);

	// Grab the controllers
	SGD::InputManager::GetInstance()->CheckForNewControllers();
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void MainMenuState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hBackground);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Terminate & deallocate menu items
	m_pButton->Terminate();
	delete m_pButton;
	m_pButton = nullptr;

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool MainMenuState::Input(void)
{
	// Singletons
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape))
		m_nCursor = MENU_EXIT;

	// If we're transitioning, disable input
	if (IsTransitioning())
		return false;


	// --- Scrolling through options ---
	// If the down arrow (PC), or down dpad (Xbox 360) are pressed
	// Move the cursor (selected item) down
	if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
	{
		// TODO: Add sound fx for going up and down
		++m_nCursor;

		// Wrap around the options
		if (m_nCursor > MENU_EXIT)
			m_nCursor = MENU_START;
	}
	// If the up arrow (PC), or up dpad (Xbox 360) are pressed
	// Move the cursor (selected item) up
	else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
	{
		--m_nCursor;

		// Wrap around the options
		if (m_nCursor < MENU_START)
			m_nCursor = MENU_EXIT;
	}

	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
	{
		// Switch table for the item selected
		switch (m_nCursor)
		{
		case MENU_START:
		{
			// Play Game
			pGame->Transition(LoadSaveState::GetInstance());
			// Leave immediately
			return true;
		}
			break;

		case MENU_HOWTOPLAY:
		{
			// How to Play (Gameplay loading tutorial level)
			// This doesn't transition since it'll load a tutorial level, no screen.
			pGame->ChangeState(GameplayState::GetInstance());
			// Leave immediately
			return true;
		}
			break;

		case MENU_OPTIONS:
		{

			// Options
			pGame->Transition(OptionsState::GetInstance());
			// Leave immediately
			return true;
		}
			break;

		case MENU_CREDITS:
		{
			// Credits
			pGame->Transition(CreditsState::GetInstance());
			// Leave immediately
			return true;
		}
			break;

		case MENU_EXIT:
		{
			// Exit the game
			return false;
		}

			break;
		}

#if _DEBUG	// if statement that says 'if we're running in debug mode' (Visual Studio goody :P)
		// this won't work in release ;3

		// Debugging so our team doesn't have to go through so many menus to test stuff

		// 1	-	Forceload GameplayState
		// 2	-	Forceload LoadSaveState
		// 3	-	Forceload OptionsState
		// 4	-	Forceload CreditsState

		if (pInput->IsKeyPressed(SGD::Key::One))
		{
			pGame->ChangeState(GameplayState::GetInstance());
			return true;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Two))
		{
			pGame->ChangeState(LoadSaveState::GetInstance());
			return true;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Three))
		{
			pGame->ChangeState(OptionsState::GetInstance());
			return true;
		}
		else if (pInput->IsKeyPressed(SGD::Key::Four))
		{
			pGame->ChangeState(CreditsState::GetInstance());
			return true;
		}

#endif
	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void MainMenuState::Update(float elapsedTime)
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

	// Check collisions
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void MainMenuState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

#if _DEBUG

	pGraphics->DrawString("Debugging: TRUE: \n1 - Launch Gameplay\n2 - Launch LoadSave\n3 - Launch Options\n4 - Launch Credits", { 0, 0 }, { 0, 0, 0 });
#endif

	// If we're transitioning
	if (IsTransitioning())
	{
		// TODO: In polish, make different directions
		// Draw the background moving from the bottom up
		// As well as grab a screenshot with buttons/text unselected.
		// I tried to do this, but realized that bitmap saving is a bit more
		// time consuming with this user story.
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 800 / TRANSITION_TIME * m_fTransitionTime });
		return;
	}
	else
	{
		pGraphics->DrawTexture(m_hBackground, { 0, 0 });
		// TODO: Add Strings to STRING TABLE for easy localization
		// Draw the buttons and text (Super JIT, later make a conditional for the selected color)
		if (m_nCursor == MENU_START)
			m_pButton->Draw("Play Game", { 200, 200 }, { 255, 0, 0 }, { 1, 1 }, 0);		// 0
		else
			m_pButton->Draw("Play Game", { 200, 200 }, { 0, 0, 0 }, { 1, 1 }, 0);		// 0

		if (m_nCursor == MENU_HOWTOPLAY)
			m_pButton->Draw("How to Play", { 150, 270 }, { 255, 0, 0 }, { 1, 1 }, 0);	// 1
		else
			m_pButton->Draw("How to Play", { 150, 270 }, { 0, 0, 0 }, { 1, 1 }, 0);		// 1

		if (m_nCursor == MENU_OPTIONS)
			m_pButton->Draw("Options", { 200, 340 }, { 255, 0, 0 }, { 1, 1 }, 0);		// 2
		else
			m_pButton->Draw("Options", { 200, 340 }, { 0, 0, 0 }, { 1, 1 }, 0);			// 2

		if (m_nCursor == MENU_CREDITS)
			m_pButton->Draw("Credits", { 230, 410 }, { 255, 0, 0 }, { 1, 1 }, 0);		// 3
		else
			m_pButton->Draw("Credits", { 230, 410 }, { 0, 0, 0 }, { 1, 1 }, 0);			// 3

		if (m_nCursor == MENU_EXIT)
			m_pButton->Draw("Exit Game", { 180, 480 }, { 255, 0, 0 }, { 1, 1 }, 0);		// 4
		else
			m_pButton->Draw("Exit Game", { 180, 480 }, { 0, 0, 0 }, { 1, 1 }, 0);		// 4
	}

}

/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* MainMenuState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}
