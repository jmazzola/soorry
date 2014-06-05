/***************************************************************
|	File:		Game.cpp
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "AnimationManager.h"
#include "Sprite.h"
#include "Frame.h"

#include "BitmapFont.h"
#include "IGameState.h"

#include "MainMenuState.h"
#include "GameplayState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "IntroState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


/**************************************************************/
// Singleton
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the ONE instance & return it
/*static*/ Game* Game::GetInstance(void)
{
	if (s_pInstance == nullptr)
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


void Game::Transition(IGameState* to)
{
	if (m_pTransState)
		return;

	m_pTransState = to;
	to->Enter();
	to->SetTransition(true);
}


/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- load resources / assets
//	- allocate & initialize the game entities
bool Game::Initialize(int width, int height)
{
	// Seed First!
	srand((unsigned int)time(nullptr));
	rand();


	// Store the size parameters
	m_nScreenWidth = width;
	m_nScreenHeight = height;

	// Store the SGD singletons
	m_pAudio = SGD::AudioManager::GetInstance();
	m_pGraphics = SGD::GraphicsManager::GetInstance();
	m_pInput = SGD::InputManager::GetInstance();

	// Initialize each singleton
	if (m_pAudio->Initialize() == false
		|| m_pInput->Initialize() == false
		|| m_pGraphics->Initialize(false) == false
		)
	{
		return false;
	}

	// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize("resource/images/fonts/BitmapFont_Roboto_0.png", "resource/data/BitmapFont_Roboto.fnt");

	// Start the game in the Intro state
	ChangeState(IntroState::GetInstance());


	// Store the current time (in milliseconds)
	m_ulGameTime = GetTickCount();


	return true;	// success!
}


/**************************************************************/
// Main
//	- update the SGD wrappers
//	- update the game entities
//	- render the game entities
int Game::Main(void)
{
	if (m_pAudio->Update() == false
		|| m_pGraphics->Update() == false
		|| m_pInput->Update() == false)
	{
		return -10;		// abort!
	}

	// Current time
	unsigned long now = GetTickCount();
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;
	m_ulGameTime = now;

	// Cap the elapsed time to 1/8th of a second
	if (elapsedTime > 0.125f)
		elapsedTime = 0.125f;

	// Toggle fullscreen

	if (m_pInput->IsKeyDown(SGD::Key::Alt) &&
		m_pInput->IsKeyPressed(SGD::Key::Enter))
	{
		ToggleFullscreen();
		// Disable the 'Enter' input
		return false;
	}
	if(m_pInput->IsControllerConnected(0))
	{
		// Grab the input from the right joystick
		SGD::Vector mouseUpdate = {0.0f, 0.0f};
		mouseUpdate = m_pInput->GetRightJoystick(0);

		// Clamp the input to be greater than .1 in either direction
		// so the mouse doesn't drift even when not being used
		if(abs(mouseUpdate.x) < 0.2f)
			mouseUpdate.x = 0.0f;
		if(abs(mouseUpdate.y) < 0.2f)
			mouseUpdate.y = 0.0f;

		POINT mouse;
		RECT window;
		int X, Y;
		X = 0;
		Y = 0;

		HWND hWnd = m_pGraphics->GetHWND();

		// Grab the client rectangle
		GetWindowRect(hWnd, &window);

		// Grab the mouse's actual position
		GetCursorPos(&mouse);
		X = (int)(mouse.x + mouseUpdate.x);
		Y = (int)(mouse.y + mouseUpdate.y);
		

		// Clamp the mouse position to be inside the client window
		if ( X < window.left + 8 && m_bFullscreen == false)
			X = window.left + 8;
		else if(X < window.left && m_bFullscreen == true)
			X = window.left;

		if ( X > window.right - 8 && m_bFullscreen == false)
			X = window.right - 8;
		else if(X > window.right && m_bFullscreen == true)
			X = window.right;

		if ( Y < window.top + 28 && m_bFullscreen == false)
			Y = window.top + 28;
		else if(Y < window.top && m_bFullscreen == true)
			Y = window.top;

		if ( Y > window.bottom - 8 && m_bFullscreen == false)
			Y = window.bottom - 8;
		else if( Y > window.bottom && m_bFullscreen == true)
			Y = window.bottom;

		// Set the cursor position based on input and inside the window
		if(GetActiveWindow() == hWnd)
		SetCursorPos(X, Y);
	}

	// Let the current state handle input
	if (m_pCurrState->Input() == false)
		return 1;	// exit success!

	// Testing the rending for the animation
	//m_pAnimation->m_sAnimationTS.m_nCurrAnimation = "running";
	//m_pAnimation->m_sAnimationTS.m_nCurrFrame = 0;
	//m_pAnimation->Render(m_pAnimation->m_sAnimationTS, 0, 0);

	// Update & render the current state
	if (!m_pTransState)
		m_pCurrState->Update(elapsedTime);

	m_pCurrState->Render();

	if (m_pTransState)
	{
		m_pTransState->Update(elapsedTime);
		m_pTransState->Render();

		if (m_pTransState->IsTransitioning() == false)
		{
			m_pCurrState->Exit();
			m_pCurrState = m_pTransState;
			m_pTransState = nullptr;
		}
	}

	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- deallocate game entities
//	- unload resources / assets
//	- terminate the SGD wrappers
void Game::Terminate(void)
{
	// Exit the current state
	ChangeState(nullptr);

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;


	// Terminate & deallocate the SGD wrappers
	m_pAudio->Terminate();
	m_pAudio = nullptr;
	SGD::AudioManager::DeleteInstance();

	m_pGraphics->Terminate();
	m_pGraphics = nullptr;
	SGD::GraphicsManager::DeleteInstance();

	m_pInput->Terminate();
	m_pInput = nullptr;
	SGD::InputManager::DeleteInstance();

}

/**************************************************************/
// ChangeState
//	- exit the current state to release resources
//	- enter the new state to allocate resources
//	- DANGER! Exiting the current state can CRASH the program!
//	  The state can ONLY be exited from the
//	  Input, Update, and Render methods!!!
void Game::ChangeState(IGameState* pNewState)
{
	// Exit the old state
	if (m_pCurrState != nullptr)
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	// Enter the new state
	if (m_pCurrState != nullptr)
		m_pCurrState->Enter();
}
