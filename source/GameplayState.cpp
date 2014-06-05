/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Justin Mazzola
***************************************************************/

#include "GameplayState.h"

#include "Game.h"
#include "OptionsState.h"
#include "MainMenuState.h"
#include "LoadSaveState.h"
#include "Button.h"
#include "Camera.h"

#include "Shop.h"
#include "Weapon.h"
#include "Inventory.h"

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
#include "CreateParticleMessage.h"
#include "CreateTowerMessage.h"
#include "CreateMachineGunBulletMessage.h"
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

#include "MachineGunTower.h"
#include "MapleSyrupTower.h"
#include "HockeyStickTower.h"
#include "LaserTower.h"

#include "MachineGunBullet.h"

#include "../TinyXML/tinyxml.h"

#include <Shlobj.h>

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <fstream>
using namespace std;


// Buckets
#define BUCKET_PLAYER 0
#define BUCKET_ENEMIES 1
#define BUCKET_PROJECTILES 5
#define BUCKET_PLACEABLE 3
#define BUCKET_PICKUP 4
#define BUCKET_TOWERS 2

// Winning Credits
#define SCROLL_SPEED 0.04f;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

char GameplayState::GetCurrentGameSlot() const
{
	return m_nCurrGameSlot;
}

void GameplayState::SetCurrentGameSlot(char slot)
{
	m_nCurrGameSlot = slot;
}

EntityManager* GameplayState::GetEntityManager() const
{
	return GetInstance()->m_pEntities;
}

ZombieFactory* GameplayState::GetZombieFactory() const
{
	return GetInstance()->zombieFactory;
}

/*************************************************************/
// CreatePlayer
//	- allocate a new player
//	- set the player's properties
Entity*	GameplayState::CreatePlayer() const
{
	Player* player = new Player();
	player->SetPosition(m_ptPlayerSpawnPoint);
	player->SetZombieFactory(zombieFactory);
	player->SetEntityManager(m_pEntities);
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

	// Load tower images
	m_hMachineGunBaseImage = pGraphics->LoadTexture("resource/images/towers/machineGunBase.png");
	m_hMachineGunGunImage = pGraphics->LoadTexture("resource/images/towers/machineGunGun.png");
	m_hMachineGunBulletImage = pGraphics->LoadTexture("resource/images/towers/machineGunBullet.png");
	m_hMapleSyrupBaseImage = pGraphics->LoadTexture("resource/images/towers/mapleSyrupBase.png");
	m_hHockeyStickBaseImage = pGraphics->LoadTexture("resource/images/towers/hockeyStickBase.png");
	m_hLaserBaseImage = pGraphics->LoadTexture("resource/images/towers/laserBase.png");

	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	m_hBackgroundMus = pAudio->LoadAudio(L"resource/audio/Background_Music.xwm");
	m_hShopMusic = pAudio->LoadAudio("resource/audio/shop_music.xwm");
	m_hGunShoot = pAudio->LoadAudio("resource/audio/Gun_Sound.wav");
	m_hRocketShoot = pAudio->LoadAudio("resource/audio/rocket_launch.wav");
	m_hShotgunShoot = pAudio->LoadAudio("resource/audio/shotgun_shot.wav");
	m_hBulletHit = pAudio->LoadAudio("resource/audio/Bullet_Hit.wav");
	//Load Particle Manager
	m_pParticleManager = ParticleManager::GetInstance();
	m_pParticleManager->loadEmitters("resource/particle/Blood_Particle1.xml");
	//m_pParticleManager->loadEmitters("resource/particle/smokeparticle.xml");
	//Set background color
	//SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black

	// Camera
	Camera::x = 0;
	Camera::y = 0;

	// Load all animation
	m_pAnimation = AnimationManager::GetInstance();
	m_pAnimation->LoadAll();

	// Load the world
	WorldManager* pWorld = WorldManager::GetInstance();
	pWorld->LoadWorld("resource/world/world.xml");

	// Start Zombie Factory
	zombieFactory = new ZombieFactory;
	zombieFactory->LoadWaves("resource/data/singleEnemy.xml");
	zombieFactory->Start();
	zombieFactory->SetSpawnWidth(pWorld->GetWorldWidth() * pWorld->GetTileWidth());
	zombieFactory->SetSpawnHeight(pWorld->GetWorldHeight() * pWorld->GetTileHeight());
	zombieFactory->SetEntityManager(m_pEntities);

	// Load the gamesave

	// If the slot is set
	if (m_nCurrGameSlot > 0)
	{
		// If we can't load the savegame
		if (!LoadSaveState::GetInstance()->CheckSlotExists(m_nCurrGameSlot - 1))
			// Make a new savegame
			SaveGame(true);
		else
			// load the savegame
			LoadGameFromSlot(m_nCurrGameSlot);
	}


	// Create our player
	m_pPlayer = CreatePlayer();
	zombieFactory->SetPlayer(dynamic_cast<Player*>(m_pPlayer));

	// If the slot is set

	// TODO: Make it so I DON'T have to do this twice, since I can't set money because
	// the player isn't created yet.
	if (m_nCurrGameSlot > 0)
	{
		// If we can't load the savegame
		if (!LoadSaveState::GetInstance()->CheckSlotExists(m_nCurrGameSlot - 1))
			// Make a new savegame
			SaveGame(true);
		else
			// load the savegame
			LoadGameFromSlot(m_nCurrGameSlot);
	}

	// Add it to the entity manager
	m_pEntities->AddEntity(m_pPlayer, BUCKET_PLAYER);

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

	// Setup Winning Credits
	// Set the margins for the text
	m_nTopMargin = 220;
	m_nBottomMargin = 474;

	// Set and start the credits movement
	m_ptTextPosition.x = 180;
	m_ptTextPosition.y = m_nBottomMargin + SCROLL_SPEED;

	// Setup You Win message transition timer
	m_fWinTimer = 5.0f;
	
	// Setup Losing Screen Variables
	m_bHasLost = false;
	m_bReplay = true;
	m_fLossTimer = 5.0f;

	// Play the background music
	pAudio->PlayAudio(m_hBackgroundMus, true);

	//OptionsState::GetInstance()->LoadOptions("resource/data/config.xml");

	// HUD
	m_hHUD = pGraphics->LoadTexture("resource/images/hud/hud.png");

	m_hShotgunPic = pGraphics->LoadTexture("resource/images/hud/shotgun.png");
	m_hShotgunThumb = pGraphics->LoadTexture("resource/images/hud/shotgunThumb.png");
	m_hARPic = pGraphics->LoadTexture("resource/images/hud/ar.png");
	m_hARThumb = pGraphics->LoadTexture("resource/images/hud/arthumb.png");
	m_hRLPic = pGraphics->LoadTexture("resource/images/hud/rpg.png");
	m_hRLThumb = pGraphics->LoadTexture("resource/images/hud/rpgthumb.png");
	m_hFireAxePic = pGraphics->LoadTexture("resource/images/hud/fireaxe.png");
	m_hFireAxeThumb = pGraphics->LoadTexture("resource/images/hud/fireaxethumb.png");
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/Blank.png");

	// Turn the cursor on
	if(pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();
}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)
{

	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Unload tower images
	pGraphics->UnloadTexture(m_hMachineGunBaseImage);
	pGraphics->UnloadTexture(m_hMachineGunGunImage);
	pGraphics->UnloadTexture(m_hMachineGunBulletImage);
	pGraphics->UnloadTexture(m_hMapleSyrupBaseImage);
	pGraphics->UnloadTexture(m_hHockeyStickBaseImage);
	pGraphics->UnloadTexture(m_hLaserBaseImage);

	m_pAnimation->UnloadSprites();
	m_pAnimation = nullptr;
	AnimationManager::DeleteInstance();

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	pAudio->UnloadAudio(m_hBackgroundMus);
	pAudio->StopAudio(m_hShopMusic);
	pAudio->UnloadAudio(m_hShopMusic);
	pAudio->UnloadAudio(m_hBulletHit);
	pAudio->UnloadAudio(m_hGunShoot);
	pAudio->UnloadAudio(m_hShotgunShoot);
	pAudio->UnloadAudio(m_hRocketShoot);

	//Matt gets rid of the memory leaks
	m_pParticleManager->unload();

	// Delete the zombie factory
	delete zombieFactory;

	// Save the file
	SaveGame(false);

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

	// Unload HUD
	pGraphics->UnloadTexture(m_hHUD);
	pGraphics->UnloadTexture(m_hShotgunPic);
	pGraphics->UnloadTexture(m_hShotgunThumb);
	pGraphics->UnloadTexture(m_hARPic);
	pGraphics->UnloadTexture(m_hARThumb);
	pGraphics->UnloadTexture(m_hRLPic);
	pGraphics->UnloadTexture(m_hRLThumb);
	pGraphics->UnloadTexture(m_hFireAxePic);
	pGraphics->UnloadTexture(m_hFireAxeThumb);
	pGraphics->UnloadTexture(m_hBackground);

	// Unload Blank
	pGraphics->UnloadTexture(m_hBackground);

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

	// Reset Winning Credits
	m_bCreditsStarted = false;

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

	// Manipulate the mouse here
	SGD::Point mousePt = {0.0f, 0.0f};
	mousePt = pInput->GetMousePosition();

	

	if (m_bCreditsStarted == false && m_fWinTimer == 5.0f && m_bHasLost == false)
		// Press Escape (PC) or Start (Xbox 360) to toggle pausing
	{
		if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::Start))
		{
			if (m_pShop->IsOpen() == false)
				m_bIsPaused = !m_bIsPaused;
		}
	// enter shop
	if (pInput->IsKeyPressed(SGD::Key::Backspace))
	{
		pAudio->StopAudio(m_hBackgroundMus);
		// to stop audio from playing after every backspace
		if (pAudio->IsAudioPlaying(m_hShopMusic) == false)
		{
			pAudio->PlayAudio(m_hShopMusic, true);
		}
		m_pShop->SetShopStatus(true);
	}

#pragma region Pause Menu Navigation Clutter
		// Handle pause menu input
		// If we're paused
		if (m_bIsPaused)
		{
			//-----------------------------------------------------------------------
			// --- Handling what tab we're in ---
			// If we're in the Main menu OF the pause menu. 
			if (m_nPauseMenuTab == PauseMenuTab::TAB_MAIN)
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

				// --- Scrolling through options ---
				// If the down arrow (PC), or down dpad (Xbox 360) are pressed
				// Move the cursor (selected item) down
				if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
				{
					// TODO: Add sound fx for going up and down
					++m_nPauseMenuCursor;

					// Wrap around the options
					if (m_nPauseMenuCursor > PauseMenuOptionsOption::OPTION_GOBACK)
						m_nPauseMenuCursor = PauseMenuOptionsOption::OPTION_MUSIC;
				}
				// If the up arrow (PC), or up dpad (Xbox 360) are pressed
				// Move the cursor (selected item) up
				else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
				{
					--m_nPauseMenuCursor;

					// Wrap around the options
					if (m_nPauseMenuCursor < PauseMenuOptionsOption::OPTION_MUSIC)
						m_nPauseMenuCursor = PauseMenuOptionsOption::OPTION_GOBACK;
				}


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
					case PauseMenuOptionsOption::OPTION_FULLSCREEN:
					{
																	  pGame->ToggleFullscreen();
					}
						break;
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
	}
#pragma endregion
	if (m_bCreditsStarted == true && (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A)))
	{
		// Since there's only one state..go back to main menu
		pGame->ChangeState(MainMenuState::GetInstance());
		return true;
	}
	if (m_bHasLost == true && m_fLossTimer <= 0.0f)
	{
		if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Up) || pInput->IsDPadPressed(0, SGD::DPad::Down))
			m_bReplay = !m_bReplay;

		else if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
		{
			switch (m_bReplay)
			{
			case true:
				Game::GetInstance()->ChangeState(GameplayState::GetInstance());
				return true;
				break;

			case false:
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
				return true;
				break;
			}
		}
	}

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
	// when shop closes play game background music
	if (m_pShop->IsOpen() == false && SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBackgroundMus) == false)
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hShopMusic);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hBackgroundMus);
	}
	// If the game isn't paused and you haven't won and you haven't lost
	if (m_bIsPaused == false && zombieFactory->GetWave() != zombieFactory->GetTotalWaves() + 1 && m_bHasLost == false)
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
		m_pEntities->CheckCollisions(BUCKET_ENEMIES, BUCKET_PROJECTILES);
		m_pEntities->CheckCollisions(BUCKET_ENEMIES, BUCKET_PLACEABLE);
		//draw grid rectangle
	}

	// If you have won the game
	else if (zombieFactory->GetWave() == zombieFactory->GetTotalWaves() + 1 && m_bHasLost == false)
	{
		m_bIsPaused = false;
		// Move the credits if they have started
		if (m_bCreditsStarted == true)
		{
			m_ptTextPosition.x = 220;
			m_ptTextPosition.y -= SCROLL_SPEED;

			if (m_fCreditsTimer < 0.0f)
			{
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
				return;
			}

			m_fCreditsTimer -= elapsedTime;
		}

		// If the screen has faded to black start the credits
		if (m_fWinTimer <= 0.0f && m_bCreditsStarted == false)
		{
			m_bCreditsStarted = true;
			m_fCreditsTimer = 28.0f;
		}

		// Count down to fade the screen to black and roll the credits
		if (m_fWinTimer > 0.0f)
			m_fWinTimer -= elapsedTime;
	}
	// If you have lost fade to the replay menu
	else if (m_bHasLost == true)
	{
		if (m_fLossTimer > 0)
			m_fLossTimer -= elapsedTime;
	}

	// Update FPS
	// Increase the frame count
	m_unFrames++;

	// Decrease the timer
	m_fFPSTimer -= elapsedTime;

	// Has the timer expired?
	if (m_fFPSTimer <= 0.0f)
	{
		m_unFPS = m_unFrames;
		m_unFrames = 0;
		m_fFPSTimer = 1.0f;		// count down from 1 second
	}
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

#if _DEBUG
	pGraphics->DrawString("Gameplay State | Debugging", { 240, 0 }, { 255, 0, 255 });
#endif
	// If the credits aren't rolling and you haven't loss
	if (m_bCreditsStarted == false && m_fLossTimer > 0.0f)
	{
		// Render test world
		WorldManager::GetInstance()->Render(SGD::Point((float)Camera::x, (float)Camera::y));
		Player* player = dynamic_cast<Player*>(m_pPlayer);

		//Render test particles
		m_pParticleManager->Render();

		// Render the entities
		m_pEntities->RenderAll();

		// Draw health overlay
		float currHealth = player->GetCurrHealth();
		float maxHealth = player->GetMaxHealth();
		if (currHealth != maxHealth)
		{
			float ratio = currHealth / maxHealth;
			unsigned char alpha = 255 - (unsigned int)(255.0f * ratio);

			pGraphics->DrawRectangle(SGD::Rectangle(0.0f, 0.0f, 800.0f, 600.0f), SGD::Color(alpha, 255, 0, 0));
		}

		// FOR DEBUG PURPOSES ONLY!
		//pGraphics->DrawString(std::to_string(currHealth).c_str(), { 0, 32 });

		// --- Pause Menu stuff ---
		// If we're paused
		if (m_bIsPaused)
		{
			if(pGraphics->IsCursorShowing() == false)
				pGraphics->TurnCursorOn();
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

				// If the game is in fullscreen
				if (Game::GetInstance()->GetFullscreen())
				{
					if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_FULLSCREEN)
						m_pMainButton->Draw("Fullscreen: No", { 160, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
					else
						m_pMainButton->Draw("Fullscreen: No", { 160, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
				}
				// If the game is windowed
				else
				{
					if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_FULLSCREEN)
						m_pMainButton->Draw("Fullscreen: Yes", { 160, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
					else
						m_pMainButton->Draw("Fullscreen: Yes", { 160, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
				}

				if (m_nPauseMenuCursor == PauseMenuOptionsOption::OPTION_GOBACK)
					m_pMainButton->Draw("Go Back", { 150, 470 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
				else
					m_pMainButton->Draw("Go Back", { 150, 470 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
			}

		}

		// If we're shopping
		if (m_pShop->IsOpen())
		{
			m_pShop->Render();
		}

		// Render the FPS
		string fps = "FPS: ";
		fps += std::to_string(m_unFPS);
		pGraphics->DrawString(fps.c_str(), { 0, 580 }, { 255, 0, 0 });

		// -- Render HUD --
		if (!m_bIsPaused)
		{
			if (!m_pShop->IsOpen())
			{
				pGraphics->DrawTexture(m_hHUD, { 0, 0 });

				// -- Draw the score --
				string score = "Score: ";
				score += std::to_string(player->GetScore());
				m_pFont->Draw(score.c_str(), 50, 70, 0.8f, { 255, 255, 255 });

				// -- Draw the wave number --
				string waveNum = "Wave: ";
				waveNum += std::to_string(zombieFactory->GetWave());
				m_pFont->Draw(waveNum.c_str(), 350, 60, 0.6f, { 255, 255, 255 });

				// -- Draw the time remaining [during build mode] --
				if (zombieFactory->IsBuildMode())
				{
					// Turn the cursor off for build mode
					if(pGraphics->IsCursorShowing() == true)
						pGraphics->TurnCursorOff();

					string timeRemaining = "Time remaining: ";
					timeRemaining += (std::to_string(zombieFactory->GetBuildTimeRemaining() / 100.0f));
					timeRemaining += " secs";
					m_pFont->Draw(timeRemaining.c_str(), 180, 30, 0.6f, { 255, 255, 255 });

					m_pFont->Draw("Time to Build!", 340, 110, 0.4f, { 255, 255, 0 });
				}
				// -- Draw the number of enemies remaining [during fight mode] --
				else
				{
					// Turn the cursor on when not in build mode
					if(pGraphics->IsCursorShowing() == false)
						pGraphics->TurnCursorOn();

					string enemiesRemaining = "Enemies Remaining: ";
					m_pFont->Draw(enemiesRemaining.c_str(), 225, 30, 0.6f, { 255, 255, 255 });

					int numOfEnemies = zombieFactory->GetEnemiesRemaining();
					if (numOfEnemies <= 3)
						m_pFont->Draw(std::to_string(numOfEnemies).c_str(), 495, 30, 0.6f, { 255, 0, 0 });
					else
						m_pFont->Draw(std::to_string(numOfEnemies).c_str(), 495, 30, 0.6f, { 255, 255, 255 });

				}

				// -- Draw the items --

				// Get the inventory
				Inventory* inv = player->GetInventory();

				// Draw the number of healthpacks
				m_pFont->Draw(std::to_string(inv->GetHealthPacks()).c_str(), 83, 392, 0.4f, { 255, 255, 255 });

				// Draw the number of grenades
				m_pFont->Draw(std::to_string(inv->GetGrenades()).c_str(), 83, 462, 0.4f, { 255, 255, 255 });

				// Draw the number of walls
				m_pFont->Draw(std::to_string(inv->GetWalls()).c_str(), 75, 532, 0.4f, { 255, 255, 255 });

				// Draw the number of windows
				m_pFont->Draw(std::to_string(inv->GetWindows()).c_str(), 140, 532, 0.4f, { 255, 255, 255 });

				// Draw the number of beartraps
				m_pFont->Draw(std::to_string(inv->GetBearTraps()).c_str(), 220, 532, 0.4f, { 255, 255, 255 });

				// Draw the number of mines
				m_pFont->Draw(std::to_string(inv->GetMines()).c_str(), 298, 532, 0.4f, { 255, 255, 255 });

				// -- Draw the selected weapon -- 

				// Get the weapons
				Weapon* weapons = player->GetWeapons();
				string names[4] = { "Assault Rifle", "Shotgun", "Rocket Launcher", "Fire Axe" };
				SGD::HTexture textures[4] = { m_hARPic, m_hShotgunPic, m_hRLPic, m_hFireAxePic };

				// Draw the name of the selected weapon
				m_pFont->Draw(names[player->GetCurrWeapon()], 515, 435, 0.4f, { 255, 255, 255 });

				// Draw the picture of the selected pic
				pGraphics->DrawTextureSection(textures[player->GetCurrWeapon()], { 506, 466 }, { 0, 0, 160, 80 });

				// Draw the ammo of the selected weapon
				m_pFont->Draw(std::to_string(weapons[player->GetCurrWeapon()].GetCurrAmmo()).c_str(), 700, 494, 0.6f, { 255, 255, 255 });

				// -- Draw the offhand weapons ammos --
				m_pFont->Draw(std::to_string(weapons[0].GetCurrAmmo()).c_str(), 510, 375, 0.5f, { 255, 255, 255 });
				m_pFont->Draw(std::to_string(weapons[1].GetCurrAmmo()).c_str(), 580, 375, 0.5f, { 255, 255, 255 });
				m_pFont->Draw(std::to_string(weapons[2].GetCurrAmmo()).c_str(), 660, 375, 0.5f, { 255, 255, 255 });
				//Draw the grid rectange
				SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
				//NOTE: why did it take this much work? did i do something wrong?
				/*pos.x = (pos.x + player->GetPosition().x - ((int)pos.x + (int)player->GetPosition().x) % 32) - Camera::x - 384;
				pos.y = (pos.y + player->GetPosition().y - ((int)pos.y + (int)player->GetPosition().y) % 32) - Camera::y - 288;
				pGraphics->DrawRectangle({ pos.x, pos.y, pos.x + 32, pos.y + 32 }, { 0, 0, 0, 0 }, { 255, 0, 0, 0 }, 2);*/
			}
		}

		// If you have won the game render You Win and fade to credits
		if (zombieFactory->GetWave() == zombieFactory->GetTotalWaves() + 1 && m_bHasLost == false)
		{
			Game * pGame = Game::GetInstance();

			pGraphics->DrawRectangle(
				SGD::Rectangle(SGD::Point(0.0f, 0.0f), SGD::Point((float)pGame->GetScreenWidth(), (float)pGame->GetScreenHeight())),
				SGD::Color(255 - (char)(m_fWinTimer * 51), 0, 0, 0));

			m_pFont->Draw("You Win!", (pGame->GetScreenWidth() / 2) - (m_pFont->GetTextWidth("You Win!")), pGame->GetScreenHeight() / 2 - 64, 2.0f, SGD::Color{ 255, 0, 0 });
		}
		if (m_bHasLost == true)
		{
			Game * pGame = Game::GetInstance();

			pGraphics->DrawRectangle(
				SGD::Rectangle(SGD::Point(0.0f, 0.0f), SGD::Point((float)pGame->GetScreenWidth(), (float)pGame->GetScreenHeight())),
				SGD::Color(255 - (char)(m_fLossTimer * 51), 0, 0, 0));

			m_pFont->Draw("You Lose!", (pGame->GetScreenWidth() / 2) - (m_pFont->GetTextWidth("You Lose!")), pGame->GetScreenHeight() / 2 - 64, 2.0f, SGD::Color{ 0, 0, 0 });
		}
	}
	// Render the credits if you have won and faded to them
	else if (m_bCreditsStarted == true)
	{
		RenderCredits();
	}
	// Render the Replay menu if you have lost and faded to them
	else if (m_bHasLost  && m_fLossTimer <= 0.0f)
	{
		RenderLoss();
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
											 if (pCreateMessage->GetWeaponNumber() == 1)
											 {
												 for (int i = 0; i < 9; i++)
												 {
													 Entity*bullet = self->CreateProjectile(pCreateMessage->GetWeaponNumber());
													 self->m_pEntities->AddEntity(bullet, BUCKET_PROJECTILES);
													 bullet->Release();
													 bullet = nullptr;
												 }
											 }
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
											   g->m_ptPlayerSpawnPoint.x = (float)pCreateMessage->GetX();
											   g->m_ptPlayerSpawnPoint.y = (float)pCreateMessage->GetY();

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
	case MessageID::MSG_CREATE_STATIC_PARTICLE:
	{
												  const CreateParticleMessage* pCreateMessage = dynamic_cast<const CreateParticleMessage*>(pMsg);
												  GameplayState* g = GameplayState::GetInstance();
												  ParticleManager::GetInstance()->activate(pCreateMessage->GetEmitterID(), pCreateMessage->GetX(), pCreateMessage->GetY());
	}
		break;
	case MessageID::MSG_CREATE_DYNAMIC_PARTICLE:
	{
												   const CreateParticleMessage* pCreateMessage = dynamic_cast<const CreateParticleMessage*>(pMsg);
												   GameplayState* g = GameplayState::GetInstance();
												   ParticleManager::GetInstance()->activate(pCreateMessage->GetEmitterID(), pCreateMessage->GetParticleEntity(), pCreateMessage->GetXOffset(), pCreateMessage->GetYOffset());
	}
		break;
	case MessageID::MSG_CREATE_TOWER:
	{
										const CreateTowerMessage* pCreateMessage = dynamic_cast<const CreateTowerMessage*>(pMsg);
										GameplayState* g = GameplayState::GetInstance();
										Entity* tower = g->CreateTower(pCreateMessage->x, pCreateMessage->y, pCreateMessage->towerType);
										g->m_pEntities->AddEntity(tower, BUCKET_TOWERS);
										tower->Release();
	}
		break;
	case MessageID::MSG_CREATE_MACHINE_GUN_BULLET:
	{
													 const CreateMachineGunBulletMessage* pCreateMessage = dynamic_cast<const CreateMachineGunBulletMessage*>(pMsg);
													 GameplayState* g = GameplayState::GetInstance();
													 Entity* bullet = g->CreateMachineGunBullet(pCreateMessage->x, pCreateMessage->y, pCreateMessage->velocity, pCreateMessage->damage);
													 g->m_pEntities->AddEntity(bullet, BUCKET_PROJECTILES);
													 bullet->Release();
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

Entity* GameplayState::CreateBeaverZombie(int _x, int _y) const
{
	BeaverZombie* tempBeav = new BeaverZombie;
	tempBeav->SetDamage(10);
	tempBeav->SetPosition({ (float)_x, (float)_y });
	tempBeav->SetAttackRange(1.0f);
	tempBeav->SetMaxHealth(100);
	tempBeav->SetCurrHealth(100);
	tempBeav->SetSpeed(200.0f);
	tempBeav->SetVelocity({ 0, 0 });

	// AIComponent
	tempBeav->SetPlayer(m_pPlayer);

	return tempBeav;
}

Entity* GameplayState::CreateFastZombie(int _x, int _y) const
{
	FastZombie* zambie = new FastZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(100);
	zambie->SetCurrHealth(100);
	zambie->SetSpeed(150.0f);
	zambie->SetVelocity({ 0, 0 });

	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	return zambie;
}

Entity* GameplayState::CreateSlowZombie(int _x, int _y) const
{
	SlowZombie* zambie = new SlowZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(100);
	zambie->SetCurrHealth(100);
	zambie->SetSpeed(100.0f);
	zambie->SetVelocity({ 0, 0 });

	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	return zambie;
}

Entity* GameplayState::CreatePlaceable(int trap) const
{
	if (trap == 2)
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
		trap->SetSprite(AnimationManager::GetInstance()->GetSprite("mine"));
		trap->SetCurrFrame(0);
		trap->SetTimeOfFrame(0);
		trap->SetCurrAnimation("mine");
		return trap;
	}
}

Entity* GameplayState::CreateProjectile(int _Weapon) const
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
			   SGD::Vector vec = pos - m_pPlayer->GetPosition();
			   vec.Normalize();
			   vec *= 1000;
			   tempProj->SetVelocity(vec);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hGunShoot);
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
			   SGD::Vector vec = pos - m_pPlayer->GetPosition();
			   vec.Normalize();
			   vec *= (float)(750 + rand() % 500);

			   // Rotate bullet at random direction
			   float degree = (-50 + rand() % 100) / 100.0f;
			   vec.Rotate(degree);
			   SGD::AudioManager::GetInstance()->PlayAudio(m_hShotgunShoot);

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
			   pos.x += Camera::x;
			   pos.y += Camera::y;
			   SGD::Vector vec = pos - m_pPlayer->GetPosition();
			   vec.Normalize();
			   vec *= 1000;
			   tempProj->SetVelocity(vec);

			   //ParticleManager::GetInstance()->activate("Smoke_Particle", tempProj, 0, 0);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hRocketShoot);

			   return tempProj;
	}
		break;
	case 3://Fire axe?
	{
			   break;
	}
	}

	return nullptr;
}

Entity* GameplayState::CreatePickUp(int pick, SGD::Point pos) const
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

Entity* GameplayState::CreateTower(int _x, int _y, int _type) const
{
	switch (_type)
	{
	case CreateTowerMessage::TOWER_MACHINE_GUN:
	{
												  MachineGunTower* tower = new MachineGunTower;

												  tower->SetPosition(SGD::Point((float)_x, (float)_y));
												  tower->SetBaseImage(m_hMachineGunBaseImage);
												  tower->SetGunImage(m_hMachineGunGunImage);

												  return tower;

	}
		break;
	case CreateTowerMessage::TOWER_MAPLE_SYRUP:
	{
												  MapleSyrupTower* tower = new MapleSyrupTower;

												  tower->SetPosition(SGD::Point((float)_x, (float)_y));
												  tower->SetBaseImage(m_hMapleSyrupBaseImage);

												  return tower;
	}
		break;
	case CreateTowerMessage::TOWER_HOCKEY_STICK:
	{
												   HockeyStickTower* tower = new HockeyStickTower;

												   tower->SetPosition(SGD::Point((float)_x, (float)_y));
												   tower->SetBaseImage(m_hHockeyStickBaseImage);

												   return tower;
	}
		break;
	case CreateTowerMessage::TOWER_LASER:
	{
											LaserTower* tower = new LaserTower;

											tower->SetPosition(SGD::Point((float)_x, (float)_y));
											tower->SetBaseImage(m_hLaserBaseImage);

											return tower;
	}
		break;
	}

	return nullptr;
}

Entity* GameplayState::CreateMachineGunBullet(int _x, int _y, SGD::Vector _velocity, int _damage) const
{
	MachineGunBullet* bullet = new MachineGunBullet;

	bullet->SetPosition(SGD::Point((float)_x, (float)_y));
	bullet->SetDamage(_damage);
	bullet->SetImage(m_hMachineGunBulletImage);
	bullet->SetVelocity(_velocity);

	return bullet;
}

// LoadGameFromSlot
// - Load game from the slot
void GameplayState::LoadGameFromSlot(int slot)
{
	HRESULT hr;
	ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\RazorBalloon\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);

	// Create our save file
	pathtowrite += "\\SoorrySaveGame_0";
	pathtowrite += std::to_string(slot) + ".xml";


	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load the file, if not gtfo
	if (!doc.LoadFile(pathtowrite.c_str()))
		return;

	// Access the root element (volume)
	TiXmlElement* pRoot = doc.RootElement();

	// Is the root there, if not, gtfo
	if (pRoot == nullptr)
		return;

	m_ptPlayerSpawnPoint.x = float(atoi(pRoot->Attribute("x")));
	m_ptPlayerSpawnPoint.y = float(atoi(pRoot->Attribute("y")));

	// Get the volume
	TiXmlElement* pStats = pRoot->NextSiblingElement("stats");

	// Set the player's score
	if (dynamic_cast<Player*>(m_pPlayer) != nullptr)
	{
		int money = int(atoi(pStats->Attribute("money")));
		dynamic_cast<Player*>(m_pPlayer)->SetScore(money);
	}


}

// SaveGame
// - Saves and or creates a savefile in the appdata
// [in] newFile - if it's creating a file: true, otherwise false
void GameplayState::SaveGame(bool newFile)
{

	// --- Make a new XML file in Appdata ---
	HRESULT hr;
	ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\RazorBalloon\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);

	// Create our save file
	pathtowrite += "\\SoorrySaveGame_0";
	pathtowrite += std::to_string(m_nCurrGameSlot) + ".xml";


	// If we're making a new file
	if (newFile)
	{

		// Make a document
		TiXmlDocument doc;

		// Allocate a Tiny XML Declaration
		TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "utf-8", "");

		// Attach the declaration to the document
		doc.LinkEndChild(pDecl);

		// Add a new element 'position'
		TiXmlElement* pRoot = new TiXmlElement("position");

		// Add the X and Y of position
		pRoot->SetAttribute("x", 0);
		pRoot->SetAttribute("y", 0);

		// Link the root to the doc
		doc.LinkEndChild(pRoot);

		// Add a new element called 'stats'
		TiXmlElement* pStats = new TiXmlElement("stats");

		// Add the money
		pStats->SetAttribute("money", 0);

		// Link the stats to the doc
		doc.LinkEndChild(pStats);

		// Save the file
		doc.SaveFile(pathtowrite.c_str());
	}
	else
	{
		// Make a document
		TiXmlDocument doc;

		// Allocate a Tiny XML Declaration
		TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "utf-8", "");

		// Attach the declaration to the document
		doc.LinkEndChild(pDecl);

		// Add a new element 'position'
		TiXmlElement* pRoot = new TiXmlElement("position");

		// Add the X and Y of position
		pRoot->SetAttribute("x", (int)m_pPlayer->GetPosition().x);
		pRoot->SetAttribute("y", (int)m_pPlayer->GetPosition().y);

		// Link the root to the doc
		doc.LinkEndChild(pRoot);

		// Add a new element called 'stats'
		TiXmlElement* pStats = new TiXmlElement("stats");

		// Add the money
		pStats->SetAttribute("money", (int)dynamic_cast<Player*>(m_pPlayer)->GetScore());

		// Link the stats to the doc
		doc.LinkEndChild(pStats);

		// Save the file
		doc.SaveFile(pathtowrite.c_str());
	}
}

// Render Credits
// - Renders the credits upon winning the game
void GameplayState::RenderCredits(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	if(pGraphics->IsCursorShowing() == true)
		pGraphics->TurnCursorOn();

	// Draw the background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 });

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
					  Justin Mazzola\n\n\
					  Special Thanks\n\
					  Jordan Butler for ideas.";
	m_pFont->Draw(credits, (int)m_ptTextPosition.x, (int)m_ptTextPosition.y, 0.5f, { 255, 0, 0 });

	// Warning: SUPER JIT. THIS IS REALLY GHETTO.
	// Draw rectangles to cut off the words to give an illusion of margins
	pGraphics->DrawTextureSection(m_hBackground, { 0, 0 },
		SGD::Rectangle(0, 0, 800, 228), {}, {});

	pGraphics->DrawTextureSection(m_hBackground, { 0, 475 },
		SGD::Rectangle(0, 475, 800, 600), {}, {});

	// Render button
	m_pMainButton->Draw("Main Menu", { 180, 500 }, { 255, 0, 0 }, { 1, 1 }, 0);

	m_pFont->Draw("Credits", Game::GetInstance()->GetScreenWidth() / 2 - (int)((m_pFont->GetTextWidth("Credits") / 2) * 1.2f) - 20, 100, 1.2f, SGD::Color(255, 0, 0, 0));

	if (m_fCreditsTimer <= 5.0f)
	{
		Game * pGame = Game::GetInstance();
		pGraphics->DrawRectangle(SGD::Rectangle(SGD::Point(0.0f, 0.0f), SGD::Point((float)pGame->GetScreenWidth(), (float)pGame->GetScreenHeight())), SGD::Color(255 - (char)(m_fCreditsTimer * 51), 0, 0, 0));
	}
}

// HasLost
// - Lets gameplay know that the player has died
// sets the game pause to false
void GameplayState::HasLost(void)
{
	m_bHasLost = true;
	m_bIsPaused = false;
}

// RenderLoss
// - Renders the game over screen
// Allows the player to replay the current game mode or
// go to the main menu
void GameplayState::RenderLoss(void)
{
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	// Turn on the cursor for menu purposes
	if(pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();

	// Draw the paused main menu background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 });

	// Draw the game over at the top
	m_pFont->Draw("Game Over", Game::GetInstance()->GetScreenWidth() / 2 - (int)(m_pFont->GetTextWidth("Game Over") * .75f), 100, 1.2f, SGD::Color(255, 0, 0, 0));

	// Draw the options
	if (m_bReplay == true)
		m_pMainButton->Draw("Soorry, Try Again?", { 220, 200 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
	else
		m_pMainButton->Draw("Soorry, Try Again?", { 220, 200 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

	if (m_bReplay == false)
		m_pMainButton->Draw("Main Menu, eh?", { 200, 290 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
	else
		m_pMainButton->Draw("Main Menu, eh?", { 200, 290 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);


}