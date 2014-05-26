/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Justin Mazzola
***************************************************************/

#include "GameplayState.h"

#include "Game.h"
#include "OptionsState.h"
#include "MainMenuState.h"
#include "Button.h"
#include "Camera.h"

#include "Shop.h"
#include "Weapon.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "Sprite.h"
//Message Includes
#include "CreateBeaverZombieMessage.h"
#include "CreateFastZombieMessage.h"
#include "CreateSlowZombieMessage.h"
#include "CreateProjectileMessage.h"
#include "CreatePlaceableMessage.h"
#include "CreatePickupMessage.h"
#include "DestroyEntityMessage.h"
#include "CreatePlayerSpawnMessage.h"
//Object Includes
#include "BeaverZombie.h"
#include "FastZombie.h"
#include "SlowZombie.h"
#include "ShotgunPellet.h"
#include "Rocket.h"
#include "AssaultRifleBullet.h"

#include "MessageID.h"
#include "BitmapFont.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "ParticleManager.h"
#include "WorldManager.h"
#include "Mine.h"
#include "BearTrap.h"
#include "Camera.h"
#include "WallPickup.h"
#include "WindowPickup.h"

#include <cstdlib>
#include <cassert>
#include <sstream>
using namespace std;


// Buckets
#define BUCKET_PLAYER 0
#define BUCKET_ENEMIES 1
#define BUCKET_PROJECTILES 2
#define BUCKET_PLACEABLE 3
#define BUCKET_PICKUP 4

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


EntityManager* GameplayState::GetEntityManager() const
{
	return GetInstance()->m_pEntities;
}


/*************************************************************/
// CreatePlayer
//	- allocate a new player
///	- set the player's properties
Entity*	GameplayState::CreatePlayer() const
{
	Player* player = new Player();
	player->SetPosition(m_ptPlayerSpawnPoint);
	player->SetZombieFactory(zombieFactory);
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
	m_hBackgroundMus = pAudio->LoadAudio(L"resource/audio/JPM_LightsAndSounds.xwm");

	//Load Particle Manager
	m_pParticleManager = ParticleManager::GetInstance();
	//Load preset xml file
	//m_pParticleManager->createEmitter("test_particle", "resource/world/testparticle.xml");
	//load emitters
	//m_pParticleManager->load();
	// Set background color
	//SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black

	// Camera
	Camera::x = 0;
	Camera::y = 0;

	// Load all animation
	m_pAnimation = AnimationManager::GetInstance();
	m_pAnimation->LoadAll();

	// Load the world
	WorldManager* pWorld = WorldManager::GetInstance();
	pWorld->LoadWorld("resource/world/colWorld.xml");

	// Start Zombie Factory
	zombieFactory = new ZombieFactory;
	zombieFactory->LoadWaves("resource/data/wave.xml");
	zombieFactory->Start();
	zombieFactory->SetSpawnWidth(pWorld->GetWorldWidth() * pWorld->GetTileWidth());
	zombieFactory->SetSpawnHeight(pWorld->GetWorldHeight() * pWorld->GetTileHeight());
	zombieFactory->SetEntityManager(m_pEntities);

	// Create our player
	m_pPlayer = CreatePlayer();

	// Add it to the entity manager
	m_pEntities->AddEntity(m_pPlayer, BUCKET_PLAYER);

	// Load the world
	WorldManager::GetInstance()->LoadWorld("resource/world/colWorld.xml");

	// Load wave information
	zombieFactory.LoadWaves("resource/data/wave.xml");

	// Start Zombie Factory
	zombieFactory.Start();
	//// Add it to the entity manager
	// Load pause menu background
	m_hPauseMainBackground = pGraphics->LoadTexture("resource/images/menus/PausedBG.png");
	m_hPauseOptionsBackground = pGraphics->LoadTexture("resource/images/menus/OptionsBG.png");

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Setup the main button (text)
	m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);

	// Load Store
	m_pShop = new Shop;
	m_pShop->SetShopStatus(false);
	m_pShop->Enter(m_pPlayer);

	// Load menu stuff
	m_nPauseMenuCursor = PauseMenuOption::PAUSE_RESUME;
	m_nPauseMenuTab = PauseMenuTab::TAB_MAIN;
	m_bIsPaused = false;
	// Play the background music
	//pAudio->PlayAudio(m_hBackgroundMus, true);

	OptionsState::GetInstance()->LoadOptions("resource/data/config.xml");
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();


	m_pAnimation->UnloadSprites();
	m_pAnimation = nullptr;
	AnimationManager::DeleteInstance();

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	pAudio->UnloadAudio(m_hBackgroundMus);

	//Matt gets rid of the memory leaks
	m_pParticleManager->unload();

	// Delete the zombie factory
	delete zombieFactory;

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

	// Unload pause menu
	pGraphics->UnloadTexture(m_hPauseMainBackground);
	pGraphics->UnloadTexture(m_hPauseOptionsBackground);

	// Make sure pause isn't set
	m_bIsPaused = false;


	// Unload World Manager
	WorldManager::GetInstance()->UnloadWorld();


	m_pMessages->Terminate();
	m_pMessages = nullptr;
	SGD::MessageManager::DeleteInstance();

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;

	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();


	// Terminate & deallocate shop
	m_pShop->Exit();
	delete m_pShop;
	m_pShop = nullptr;

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

	// Press Escape (PC) or Start (Xbox 360) to toggle pausing
	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::Start))
	{
		if (m_pShop->IsOpen() == false)
			m_bIsPaused = !m_bIsPaused;
	}

	if (pInput->IsKeyPressed(SGD::Key::Backspace))
	{
		m_pShop->SetShopStatus(true);
	}

	if (pInput->IsKeyPressed(SGD::Key::Z))
	{
		CreateBeaverZombieMessage* msg = new CreateBeaverZombieMessage(0, 0);
		msg->QueueMessage();
		msg = nullptr;
	}
	if (pInput->IsKeyPressed(SGD::Key::X))
	{
		CreateSlowZombieMessage* msg = new CreateSlowZombieMessage(0, 0);
		msg->QueueMessage();
		msg = nullptr;
	}
	if (pInput->IsKeyPressed(SGD::Key::C))
	{
		CreateFastZombieMessage* msg = new CreateFastZombieMessage(0, 0);
		msg->QueueMessage();
		msg = nullptr;
	}
#pragma region Pause Menu Navigation Clutter
	// Handle pause menu input
	// If we're paused
	if (m_bIsPaused)
	{
		// --- Scrolling through options ---
		// If the down arrow (PC), or down dpad (Xbox 360) are pressed
		// Move the cursor (selected item) down
		if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
		{
			// TODO: Add sound fx for going up and down
			++m_nPauseMenuCursor;

			// Wrap around the options
			if (m_nPauseMenuCursor > PauseMenuOption::PAUSE_EXIT)
				m_nPauseMenuCursor = PauseMenuOption::PAUSE_RESUME;
		}
		// If the up arrow (PC), or up dpad (Xbox 360) are pressed
		// Move the cursor (selected item) up
		else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
		{
			--m_nPauseMenuCursor;

			// Wrap around the options
			if (m_nPauseMenuCursor < PauseMenuOption::PAUSE_RESUME)
				m_nPauseMenuCursor = PauseMenuOption::PAUSE_EXIT;
		}

		//-----------------------------------------------------------------------
		// --- Handling what tab we're in ---
		// If we're in the Main menu OF the pause menu. 
		if (m_nPauseMenuTab == PauseMenuTab::TAB_MAIN)
		{
			// --- Selecting an option ---
			// If the enter key (PC) or A button (Xbox 360) are pressed
			// Select the item
			if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
			{
				// Switch table for the item selected
				switch (m_nPauseMenuCursor)
				{
				case PauseMenuOption::PAUSE_RESUME:
				{
					// Resume gameplay
					m_bIsPaused = false;
					break;
				}
					break;

				case PauseMenuOption::PAUSE_OPTION:
				{
					// Set the cursor to the first option in the options tab
					m_nPauseMenuCursor = PauseMenuOptionsOption::OPTION_MUSIC;
					// Go to the options tab
					m_nPauseMenuTab = PauseMenuTab::TAB_OPTION;
					// Load the options
					OptionsState::GetInstance()->LoadOptions("resource/data/config.xml");
					break;
				}
					break;

				case PauseMenuOption::PAUSE_EXIT:
				{
					//Go to Main Menu
					pGame->ChangeState(MainMenuState::GetInstance());
					// Exit immediately
					return true;
				}
					break;
				}
			}
		}
		// If we're in the main menu's options tab
		else if (m_nPauseMenuTab == PauseMenuTab::TAB_OPTION)
		{
			// --- Increasing an option ---
			// If the right key (PC) or right dpad (Xbox 360) are pressed
			// Increase the value
			if (pInput->IsKeyPressed(SGD::Key::Right) || pInput->IsDPadPressed(0, SGD::DPad::Right))
			{
				switch (m_nPauseMenuCursor)
				{
				case PauseMenuOptionsOption::OPTION_MUSIC:
				{
					// Increase the music volume += 5
					pAudio->SetMasterVolume(SGD::AudioGroup::Music, pAudio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
				}
					break;

				case PauseMenuOptionsOption::OPTION_SFX:
				{
					// Increase the sound effects volume += 5
					pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
				}
					break;
				}
			}
			// --- Decreasing an option ---
			// If the left key (PC) or left dpad (Xbox 360) are pressed
			// Decrease the value
			if (pInput->IsKeyPressed(SGD::Key::Left) || pInput->IsDPadPressed(0, SGD::DPad::Left))
			{
				switch (m_nPauseMenuCursor)
				{
				case PauseMenuOptionsOption::OPTION_MUSIC:
				{
					// Increase the music volume -= 5
					pAudio->SetMasterVolume(SGD::AudioGroup::Music, pAudio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
				}
					break;

				case PauseMenuOptionsOption::OPTION_SFX:
				{
					// Increase the sound effects volume -= 5
					pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);
				}
					break;
				}
			}

			// --- Selecting an option ---
			// If the enter key (PC) or A button (Xbox 360) are pressed
			// Select the item
			if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
			{
				switch (m_nPauseMenuCursor)
				{
				case PauseMenuOptionsOption::OPTION_GOBACK:
				{
					// Go back to the pause menu's main menu
					m_nPauseMenuTab = PauseMenuTab::TAB_MAIN;
					// Make the highlighted option 'Options'
					m_nPauseMenuCursor = PauseMenuOption::PAUSE_OPTION;
					// Save options
					OptionsState::GetInstance()->SaveOptions("resource/data/config.xml");

					break;
				}
					break;
				}
			}
		}

	}
#pragma endregion



	if (m_pShop->IsOpen())
		m_pShop->Input();

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void GameplayState::Update(float elapsedTime)
{
	// Grab the controllers
	//SGD::InputManager::GetInstance()->CheckForNewControllers();

	// If the game isn't paused
	if (!m_bIsPaused || m_pShop->IsOpen() == false)
	{
		// Update the entities
		m_pEntities->UpdateAll(elapsedTime);
		m_pParticleManager->Update(elapsedTime);

		// Process the events & messages
		m_pEvents->Update();
		m_pMessages->Update();

		// Update Zombie Factory
		zombieFactory->Update(elapsedTime);

		// Check collisions
		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PICKUP);
	}
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render test world
	WorldManager::GetInstance()->Render(SGD::Point((float)Camera::x, (float)Camera::y));

#if _DEBUG
	pGraphics->DrawString("Gameplay State | Debugging", { 240, 0 }, { 255, 0, 255 });

	Player* player = dynamic_cast<Player*>(m_pPlayer);
	// Draw money
	int money = player->GetScore();
	string moneyy = "Current Money: " + std::to_string(money);
	pGraphics->DrawString(moneyy.c_str(), { 100, 50 });

	// Draw weapons
	for (int i = 0; i < 4; i++)
	{
		Weapon* weapons = player->GetWeapons();
		string weaponAmmo = "Weapon " + std::to_string(i); + " :  ";
		weaponAmmo += std::to_string(weapons[i].GetCurrAmmo()).c_str();
		pGraphics->DrawString(weaponAmmo.c_str(), SGD::Point(200, 100 + i * 20 ));
		weaponAmmo.clear();
	}
#endif

	//Render test particles
	m_pParticleManager->Render();

	// Render the entities
	m_pEntities->RenderAll();

	// --- Pause Menu stuff ---
	// If we're paused
	if (m_bIsPaused)
	{
		if (m_nPauseMenuTab == PauseMenuTab::TAB_MAIN)
		{
			// Draw the paused main menu background
			pGraphics->DrawTexture(m_hPauseMainBackground, { 0, 0 });

			// Draw the options
			if (m_nPauseMenuCursor == PauseMenuOption::PAUSE_RESUME)
				m_pMainButton->Draw("Resume Game", { 170, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Resume Game", { 170, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

			if (m_nPauseMenuCursor == PauseMenuOption::PAUSE_OPTION)
				m_pMainButton->Draw("Options", { 150, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Options", { 150, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

			if (m_nPauseMenuCursor == PauseMenuOption::PAUSE_EXIT)
				m_pMainButton->Draw("Exit Game", { 165, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Exit Game", { 165, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}
		else if (m_nPauseMenuTab == PauseMenuTab::TAB_OPTION)
		{
			// Draw the paused menu option's background
			pGraphics->DrawTexture(m_hPauseOptionsBackground, { 0, 0 });

			// Create the string for the button
			string musicVol = "Music Vol: ";
			// Grab the volume
			int musicVolValue = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
			// Add it to the string since C++ doesn't support [ string + "" ] 
			musicVol.append(std::to_string(musicVolValue));

			// Same stuff here for the sfx vol
			string sfxVol = "SFX Vol: ";
			int sfxVolValue = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			sfxVol.append(std::to_string(sfxVolValue));

			if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_MUSIC)
				m_pMainButton->Draw(musicVol, { 140, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw(musicVol, { 140, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

			if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_SFX)
				m_pMainButton->Draw(sfxVol, { 120, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw(sfxVol, { 120, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

			if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_GOBACK)
				m_pMainButton->Draw("Go Back", { 160, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
			else
				m_pMainButton->Draw("Go Back", { 160, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
		}

	}

	// If we're shopping
	if (m_pShop->IsOpen())
	{
		m_pShop->Render();
	}
	// Draw wave info
	if (zombieFactory->IsBuildMode())
	{
		string timeRemaining = "Time remaining: ";
		timeRemaining.append(std::to_string(zombieFactory->GetBuildTimeRemaining()));
		pGraphics->DrawString(timeRemaining.c_str(), { 0, 0 });
	}

	else
	{
		string enemiesRemaining = "Enemies Remaining: ";
		enemiesRemaining.append(std::to_string(zombieFactory->GetEnemiesRemaining()));
		pGraphics->DrawString(enemiesRemaining.c_str(), { 0, 0 });
	}
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
	case MessageID::MSG_CREATE_BEAVER_ZOMBIE:
	{
		const CreateBeaverZombieMessage* pCreateMessage = dynamic_cast<const CreateBeaverZombieMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*beaver = self->CreateBeaverZombie(pCreateMessage->GetX(), pCreateMessage->GetY());
		self->m_pEntities->AddEntity(beaver, 1);
		beaver->Release();
		beaver = nullptr;
	}
		break;
	case MessageID::MSG_CREATE_FAST_ZOMBIE:
	{
		const CreateFastZombieMessage* pCreateMessage = dynamic_cast<const CreateFastZombieMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*zambie = self->CreateFastZombie(pCreateMessage->GetX(), pCreateMessage->GetY());
		self->m_pEntities->AddEntity(zambie, 1);
		zambie->Release();
		zambie = nullptr;
	}
		break;
	case MessageID::MSG_CREATE_SLOW_ZOMBIE:
	{
		const CreateSlowZombieMessage* pCreateMessage = dynamic_cast<const CreateSlowZombieMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*zambie = self->CreateSlowZombie(pCreateMessage->GetX(), pCreateMessage->GetY());
		self->m_pEntities->AddEntity(zambie, 1);
		zambie->Release();
		zambie = nullptr;
	}
		break;

	case MessageID::MSG_CREATE_PROJECTILE:

	{

		const CreateProjectileMessage* pCreateMessage = dynamic_cast<const CreateProjectileMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*bullet = self->CreateProjectile(pCreateMessage->GetWeaponNumber());
		self->m_pEntities->AddEntity(bullet, BUCKET_PROJECTILES);
		bullet->Release();
		bullet = nullptr;
	}
		break;

	case MessageID::MSG_CREATE_PLACEABLE:
	{
		const CreatePlaceableMessage* pCreateMessage = dynamic_cast<const CreatePlaceableMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* place = g->CreatePlaceable(pCreateMessage->GetPlaceableType());
		g->m_pEntities->AddEntity(place, BUCKET_PLACEABLE);
		place->Release();
		place = nullptr;

	}
		break;

	case MessageID::MSG_CREATE_PICKUP:
	{
										 const CreatePickupMessage* pCreateMessage = dynamic_cast<const CreatePickupMessage*>(pMsg);
										 GameplayState* g = GameplayState::GetInstance();
										 Entity* place = g->CreatePickUp(pCreateMessage->GetPickUpID(), pCreateMessage->GetPosition());
										 g->m_pEntities->AddEntity(place, BUCKET_PICKUP);
										 place->Release();
										 place = nullptr;
	}
		break;
	case MessageID::MSG_CREATE_PLAYER_SPAWN:
	{
											const CreatePlayerSpawnMessage* pCreateMessage = dynamic_cast<const CreatePlayerSpawnMessage*>(pMsg);
											GameplayState* g = GameplayState::GetInstance();
											g->m_ptPlayerSpawnPoint.x = pCreateMessage->GetX();
											g->m_ptPlayerSpawnPoint.y = pCreateMessage->GetY();

	}
		break;
	case MessageID::MSG_DESTROY_ENTITY:
	{

		const DestroyEntityMessage* pCreateMessage = dynamic_cast<const DestroyEntityMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* ent = pCreateMessage->GetEntity();
		g->m_pEntities->RemoveEntity(ent);
	}
		break;
	}

	/* Restore previous warning levels */
#pragma warning( pop )

}


/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* GameplayState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}

Entity* GameplayState::CreateBeaverZombie(int _x, int _y)
{
	BeaverZombie* tempBeav = new BeaverZombie;
	tempBeav->SetDamage(10);
	tempBeav->SetPosition({ (float)_x, (float)_y });
	tempBeav->SetAttackRange(1.0f);
	tempBeav->SetMaxHealth(100);
	tempBeav->SetCurrHealth(100);
	tempBeav->SetSpeed(200.0f);
	tempBeav->SetVelocity({ 0, 0 });

	/*if (tempBeav->GetPosition().x < 0 || tempBeav->GetPosition().x > 10000)
		return nullptr;*/

	// AIComponent
	tempBeav->SetPlayer(m_pPlayer);

	//NOTE: need to render only one image ask james how to do this
	/*Sprite bro;
	bro.SetImage("resource\animation\TestBeaver.png");
	tempBeav->SetSprite(&bro);*/
	return tempBeav;
}

Entity* GameplayState::CreateFastZombie(int _x, int _y)
{
	FastZombie* zambie = new FastZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(100);
	zambie->SetCurrHealth(100);
	zambie->SetSpeed(100.0f);
	zambie->SetVelocity({ 0, 0 });

	/*if (zambie->GetPosition().x < 0 || zambie->GetPosition().x > 10000)
		return nullptr;*/

	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	/*Sprite bro;
	bro.SetImage("resource\images\tim\tim.png");
	zambie->SetSprite(&bro);*/
	return zambie;
}

Entity* GameplayState::CreateSlowZombie(int _x, int _y)
{
	SlowZombie* zambie = new SlowZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(100);
	zambie->SetCurrHealth(100);
	zambie->SetSpeed(50.0f);
	zambie->SetVelocity({ 0, 0 });

	/*if (zambie->GetPosition().x < 0 || zambie->GetPosition().x > 10000)
		return nullptr;*/

	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	/*Sprite bro;
	bro.SetImage("resource\images\tim\tim.png");
	zambie->SetSprite(&bro);*/
	return zambie;
}

Entity* GameplayState::CreatePlaceable(int trap)
{
	if (trap == 0)
	{
		BearTrap* trap = new BearTrap();
		trap->SetTrap(false);
		SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
		pos.x = (pos.x - (int)pos.x % 32) + Camera::x;
		pos.y = (pos.y - (int)pos.y % 32) + Camera::y;
		trap->SetPosition(pos);
		trap->SetSprite(AnimationManager::GetInstance()->GetSprite("crab"));
		trap->SetCurrFrame(0);
		trap->SetTimeOfFrame(0);
		trap->SetCurrAnimation("crab");
		return trap;
	}
	else
	{
		Mine* trap = new Mine();
		trap->SetDamage(30);
		SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
		pos.x = (pos.x - (int)pos.x % 32) + Camera::x;
		pos.y = (pos.y - (int)pos.y % 32) + Camera::y;
		trap->SetPosition(pos);
		trap->SetSprite(AnimationManager::GetInstance()->GetSprite("crab"));
		trap->SetCurrFrame(0);
		trap->SetTimeOfFrame(0);
		trap->SetCurrAnimation("crab");
		return trap;
	}
}


Entity* GameplayState::CreateProjectile(int _Weapon)
{
	switch (_Weapon)
	{
	case 0://Assault Rifle
	{
		AssaultRifleBullet* tempProj = new AssaultRifleBullet;
		tempProj->SetDamage(20);
		tempProj->SetLifeTime(5);
		tempProj->SetPosition(m_pPlayer->GetPosition());
		SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
		pos.x += Camera::x;
		pos.y += Camera::y;
		SGD::Vector vec = m_pPlayer->GetPosition() - pos;
		vec *= -1;
		tempProj->SetVelocity(vec);
		return tempProj;
	}
		break;
	case 1://Shotgun
	{
		ShotgunPellet* tempProj = new ShotgunPellet;
		tempProj->SetDamage(20);
		tempProj->SetLifeTime(5);
		tempProj->SetPosition(m_pPlayer->GetPosition());
		SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
		pos.x += Camera::x;
		pos.y += Camera::y;
		SGD::Vector vec = m_pPlayer->GetPosition() - pos;
		vec *= -1;
		tempProj->SetVelocity(vec);
		return tempProj;
	}
		break;
	case 2://Rocket launcher
	{
		Rocket* tempProj = new Rocket;
		tempProj->SetDamage(150);
		tempProj->SetLifeTime(5);
		tempProj->SetPosition(m_pPlayer->GetPosition());
		SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
		pos.x +=  Camera::x;
		pos.y +=  Camera::y;
		SGD::Vector vec = m_pPlayer->GetPosition() - pos;
		vec *= -1;
		tempProj->SetVelocity(vec);
		Sprite* bro = new Sprite;
		Frame* frame = new Frame;
		frame->SetFrameRect({ 0, 0, 32, 32 });
		frame->SetAnchorPoint({ 0, 0 });
		bro->AddFrame(frame);
		bro->SetImage("resource/images/particles/testParticle1.jpg");
		tempProj->SetSprite(bro);
		return tempProj;
	}
		break;
	case 3://Fire axe?
	{

			   break;
	}
	}
}

Entity* GameplayState::CreatePickUp(int pick, SGD::Point pos)
{
	if (pick == 0)
	{
		WallPickup* wall = new WallPickup();
		wall->SetPosition(pos);
		wall->SetSprite(AnimationManager::GetInstance()->GetSprite("wall"));
		wall->SetCurrFrame(0);
		wall->SetTimeOfFrame(0);
		wall->SetCurrAnimation("wall");
		return wall;
	}
	else
	{
		WindowPickup* window = new WindowPickup();
		window->SetPosition(pos);
		window->SetSprite(AnimationManager::GetInstance()->GetSprite("window"));
		window->SetCurrFrame(0);
		window->SetTimeOfFrame(0);
		window->SetCurrAnimation("window");
		return window;
	}
}
