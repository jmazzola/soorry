/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Justin Mazzola
|	Course:		
|	Purpose:	
***************************************************************/

#include "GameplayState.h"

#include "Game.h"
#include "OptionsState.h"
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
#include "Player.h"
#include "ParticleManager.h"
#include "WorldManager.h"

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
/*static*/ GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/*************************************************************/
// CreatePlayer
//	- allocate a new player
///	- set the player's properties
Entity*	GameplayState::CreatePlayer() const
{
	Player* player = new Player();
	return player;
}


/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void GameplayState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	// Initialize the Event Manager
	m_pEvents = SGD::EventManager::GetInstance();
	m_pEvents->Initialize();

	// Initialize the Message Manager
	m_pMessages = SGD::MessageManager::GetInstance();
	m_pMessages->Initialize(&MessageProc);


	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Load Tim
	m_hPlayerImg = pGraphics->LoadTexture(L"resource/images/tim/tim.png");

	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	
	//Load Particle Manager
	m_pParticleManager = ParticleManager::GetInstance();
	//Load preset xml file
	m_pParticleManager->createEmitter("test_particle", "resource/world/testparticle.xml");
	//load emitters
	m_pParticleManager->load();
	// Set background color
	//SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black


	// Create our player
	m_pPlayer = CreatePlayer();
	// Add it to the entity manager
	m_pEntities->AddEntity(m_pPlayer, Entity::ENT_PLAYER);




	// Load the world
	WorldManager::GetInstance()->LoadWorld("resource/world/colWorld.xml");


	// Load wave information
	zombieFactory.LoadWaves("resource/data/wave.xml");
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	//Matt gets rid of the memory leaks
	m_pParticleManager->unload();


	// Release the player
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}

	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	// Unload Assets
	pGraphics->UnloadTexture(m_hPlayerImg);


	// Unload World Manager
	WorldManager::GetInstance()->UnloadWorld();


	m_pMessages->Terminate();
	m_pMessages = nullptr;
	SGD::MessageManager::DeleteInstance();


	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool GameplayState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape))
		return false;

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void GameplayState::Update(float elapsedTime)
{

	// Update the entities
	m_pEntities->UpdateAll(elapsedTime);
	m_pParticleManager->Update(elapsedTime);

	// Process the events & messages
	m_pEvents->Update();
	m_pMessages->Update();


	// Check collisions
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render the background


	// Render test world
	WorldManager::GetInstance()->Render(SGD::Point(0,0));
	
#if _DEBUG
	pGraphics->DrawString("Gameplay State | Escape to Quit", { 240, 0 }, { 255, 0, 255 });
#endif

	//Render test particles
	m_pParticleManager->Render();

	// Render the entities
	m_pEntities->RenderAll();
}


/**************************************************************/
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD
//		- does NOT have invoking object!!!
//		- must use singleton to access members
/*static*/ void GameplayState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_UNKNOWN:
	default:
		OutputDebugStringW(L"Game::MessageProc - unknown message id\n");
		break;
	}


	/* Restore previous warning levels */
#pragma warning( pop )

}


/**************************************************************/
// Factory Methods
