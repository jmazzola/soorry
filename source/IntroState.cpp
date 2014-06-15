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

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hLogo = pGraphics->LoadTexture("resource/images/intro/razorballoon.png");
	m_hStudioLogo = pGraphics->LoadTexture("resource/images/intro/studiologo.png");


	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black

	// Hide the cursor for the intro
	if(pGraphics->IsCursorShowing() == true)
		pGraphics->TurnCursorOff();
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
	pGraphics->UnloadTexture(m_hStudioLogo);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	/*m_pMessages->Terminate();
	m_pMessages = nullptr;
	SGD::MessageManager::DeleteInstance();*/

	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();
	
	if(pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();
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
	if (pInput->IsAnyKeyPressed() || pInput->IsButtonPressed(0, 0) || pInput->IsButtonPressed(0, 1) ||
		 pInput->IsButtonPressed(0, 2) || pInput->IsButtonPressed(0, 3) || pInput->IsButtonPressed(0, 4) ||
		  pInput->IsButtonPressed(0, 5) || pInput->IsButtonPressed(0, 6) || pInput->IsButtonPressed(0, 7))
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

	// Render the text to press Space to skip in bottom right corner WITH 90% SPACE!!!!!!!!11111111111
	m_pFont->Draw("Press Any Key to Skip", 550, 500, 0.4f, { 255, 0, 0 });

	// Run the animation

	// -- Show Developer Logo --
	// If it's been between 0 and 1.5 seconds
	if (m_fTimeRemaining <= 1.5f)
	{
		// If we are already at full alpha
		if (m_fTimeRemaining >= 0.95f)
		{
			// Show "Developed by"
			m_pFont->Draw("Developed by", 320, 80, 0.6f, { 255, 0, 0 });
			// Show the logo
			pGraphics->DrawTexture(m_hLogo, { 300, 128 }, 0, {}, { 255, 255, 255 }, { 0.6f, 0.6f });
		}
		else
		{
			// Fade in text
			m_pFont->Draw("Developed by", 320, 80, 0.6f, SGD::Color(char(m_fTimeRemaining * 255.0f), 255, 0, 0));
			// Fade in the logo
			pGraphics->DrawTexture(m_hLogo, { 300, 128 }, 0, {}, SGD::Color(char(m_fTimeRemaining * 255.0f), 255, 255, 255), { 0.6f, 0.6f });
		}
	}
	// If it's between 1.5 seconds and 2 seconds
	else if (m_fTimeRemaining >= 1.5f && m_fTimeRemaining < 2.0f)
	{
		// Fade out text
		m_pFont->Draw("Developed by", 320, 80, 0.6f, SGD::Color(char(255.0f - char(m_fTimeRemaining * 255.0f)), 255, 0, 0));
		// Fade out the logo
		pGraphics->DrawTexture(m_hLogo, { 300, 128 }, 0, {}, SGD::Color(char(255.0f - char(m_fTimeRemaining * 255.0f)), 255, 255, 255), { 0.6f, 0.6f });
	}
	

	// -- Show Studio Logo --
	// If it's between 2.1 seconds and 3 seconds
	else if (m_fTimeRemaining >= 2.1f && m_fTimeRemaining < 3.0f)
	{
		// If we are already at full alpha
		if (m_fTimeRemaining >= 2.99f)
		{
			// Show "Produced by"
			m_pFont->Draw("Produced by", 320, 80, 0.6f, { 255, 0, 0 });
			// Show the studio logo
			pGraphics->DrawTexture(m_hStudioLogo, { 300, 128 }, 0, {}, { 255, 255, 255 }, { 0.6f, 0.6f });
		}
		else
		{
			// Fade in text
			m_pFont->Draw("Produced by", 320, 80, 0.6f, SGD::Color(char((m_fTimeRemaining - 2.1f) * 255.0f), 255, 0, 0));
			// Fade in the studio logo
			pGraphics->DrawTexture(m_hStudioLogo, { 300, 128 }, 0, {}, SGD::Color(char((m_fTimeRemaining - 2.1f) * 255.0f), 255, 255, 255), { 0.6f, 0.6f });
		}
	}
	else if (m_fTimeRemaining >= 3.0f && m_fTimeRemaining <= 4.0f)
	{
		// Fade out text
		m_pFont->Draw("Produced by", 320, 80, 0.6f, SGD::Color(char(255.0f - char((m_fTimeRemaining - 3.0f) * 255.0f)), 255, 0, 0));
		// Fade out the logo
		pGraphics->DrawTexture(m_hStudioLogo, { 300, 128 }, 0, {}, SGD::Color(char(255.0f - char((m_fTimeRemaining - 3.0f) * 255.0f)), 255, 255, 255), { 0.6f, 0.6f });
	}
	else if (m_fTimeRemaining >= 4.5f)
	{
		// Go to the main menu
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}
}


/**************************************************************/
// Factory Methods
