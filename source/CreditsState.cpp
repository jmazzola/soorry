/***************************************************************
|	File:		CreditsState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will show the player the credits
|				of this game.
***************************************************************/

#include "CreditsState.h"

#include "Game.h"
#include "MainMenuState.h"

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

#define SCROLL_SPEED 35.0f


/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance;	// stored in global memory once
	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void CreditsState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	SetTransition(false);

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/CreditsBG.png");
	m_hMainMenuSnap = pGraphics->LoadTexture("resource/images/menus/MainMenuBG.png");


	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Set the margins for the text
	topMargin = 220;
	bottomMargin = 474;

	// Set and start the credits movement
	textPosition.x = 180;
	textPosition.y = bottomMargin + SCROLL_SPEED;

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Setup the main button (text)
	m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);

#if ARCADE_MODE
	m_vtStick = {0.0f, 0.0f};
	m_bAccept = true;
#endif
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void CreditsState::Exit(void)
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

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool CreditsState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

#if !ARCADE_MODE
	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
	{
		// Since there's only one state..go back to main menu
		pGame->Transition(MainMenuState::GetInstance());
		return true;
	}
#endif

#if ARCADE_MODE
	if(pInput->IsButtonPressed(0, 0))
	 {
	 // Since there's only one state..go back to main menu
		Game::GetInstance()->Transition(MainMenuState::GetInstance());
		return true;
	 }
#endif
	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void CreditsState::Update(float elapsedTime)
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
		// Move the credits
		textPosition.x = 220;
		textPosition.y -= (SCROLL_SPEED * elapsedTime);

		// Reset the transition time to allow for transitions again
		m_fTransitionTime = TRANSITION_TIME;
	}

}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void CreditsState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Draw the credits
	// TODO: Load in a text file
	string credits = "SOORRY\n\n\
					  By Razor Balloon\n\n\
					  Part of Heavy Square Studios\n\n\
					  Associate Producers\n\
					  Sean Hathaway\n\
					  Robert Martinez\n\n\
					  Executive Producer\n\
					  John O' Leske\n\n\
				      World Software Engineer\n\
					  Justin Patterson\n\n\
					  AI Programmer\n\
					  Justin Patterson\n\n\
					  Particle Software Engineer\n\
					  Matthew Salow\n\n\
					  Animation Software Engineer\n\
					  James Sylvester\n\
					  Ryan Simmons\n\n\
					  Game Core\n\
					  Justin Mazzola\n\n\
					  UI Programmer\n\
					  Justin Mazzola\n\n\
					  Mercenary Programmer\n\
					  Ryan Simmons\n\n\
					  Canadian Linguist\n\
					  Jordan Scelsa\n\n\
					  Artists\n\
					  Gregory Bey\n\
					  Caris Frazier\n\
					  Justin Mazzola\n\
					  Justin Patterson\n\n\
					  Special Thanks\n\
					  Jordan Butler for ideas.\n\
					  Ryan Simmons' Bookbag";

	// If we're transitioning
	if (IsTransitioning())
	{
		// Draw the background coming from the bottom - up
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 800 / TRANSITION_TIME * m_fTransitionTime });
	}
	else
	{
		// Draw the background
		pGraphics->DrawTexture(m_hBackground, { 0, 0 });

		m_pFont->Draw(credits, (int)textPosition.x, (int)textPosition.y, 0.5f, { 255, 0, 0 });

		// Draw rectangles to cut off the words to give an illusion of margins
		pGraphics->DrawTextureSection(m_hBackground, { 0, 0 },
			SGD::Rectangle(0, 0, 800, 228), {}, {});

		pGraphics->DrawTextureSection(m_hBackground, { 0, 475 },
			SGD::Rectangle(0, 475, 800, 600), {}, {});

		// Render button
		m_pMainButton->Draw("Go Back", { 200, 500 }, { 255, 0, 0 }, { 1, 1 }, 0);
	}
}

/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* CreditsState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}