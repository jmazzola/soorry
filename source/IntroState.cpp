/***************************************************************
|	File:		IntroState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will play an animation or video that
|				attracts the player during idling OR when the game
|				is first run.
***************************************************************/

#include "IntroState.h"

#include "Game.h"
#include "MainMenuState.h"

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
/*static*/ IntroState* IntroState::GetInstance(void)
{
	static IntroState s_Instance;	// stored in global memory once
	return &s_Instance;
}


/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void IntroState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	// Initialize the Event Manager
	m_pEvents = SGD::EventManager::GetInstance();
	m_pEvents->Initialize();

	// Initialize the Message Manager
	//m_pMessages = SGD::MessageManager::GetInstance();
	//m_pMessages->Initialize(&MessageProc);

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;


	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hLogo = pGraphics->LoadTexture("resource/images/intro/razorballoon.png");


	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void IntroState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hLogo);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;


	/*m_pMessages->Terminate();
	m_pMessages = nullptr;
	SGD::MessageManager::DeleteInstance();*/

	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool IntroState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Press Space to skip to the Main Menu at any time
	if (pInput->IsKeyPressed(SGD::Key::Space))
	{
		pGame->ChangeState(MainMenuState::GetInstance());
	}


	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void IntroState::Update(float elapsedTime)
{
	// Update the counter
	m_fTimeRemaining += elapsedTime;


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
/*virtual*/ void IntroState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render the text to press Space to skip in bottom right corner
	m_pFont->Draw("Press Space to Skip", 550, 500, 0.4f, { 255, 0, 0 });

	// Run the really ghetto and janky animation

	// If it's been between 0 and 5 seconds
	if (m_fTimeRemaining <= 4.99f)
	{
		// Draw the logo
		pGraphics->DrawTexture(m_hLogo, { 200, 128 }, 0, {}, SGD::Color((char)m_fTimeRemaining * 51, 255, 255, 255));
	}
	else if (m_fTimeRemaining >= 5.0f && m_fTimeRemaining <= 10.0f)
	{
		//
	}


	// Render the entities
	m_pEntities->RenderAll();
}


/**************************************************************/
// Factory Methods
