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

#define SCROLL_SPEED 0.04f;


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
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/CreditsBG.png");


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
/*virtual*/ bool CreditsState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
	{
		// Since there's only one state..go back to main menu
		pGame->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void CreditsState::Update(float elapsedTime)
{


	// Update the entities
	m_pEntities->UpdateAll(elapsedTime);


	// Process the events & messages
	m_pEvents->Update();
	//m_pMessages->Update();


	// Check collisions

	// Move the credits
	textPosition.x = 220;
	textPosition.y -= SCROLL_SPEED;

}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void CreditsState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Draw the background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 });

	// Draw the credits
	// TODO: Load in a text file
	string credits = "SOORRY\n\n\
					 By Razor Balloon\n\n\
					 Part of Heavy Square Studios\n\n\
					 Associate Producer\n\
					 Sean Hathaway\n\n\
					 Executive Producer\n\
					 John O' Leske\n\n\
					 World Software Engineer\n\
					 Justin Patterson\n\n\
					 AI Programmer\n\
					 Justin Patterson\n\n\
					 Particle Software Engineer\n\
					 Matthew Salow\n\n\
					 Animation Software Engineer\n\
					 James Sylvester\n\n\
					 Game Core\n\
					 Justin Mazzola\n\n\
					 UI Programmer\n\
					 Justin Mazzola\n\n\
					 Artists\n\
					 Gregory Bey\n\
					 Caris Frazier\n\
					 Justin Mazzola\n\n\
					 Special Thanks\n\
					 Jordan Butler for ideas.";

	m_pFont->Draw(credits, (int)textPosition.x, (int)textPosition.y, 0.5f, { 255, 0, 0 });

	// Warning: SUPER JIT. THIS IS REALLY GHETTO.
	// Draw rectangles to cut off the words to give an illusion of margins
	pGraphics->DrawTextureSection(m_hBackground, { 0, 0 },
		SGD::Rectangle(0, 0, 800, 228), {}, {});

	pGraphics->DrawTextureSection(m_hBackground, { 0, 475 },
		SGD::Rectangle(0, 475, 800, 600), {}, {});

	// Render button
	m_pMainButton->Draw("Go Back", { 200, 500 }, { 255, 0, 0 }, { 1, 1 }, 0);

	// Render the entities
	m_pEntities->RenderAll();
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