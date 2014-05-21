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
/*static*/ Game* Game::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the ONE instance
/*static*/ void Game::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/**************************************************************/
// Initialize
//	- initialize the SGD wrappers
//	- load resources / assets
//	- allocate & initialize the game entities
bool Game::Initialize( int width, int height )
{
	// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();


	// Store the size parameters
	m_nScreenWidth	= width;
	m_nScreenHeight = height;

	// Store the SGD singletons
	m_pAudio	= SGD::AudioManager::GetInstance();
	m_pGraphics	= SGD::GraphicsManager::GetInstance();
	m_pInput	= SGD::InputManager::GetInstance();
	
	// Initialize each singleton
	if( m_pAudio->Initialize() == false 
	   || m_pGraphics->Initialize( false ) == false 
	   || m_pInput->Initialize() == false )
	{
		return false;
	}


	// Allocate & initialize the font
	   m_pFont = new BitmapFont;
	   m_pFont->Initialize("resource/images/fonts/BitmapFont_Roboto_0.png","resource/data/BitmapFont_Roboto.fnt");

	// Start the game in the Main Menu state
	ChangeState( MainMenuState::GetInstance() );
	

	// Store the current time (in milliseconds)
	m_ulGameTime = GetTickCount();

	// Testing the loading for the animation
	m_pAnimation = AnimationManager::GetInstance();
	m_pAnimation->LoadSprites("resource/animation/piggy2.xml");
	
	
	return true;	// success!
}


/**************************************************************/
// Main
//	- update the SGD wrappers
//	- update the game entities
//	- render the game entities
int Game::Main( void )
{
	if( m_pAudio->Update() == false 
	   || m_pGraphics->Update() == false 
	   || m_pInput->Update() == false )
	{
		return -10;		// abort!
	}

	// Current time
	unsigned long now = GetTickCount();
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;
	m_ulGameTime = now;

	// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;
	

	// Let the current state handle input
	if( m_pCurrState->Input() == false )
		return 1;	// exit success!


	// Testing the rending for the animation
	m_pAnimation->m_sAnimationTS.m_nCurrAnimation = "running";
	m_pAnimation->m_sAnimationTS.m_nCurrFrame = 0;
	m_pAnimation->Render(m_pAnimation->m_sAnimationTS, 0, 0);



	// Update & render the current state
	m_pCurrState->Update( elapsedTime );
	m_pCurrState->Render();
	
	return 0;		// keep playing!
}


/**************************************************************/
// Terminate
//	- deallocate game entities
//	- unload resources / assets
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	// Exit the current state
	ChangeState( nullptr );

	// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

	m_pAnimation->UnloadSprites();
	m_pAnimation = nullptr;
	AnimationManager::DeleteInstance();

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
void Game::ChangeState( IGameState* pNewState )
{
	// Exit the old state
	if( m_pCurrState != nullptr)
		m_pCurrState->Exit();

	// Store the new state
	m_pCurrState = pNewState;

	// Enter the new state
	if( m_pCurrState != nullptr)
		m_pCurrState->Enter();
}
