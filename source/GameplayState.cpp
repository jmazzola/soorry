/***************************************************************
|	File:		GameplayState.h
|	Author:		Justin Mazzola & Justin Patterson & Matthew Salow &
|				Ryan Simmons & James Sylvester 
|	Course:		SGP
|	Purpose:	This state is the game. Like the whole game.
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
#include "EntityManager.h"

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
#include "CreateDroneMessage.h"
#include "CreateTrapMessage.h"
#include "WaveCompleteMessage.h"
#include "CreateMapleSyrupBulletMessage.h"
#include "CreateGrenadeMessage.h"
#include "CreateShopMessage.h"
#include "CreateExplosionMessage.h"

//Object Includes
#include "BeaverZombie.h"
#include "FastZombie.h"
#include "SlowZombie.h"
#include "ShotgunPellet.h"
#include "Rocket.h"
#include "AssaultRifleBullet.h"
#include "Drone.h"
#include "Grenade.h"
#include "TrickShotBullet.h"
#include "ShopEntity.h"

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
#include "AmmoPickup.h"
#include "HealthPackPickup.h"
#include "SuperPack.h"

#include "TowerFlyweight.h"
#include "MachineGunTower.h"
#include "MapleSyrupTower.h"
#include "HockeyStickTower.h"
#include "LaserTower.h"
#include "SpikeTrap.h"
#include "LavaTrap.h"

#include "AIComponent.h"

#include "MachineGunBullet.h"
#include "MapleSyrupBullet.h"

#include "StatTracker.h"

#include "Explosion.h"

#include "../TinyXML/tinyxml.h"

#include <Shlobj.h>

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <fstream>
using namespace std;

// Buckets
#define BUCKET_TRAPS 0
#define BUCKET_PLAYER 1
#define BUCKET_ENEMIES 2
#define BUCKET_TOWERS 3
#define BUCKET_PLACEABLE 4
#define BUCKET_PICKUP 5
#define BUCKET_PROJECTILES 6
#define BUCKET_DRONE 7
#define BUCKET_GRENADES 8
#define BUCKET_SHOP 9
#define BUCKET_EXPLOSIONS 10

// Winning Credits
#define SCROLL_SPEED 0.04f;

// Selected Box
#define DRAWSELECTED(TOP,LEFT,SIZEW,SIZEH) SGD::Rectangle({ TOP, LEFT }, SGD::Size(SIZEW, SIZEH)), {0,0,0,0}, { 255, 255, 255, 0 }, 2

// Colors
// Todo make Colors.h 

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

int GameplayState::GetGameMode() const
{
	return m_nGamemode;
}

void GameplayState::SetGameMode(int _gameMode)
{
	m_nGamemode = _gameMode;
}

/*************************************************************/
// CreatePlayer
//	- allocate a new player
//	- set the player's properties
Entity*	GameplayState::CreatePlayer(string _playerStatsFileName) const
{
	Player* player = new Player();

	player->SetPosition(m_ptPlayerSpawnPoint);
	player->SetZombieFactory(zombieFactory);
	player->SetEntityManager(m_pEntities);
	player->SetPlaceablesImage(m_hPlaceablesImage);
	player->SetRangeCirclesImage(m_hRangeCirclesImage);
	player->SetSuperLength(4.0f);

	// Load player stats
	TiXmlDocument doc;

	// Attempt to load from the file
	doc.LoadFile(_playerStatsFileName.c_str());

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Temp variables to store data
	int health;
	int speed;
	int walls;
	int windows;
	int bearTraps;
	int mines;
	int machineGuns;
	int mapleSyrups;
	int hockeySticks;
	int lasers;
	int lavaTraps;
	int spikeTraps;

	// Load data
	pRoot->FirstChildElement("health")->Attribute("value", &health);
	pRoot->FirstChildElement("speed")->Attribute("value", &speed);
	pRoot->FirstChildElement("walls")->Attribute("amount", &walls);
	pRoot->FirstChildElement("windows")->Attribute("amount", &windows);
	pRoot->FirstChildElement("bear_traps")->Attribute("amount", &bearTraps);
	pRoot->FirstChildElement("mines")->Attribute("amount", &mines);
	pRoot->FirstChildElement("machine_guns")->Attribute("amount", &machineGuns);
	pRoot->FirstChildElement("maple_syrups")->Attribute("amount", &mapleSyrups);
	pRoot->FirstChildElement("hockey_sticks")->Attribute("amount", &hockeySticks);
	pRoot->FirstChildElement("lasers")->Attribute("amount", &lasers);
	pRoot->FirstChildElement("lava_traps")->Attribute("amount", &lavaTraps);
	pRoot->FirstChildElement("spike_traps")->Attribute("amount", &spikeTraps);

	// Assign data
	player->SetMaxHealth((float)health);
	player->SetCurrHealth(player->GetMaxHealth());
	player->SetSpeed((float)speed);
	
	Inventory* inventory = player->GetInventory();
	inventory->SetWalls(walls);
	inventory->SetWindows(windows);
	inventory->SetBearTraps(bearTraps);
	inventory->SetMines(mines);
	inventory->SetMachineGunTowers(machineGuns);
	inventory->SetMapleSyrupTowers(mapleSyrups);
	inventory->SetHockeyStickTowers(hockeySticks);
	inventory->SetLaserTowers(lasers);
	inventory->SetLavaTraps(lavaTraps);
	inventory->SetSpikeTraps(spikeTraps);

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
	m_pStatTracker = StatTracker::GetInstance();
	m_pStatTracker->Load("resource/data/stats.xml");
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Initialize the Event Manager
	m_pEvents = SGD::EventManager::GetInstance();
	m_pEvents->Initialize();

	// Initialize the Message Manager
	m_pMessages = SGD::MessageManager::GetInstance();
	m_pMessages->Initialize(&MessageProc);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	pGraphics->SetClearColor();
	pGraphics->DrawString("Loading Textures", SGD::Point(280, 300));
	pGraphics->Update();

	// Load Tim
	m_hPlayerImg = pGraphics->LoadTexture(L"resource/images/tim/tim.png");

	// Load tower images
	m_hMachineGunBaseImage = pGraphics->LoadTexture("resource/images/towers/machineGunBase.png");
	m_hMachineGunGunImage = pGraphics->LoadTexture("resource/images/towers/machineGunGun.png");
	m_hMachineGunBulletImage = pGraphics->LoadTexture("resource/images/towers/machineGunBullet.png");
	m_hMapleSyrupBaseImage = pGraphics->LoadTexture("resource/images/towers/mapleSyrupBase.png");
	m_hMapleSyrupGunImage = pGraphics->LoadTexture("resource/images/towers/mapleSyrupGun.png");
	m_hMapleSyrupBulletImage = pGraphics->LoadTexture("resource/images/towers/mapleSyrupBullet.png");
	m_hHockeyStickBaseImage = pGraphics->LoadTexture("resource/images/towers/hockeyStickBase.png");
	m_hHockeyStickGunImage = pGraphics->LoadTexture("resource/images/towers/hockeyStickGun.png");
	m_hLaserBaseImage = pGraphics->LoadTexture("resource/images/towers/laserBase.png");
	m_hPlaceablesImage = pGraphics->LoadTexture("resource/images/towers/placeables.png");
	m_hRangeCirclesImage = pGraphics->LoadTexture("resource/images/towers/rangeCircles.png");
	m_hSpikeTrapBaseImage = pGraphics->LoadTexture("resource/images/towers/spikeTrapDown.png");
	m_hSpikeTrapSpikeImage = pGraphics->LoadTexture("resource/images/towers/spikeTrapUp.png");
	m_hLavaTrapBaseImage = pGraphics->LoadTexture("resource/images/towers/lavaTrapBase.png");
	m_hLavaTrapFlameImage = pGraphics->LoadTexture("resource/images/towers/lavaTrapFlame.png");

	m_hExplosionImage = pGraphics->LoadTexture("resource/animation/explosprite.png");

	pGraphics->SetClearColor();
	pGraphics->DrawString("Loading Audio", SGD::Point(280, 300));
	pGraphics->Update();

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
	m_pParticleManager->loadEmitter("resource/particle/Blood_Spurt.xml");
	m_pParticleManager->loadEmitter("resource/particle/Fire_Particle1.xml");
	//Set background color
	//SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black

	// Camera
	Camera::x = 0;
	Camera::y = 0;

	pGraphics->SetClearColor();
	pGraphics->DrawString("Loading Animations", SGD::Point(280, 300));
	pGraphics->Update();

	// Load all animation
	m_pAnimation = AnimationManager::GetInstance();
	m_pAnimation->LoadAll();

#pragma region Load Game Mode

	// Load game mode information
	string gameModeFileName;
	switch (m_nGamemode)
	{
	case 0:
		gameModeFileName = "resource/data/game_modes/arcade_mode/arcadeMode.xml";
		break;
	case 1:
		gameModeFileName = "resource/data/game_modes/hardcore_mode/hardcore_Mode.xml";
		break;
	case 2:
		gameModeFileName = "resource/data/game_modes/sandbox_mode/sandboxMode.xml";
		break;
	case 3:
		gameModeFileName = "resource/data/game_modes/beaver_feaver_mode/beaver_fever_mode.xml";
		break;
	case 4:
		// To be replaced with Ryan's file
		gameModeFileName = "resource/data/game_modes/arcade_mode/arcadeMode.xml";
	}

	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	doc.LoadFile(gameModeFileName.c_str());

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	string worldFileName = pRoot->FirstChildElement("world")->GetText();
	string waveFileName = pRoot->FirstChildElement("wave_data")->GetText();
	string playerStatsFileName = pRoot->FirstChildElement("player_stats")->GetText();
	string enemyStatsFileName = pRoot->FirstChildElement("enemy_stats")->GetText();
	string towerStatsFileName = pRoot->FirstChildElement("tower_stats")->GetText();
	string shopFileName = pRoot->FirstChildElement("shop")->GetText();

#pragma endregion

	pGraphics->SetClearColor();
	pGraphics->DrawString("Loading World", SGD::Point(280, 300));
	pGraphics->Update();

	// Load the world
	WorldManager* pWorld = WorldManager::GetInstance();
	pWorld->LoadWorld(worldFileName);

	pGraphics->SetClearColor();
	pGraphics->DrawString("Initializing", SGD::Point(280, 300));
	pGraphics->Update();

	// Start Zombie Factory
	zombieFactory = new ZombieFactory;
	//zombieFactory->LoadWaves("resource/data/singleEnemy.xml");
	zombieFactory->LoadWaves(waveFileName);
	//zombieFactory->LoadWaves("resource/data/longbuildtime.xml");
	zombieFactory->Start();
	zombieFactory->SetSpawnWidth(pWorld->GetWorldWidth() * pWorld->GetTileWidth());
	zombieFactory->SetSpawnHeight(pWorld->GetWorldHeight() * pWorld->GetTileHeight());
	zombieFactory->SetEntityManager(m_pEntities);

	// Load enemy stats recipes
	LoadEnemyRecipes(enemyStatsFileName);

	// Load tower flyweight
	m_pTowerFlyweight = new TowerFlyweight;
	m_pTowerFlyweight->Load(towerStatsFileName);
	m_pTowerFlyweight->SetRangeCirclesImage(m_hRangeCirclesImage);

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
	m_pPlayer = CreatePlayer(playerStatsFileName);
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
	m_pShop->LoadPrices(shopFileName);

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
	m_hBuildModeHUD = pGraphics->LoadTexture("resource/images/hud/HUD_BuildMode.png");

	m_hShotgunPic = pGraphics->LoadTexture("resource/images/hud/shotgun.png");
	m_hShotgunThumb = pGraphics->LoadTexture("resource/images/hud/shotgunThumb.png");
	m_hARPic = pGraphics->LoadTexture("resource/images/hud/ar.png");
	m_hARThumb = pGraphics->LoadTexture("resource/images/hud/arthumb.png");
	m_hRLPic = pGraphics->LoadTexture("resource/images/hud/rpg.png");
	m_hRLThumb = pGraphics->LoadTexture("resource/images/hud/rpgthumb.png");
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/Blank.png");

	// Turn the cursor on
	if(pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();

#if ARCADE_MODE
	m_vtStick = {0.0f, 0.0f};
	m_bAccept = true;
#endif

}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void GameplayState::Exit(void)
{
	m_pStatTracker->Save("resource/data/stats.xml");
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Unload tower images
	pGraphics->UnloadTexture(m_hMachineGunBaseImage);
	pGraphics->UnloadTexture(m_hMachineGunGunImage);
	pGraphics->UnloadTexture(m_hMachineGunBulletImage);
	pGraphics->UnloadTexture(m_hMapleSyrupBaseImage);
	pGraphics->UnloadTexture(m_hMapleSyrupGunImage);
	pGraphics->UnloadTexture(m_hMapleSyrupBulletImage);
	pGraphics->UnloadTexture(m_hHockeyStickBaseImage);
	pGraphics->UnloadTexture(m_hHockeyStickGunImage);
	pGraphics->UnloadTexture(m_hLaserBaseImage);
	pGraphics->UnloadTexture(m_hPlaceablesImage);
	pGraphics->UnloadTexture(m_hPlaceablesImage);
	pGraphics->UnloadTexture(m_hRangeCirclesImage);
	pGraphics->UnloadTexture(m_hSpikeTrapBaseImage);
	pGraphics->UnloadTexture(m_hSpikeTrapSpikeImage);
	pGraphics->UnloadTexture(m_hLavaTrapBaseImage);
	pGraphics->UnloadTexture(m_hLavaTrapFlameImage);

	pGraphics->UnloadTexture(m_hExplosionImage);

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

	// Delete tower flyweight
	delete m_pTowerFlyweight;

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
	pGraphics->UnloadTexture(m_hBuildModeHUD);
	pGraphics->UnloadTexture(m_hShotgunPic);
	pGraphics->UnloadTexture(m_hShotgunThumb);
	pGraphics->UnloadTexture(m_hARPic);
	pGraphics->UnloadTexture(m_hARThumb);
	pGraphics->UnloadTexture(m_hRLPic);
	pGraphics->UnloadTexture(m_hRLThumb);
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

#if ARCADE_MODE
		 m_vtStick = pInput->GetLeftJoystick(0);
	 
	 if(abs(m_vtStick.x) < 0.2f)
		 m_vtStick.x = 0.0f;
	 if(abs(m_vtStick.y) < 0.2f)
		 m_vtStick.y = 0.0f;

	 if(m_vtStick == SGD::Vector{0.0f, 0.0f})
		 m_bAccept = true;
#endif

	// Manipulate the mouse here
	SGD::Point mousePt = {0.0f, 0.0f};
	mousePt = pInput->GetMousePosition();

	//NOTE: to remove for testing the drone only
	if (pInput->IsKeyPressed(SGD::Key::P))
	{
		CreateDroneMessage* pMsg = new CreateDroneMessage();
		pMsg->QueueMessage();
			pMsg = nullptr;
	}

	if (m_bCreditsStarted == false && m_fWinTimer == 5.0f && m_bHasLost == false)
		// Press Escape (PC) or Start (Xbox 360) to toggle pausing
	{
#if !ARCADE_MODE
		m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, (unsigned int)SGD::Button::Start);
#endif
#if ARCADE_MODE
		m_bTHEBOOL = pInput->IsButtonPressed(0, 6);
#endif
		if (m_bTHEBOOL)
		{
			if (m_pShop->IsOpen() == false)
				m_bIsPaused = !m_bIsPaused;
			if(m_bIsPaused == true && pGraphics->IsCursorShowing() == false)
			{
				pGraphics->TurnCursorOn();
			}
		}

	// enter shop DELETE ME AFTER SHOP FUNCTIONS PROPERLY
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
	// Start the wave if in build mode
	if(zombieFactory->IsBuildMode() == true && !m_pShop->IsOpen() && (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, (unsigned int)SGD::Button::Back) || 
		pInput->IsButtonPressed(1, 6)))
		zombieFactory->SetBuildTImeRemaining(0.0f);

#if !ARCADE_MODE
	// Toggle the camera mode
	if(pInput->IsButtonPressed(0, (unsigned int)SGD::Button::Y) || pInput->IsKeyPressed(SGD::Key::Spacebar))
	{
		//TOGGLE THE CAMERA
	}
#endif

#pragma region Pause Menu Navigation Clutter
		// Handle pause menu input
		// If we're paused
		if (m_bIsPaused)
		{
			//-----------------------------------------------------------------------
			// --- Handling what tab we're in ---
			// If we're in the Main menu OF the pause menu. 
			if ( m_nPauseMenuTab == PauseMenuTab::TAB_MAIN )
			{

				// --- Scrolling through options ---
				// If the down arrow (PC), or down dpad (Xbox 360) are pressed
				// Move the cursor (selected item) down
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed ( SGD::Key::Down ) || pInput->IsDPadPressed ( 0 , SGD::DPad::Down );
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.y > 0;
#endif
				if ( m_bTHEBOOL )
				{
					// TODO: Add sound fx for going up and down
					++m_nPauseMenuCursor;

					// Wrap around the options
					if ( m_nPauseMenuCursor > PauseMenuOption::PAUSE_EXIT )
						m_nPauseMenuCursor = PauseMenuOption::PAUSE_RESUME;
#if ARCADE_MODE
					m_bAccept = false;
#endif
				}
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.y < 0;
#endif
				// Move the cursor (selected item) up
				if (m_bTHEBOOL)
				{
					--m_nPauseMenuCursor;

					// Wrap around the options
					if (m_nPauseMenuCursor < PauseMenuOption::PAUSE_RESUME)
						m_nPauseMenuCursor = PauseMenuOption::PAUSE_EXIT;
#if ARCADE_MODE
					m_bAccept = false;
#endif
				}


				// --- Selecting an option ---
				// If the enter key (PC) or A button (Xbox 360) are pressed
				// Select the item
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = pInput->IsButtonPressed(0, 0);
#endif
				if (m_bTHEBOOL)
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
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.y < 0;
#endif
				if (m_bTHEBOOL)
				{
					// TODO: Add sound fx for going up and down
					++m_nPauseMenuCursor;

					// Wrap around the options
					if (m_nPauseMenuCursor > PauseMenuOptionsOption::OPTION_GOBACK)
						m_nPauseMenuCursor = PauseMenuOptionsOption::OPTION_MUSIC;

#if ARCADE_MODE
					m_bAccept = false;
#endif
				}
				// If the up arrow (PC), or up dpad (Xbox 360) are pressed
				// Move the cursor (selected item) up
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.y > 0;
#endif
				if (m_bTHEBOOL)
				{
					--m_nPauseMenuCursor;

					// Wrap around the options
					if (m_nPauseMenuCursor < PauseMenuOptionsOption::OPTION_MUSIC)
						m_nPauseMenuCursor = PauseMenuOptionsOption::OPTION_GOBACK;

#if ARCADE_MODE
					m_bAccept = false;
#endif
				}

				// --- Increasing an option ---
				// If the right key (PC) or right dpad (Xbox 360) are pressed
				// Increase the value
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Right) || pInput->IsDPadPressed(0, SGD::DPad::Right);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.x > 0;
#endif
				if (m_bTHEBOOL)
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
#if ARCADE_MODE
					m_bAccept = false;
#endif
				}
				// --- Decreasing an option ---
				// If the left key (PC) or left dpad (Xbox 360) are pressed
				// Decrease the value
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Left) || pInput->IsDPadPressed(0, SGD::DPad::Left);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = m_bAccept && m_vtStick.x < 0;
#endif
				if (m_bTHEBOOL)
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
#if ARCADE_MODE
					m_bAccept = false;
#endif
				}

				// --- Selecting an option ---
				// If the enter key (PC) or A button (Xbox 360) are pressed
				// Select the item
#if !ARCADE_MODE
				m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A);
#endif
#if ARCADE_MODE
				m_bTHEBOOL = pInput->IsButtonPressed(0,0);
#endif
				if (m_bTHEBOOL)
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

#if !ARCADE_MODE
	m_bTHEBOOL = (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A));
#endif
#if ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed(0,0);
#endif
	if (m_bCreditsStarted == true && m_bTHEBOOL)
	{
		// Since there's only one state..go back to main menu
		pGame->ChangeState(MainMenuState::GetInstance());
		return true;
	}
	if (m_bHasLost == true && m_fLossTimer <= 0.0f)
	{
#if !ARCADE_MODE
		m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Up) || pInput->IsDPadPressed(0, SGD::DPad::Down);
#endif
#if ARCADE_MODE
		m_bTHEBOOL = m_vtStick != SGD::Vector{0.0f, 0.0f};
#endif
		if (m_bTHEBOOL)
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


	// -- Debugging Mode Input always last --
	if (pGame->IsDebugMode() && !m_pShop->IsOpen() && !m_bIsPaused )
	{
		#define DEBUG_MAX 5
		#define DEBUG_MIN 0

		if (pInput->IsKeyPressed(SGD::Key::Up))
		{
			int cur = pGame->GetDebugCurs();
			cur--;
			pGame->SetDebugCurs(cur);

			// Wrap around
			if (pGame->GetDebugCurs() < DEBUG_MIN)
				pGame->SetDebugCurs(DEBUG_MAX);
		}
		if (pInput->IsKeyPressed(SGD::Key::Down))
		{
			int cur = pGame->GetDebugCurs();
			cur++;
			pGame->SetDebugCurs(cur);

			// Wrap around
			if (pGame->GetDebugCurs() > DEBUG_MAX)
				pGame->SetDebugCurs(DEBUG_MIN);
		}

		if (pInput->IsKeyPressed(SGD::Key::Shift))
		{
			if (pGame->GetDebugCurs() == DEBUG_MIN)
				pGame->SetGod(!pGame->IsGod());

			if (pGame->GetDebugCurs() == 1)
				pGame->SetInfAmmo(!pGame->HasInfAmmo());

			if (pGame->GetDebugCurs() == 2)
				pGame->SetShowPaths(!pGame->IsShowingPaths());

			if (pGame->GetDebugCurs() == 3)
				pGame->SetShowRects(!pGame->IsShowingRects());

			if (pGame->GetDebugCurs() == 4)
				dynamic_cast<Player*>(m_pPlayer)->SetScore(dynamic_cast<Player*>(m_pPlayer)->GetScore() + 1000000);

			if (pGame->GetDebugCurs() == 5)
				pGame->SetShowPos(!pGame->IsShowingPos());

		}
	}

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
		m_pEntities->CheckCollisions(BUCKET_ENEMIES, BUCKET_DRONE);
		m_pEntities->CheckCollisions(BUCKET_ENEMIES, BUCKET_TRAPS);
		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_SHOP);
		//draw grid rectangle

		// Update the stat tracker
		if(zombieFactory->IsBuildMode() == false)
			m_pStatTracker->IncreaseTime(elapsedTime, true);
		else
			m_pStatTracker->IncreaseTime(elapsedTime, false);
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


		// Render the entities
		m_pEntities->RenderAll();
		// render particles
		m_pParticleManager->Render();
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
		pGraphics->DrawString(fps.c_str(), { 20, 560 }, { 255, 0, 0 });

		// -- Render HUD --
		if (!m_bIsPaused)
		{
			if (!m_pShop->IsOpen())
			{
				// If we're in build mode
				if (zombieFactory->IsBuildMode())
				{
					// -- Draw the build mode hud --
					pGraphics->DrawTexture(m_hBuildModeHUD, { 0, 0 });


					// --- Draw the pictures of the items and tints ---
					Inventory* inv = player->GetInventory();

					// Draw the walls picture if the player has more than 0 of them
					if (inv->GetWalls() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 54, 496 }, SGD::Rectangle({ 0, 0 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						// Tint it red
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 54, 496 }, SGD::Rectangle({ 0, 0 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the windows picture
					if (inv->GetWindows() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 123, 496 }, SGD::Rectangle({ 0, 32 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 123, 496 }, SGD::Rectangle({ 0, 32 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the bear traps
					if (inv->GetBearTraps() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 191, 496 }, SGD::Rectangle({ 0, 64 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 191, 496 }, SGD::Rectangle({ 0, 64 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the mines
					if (inv->GetMines() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 259, 496 }, SGD::Rectangle({ 0, 96 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 259, 496 }, SGD::Rectangle({ 0, 96 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the MG Towers
					if (inv->GetMachineGunTowers() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 327, 496 }, SGD::Rectangle({ 0, 156 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 327, 496 }, SGD::Rectangle({ 0, 156 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the Maple Syrup Towers
					if (inv->GetMapleSyrupTowers() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 395, 496 }, SGD::Rectangle({ 0, 225 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 395, 496 }, SGD::Rectangle({ 0, 225 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the Hockey Stick Towers
					if (inv->GetHockeyStickTowers() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 463, 496 }, SGD::Rectangle({ 0, 273 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 463, 496 }, SGD::Rectangle({ 0, 273 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the Laser Towers
					if (inv->GetLaserTowers() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 531, 496 }, SGD::Rectangle({ 0, 321 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 531, 496 }, SGD::Rectangle({ 0, 321 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the Lava Traps
					if (inv->GetLavaTraps() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 599, 496 }, SGD::Rectangle({ 0, 352 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 599, 496 }, SGD::Rectangle({ 0, 352 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });

					// Draw the Spike Traps
					if (inv->GetSpikeTraps() > 0)
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 667, 496 }, SGD::Rectangle({ 0, 384 }, SGD::Size(32, 32)), 0, {}, { 255, 255, 255 }, { 2.0f, 2.0f });
					else
						pGraphics->DrawTextureSection(m_hPlaceablesImage, { 667, 496 }, SGD::Rectangle({ 0, 384 }, SGD::Size(32, 32)), 0, {}, { 255, 0, 0 }, { 2.0f, 2.0f });
					
					// Draw the selected box based on the player's selected placeable
					switch (player->GetCurrPlaceable())
					{
					case 0:		// 1 -  Walls
						pGraphics->DrawRectangle(DRAWSELECTED(53, 494, 66, 66));
							break;

					case 1:		// 2 - Windows
						pGraphics->DrawRectangle(DRAWSELECTED(121, 494, 66, 66));
						break;

					case 2:		// 3 - Bear Traps
						pGraphics->DrawRectangle(DRAWSELECTED(189, 494, 66, 66));
						break;

					case 3:		// 4 - Mines
						pGraphics->DrawRectangle(DRAWSELECTED(257, 494, 66, 66));
						break;

					case 4:		// 5 - MG Tower
						pGraphics->DrawRectangle(DRAWSELECTED(325, 494, 66, 66));
						break;

					case 5:		// 6 - Maple Syrup Tower
						pGraphics->DrawRectangle(DRAWSELECTED(393, 494, 66, 66));
						break;

					case 6:		// 7 - Hockey Stick Tower
						pGraphics->DrawRectangle(DRAWSELECTED(461, 494, 66, 66));
						break;

					case 7:		// 8 - Laser Tower
						pGraphics->DrawRectangle(DRAWSELECTED(529, 494, 66, 66));
						break;

					case 8:		// 9 - Lava Trap
						pGraphics->DrawRectangle(DRAWSELECTED(597, 494, 66, 66));
						break;

					case 9:		// 10 - Spike Trap
						pGraphics->DrawRectangle(DRAWSELECTED(665, 494, 66, 66));
						break;
						
					case -1:	// If nothing is selected, dont draw anything
						break;
					}
				}
				// If we're kicking ass
				else
				{		
					// --- Draw the killing mode hud ---
					pGraphics->DrawTexture(m_hHUD, { 0, 0 });
					
					// --- Draw the pictures of the weapons ---
					Weapon* weapons = player->GetWeapons();

					// Assault Rifle
					if (weapons[0].GetCurrAmmo() > 0)
						pGraphics->DrawTextureSection(m_hARThumb, { 268, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)));
					else
						pGraphics->DrawTextureSection(m_hARThumb, { 268, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)), 0, {}, { 255, 0, 0 });

					// Shotgun
					if (weapons[1].GetCurrAmmo() > 0)
						pGraphics->DrawTextureSection(m_hShotgunThumb, { 331, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)));
					else
						pGraphics->DrawTextureSection(m_hShotgunThumb, { 331, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)), 0, {}, { 255, 0, 0 });

					// Rocket Launcher
					if (weapons[2].GetCurrAmmo() > 0)
						pGraphics->DrawTextureSection(m_hRLThumb, { 394, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)));
					else
						pGraphics->DrawTextureSection(m_hRLThumb, { 394, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)), 0, {}, { 255, 0, 0 });

					// Who the fuck knows
					if (weapons[3].GetCurrAmmo() > 0)
						pGraphics->DrawTextureSection(m_hBackground, { 455, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)));
					else
						pGraphics->DrawTextureSection(m_hBackground, { 455, 498 }, SGD::Rectangle({ 0, 0 }, SGD::Size(56, 57)), 0, {}, { 255, 0, 0 });

					switch (player->GetCurrWeapon())
					{
						case 0:		// 1 - Assault Rifle
							pGraphics->DrawRectangle(DRAWSELECTED(266, 497, 60, 60));
							break;

						case 1:		// 2 - Shotgun
							pGraphics->DrawRectangle(DRAWSELECTED(329, 497, 60, 60));
							break;

						case 2:		// 3 - Rocket Launcher
							pGraphics->DrawRectangle(DRAWSELECTED(392, 497, 58, 60));
							break;

						case 3:		// 4 - ???
							pGraphics->DrawRectangle(DRAWSELECTED(455, 497, 58, 60));
							break;

					}
				}
				


				// -- Draw the score --
				string score = "Points";
				m_pFont->Draw(std::to_string(player->GetScore()).c_str(), 665, 60, 0.45f, { 255, 255, 255 });
				m_pFont->Draw(score.c_str(), 665, 35, 0.45f, { 255, 255, 255 });

				// -- Draw the wave number --
				string waveNum = "Wave: ";
				waveNum += std::to_string(zombieFactory->GetWave());
				m_pFont->Draw(waveNum.c_str(), 64, 38, 0.5f, { 255, 255, 255 });

				if (zombieFactory->IsBuildMode())
				{

					//string timeRemaining = "Time remaining: ";
					//timeRemaining += (std::to_string(zombieFactory->GetBuildTimeRemaining() / 100.0f));
					//timeRemaining += " secs";
					//m_pFont->Draw(timeRemaining.c_str(), 180, 30, 0.6f, { 255, 255, 255 });

					if (m_bHasLost == false)
						m_pFont->Draw("Time to Build! Press Enter to Start Wave!", 220, 38, 0.4f, { 255, 255, 0 });
				}
				// -- Draw the number of enemies remaining [during fight mode] --
				else
				{

					string enemiesRemaining = "Enemies Remaining: ";
					m_pFont->Draw(enemiesRemaining.c_str(), 68, 66, 0.45f, { 255, 255, 255 });

					int numOfEnemies = zombieFactory->GetEnemiesRemaining();

					if (numOfEnemies <= 3)
						m_pFont->Draw(std::to_string(numOfEnemies).c_str(), 264, 66, 0.45f, { 255, 0, 0 });
					else
						m_pFont->Draw(std::to_string(numOfEnemies).c_str(), 264, 66, 0.45f, { 255, 255, 255 });

				}

				// TODO: In polish, make #defines for hud spacing so you dont have to add/subtract all
				// the goddamn time.
				// -- Draw the items and their quantities --

				// Get the inventory
				Inventory* inv = player->GetInventory();

				// If we're in build mode
				if (zombieFactory->IsBuildMode())
				{
					// Draw the number of walls
					if (m_nGamemode != 2)
						m_pFont->Draw(std::to_string(inv->GetWalls()).c_str(), 54, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of windows
					if (m_nGamemode != 2)
						m_pFont->Draw(std::to_string(inv->GetWindows()).c_str(), 123, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of beartraps
					m_pFont->Draw(std::to_string(inv->GetBearTraps()).c_str(), 191, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of mines
					m_pFont->Draw(std::to_string(inv->GetMines()).c_str(), 260, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of MG Towers
					m_pFont->Draw(std::to_string(inv->GetMachineGunTowers()).c_str(), 327, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of Maple Syrup Towers
					m_pFont->Draw(std::to_string(inv->GetMapleSyrupTowers()).c_str(), 395, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of Hockey Stick Towers
					m_pFont->Draw(std::to_string(inv->GetHockeyStickTowers()).c_str(), 463, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of Laser Towers
					m_pFont->Draw(std::to_string(inv->GetLaserTowers()).c_str(), 531, 496, 0.4f, { 255, 255, 255 });

					// Draw the number of Lava Traps
					m_pFont->Draw(std::to_string(inv->GetLavaTraps()).c_str(), 599, 496, 0.4f, { 255, 255, 255 });
					
					// Draw the number of Spike Traps
					m_pFont->Draw(std::to_string(inv->GetSpikeTraps()).c_str(), 667, 496, 0.4f, { 255, 255, 255 });
				}

				// If we're not in build mode
				else
				{

					// --- Draw grenades and healthpacks --

					// Draw the number of grenades
					m_pFont->Draw(std::to_string(inv->GetGrenades()).c_str(), 98, 532, 0.4f, { 255, 255, 255 });

					// Draw the number of healthpacks
					m_pFont->Draw(std::to_string(inv->GetHealthPacks()).c_str(), 163, 532, 0.4f, { 255, 255, 255 });

					// -- Draw the selected weapon and it's ammo -- 

					// Get the weapons
					Weapon* weapons = player->GetWeapons();
					SGD::HTexture textures[4] = { m_hARPic, m_hShotgunPic, m_hRLPic, m_hBackground };

					// Draw the picture of the selected pic
					pGraphics->DrawTextureSection(textures[player->GetCurrWeapon()], { 590, 500 }, SGD::Rectangle({ 0, 0 }, SGD::Size(115, 48)));

					// Draw the ammo of the selected weapon
					if (weapons[player->GetCurrWeapon()].GetCurrAmmo() < 10)
						m_pFont->Draw(std::to_string(weapons[player->GetCurrWeapon()].GetCurrAmmo()).c_str(), 695, 515, 0.6f, { 255, 0, 0 });
					else
						m_pFont->Draw(std::to_string(weapons[player->GetCurrWeapon()].GetCurrAmmo()).c_str(), 695, 515, 0.6f, { 255, 255, 255 });
				}

				//Draw the grid rectangle
				SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
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
		// If you have lost render You Lose and go to replay menu
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


	// -- Render Debugging Mode over everything else --

	// If we're debugging
	Game* pGame = Game::GetInstance();
	if (pGame->IsDebugMode() && !m_pShop->IsOpen() && !m_bIsPaused)
	{
		// Render the debug menu box
		pGraphics->DrawRectangle(SGD::Rectangle({ 19, 110 }, SGD::Size(205, 331)), { 128, 0, 0, 0 });

		int pos = pGame->GetDebugCurs();
		// Draw the selected image
		pGraphics->DrawRectangle(SGD::Rectangle({ 10.0f, float(116 + 20 * pos) }, SGD::Size(10, 10)), { 255, 255, 0 });
		// Render the options
		if (pGame->IsGod())
			pGraphics->DrawString("God Mode", { 20, 111 }, { 0, 255, 0 });
		else
			pGraphics->DrawString("God Mode", { 20, 111 }, { 255, 0, 0 });

		if (pGame->HasInfAmmo())
			pGraphics->DrawString("Infinite Ammo", { 20, 131 }, { 0, 255, 0 });
		else
			pGraphics->DrawString("Infinite Ammo", { 20, 131 }, { 255, 0, 0 });


		if (pGame->IsShowingPaths())
			pGraphics->DrawString("Show Zombie Paths", { 20, 151 }, { 0, 255, 0 });
		else
			pGraphics->DrawString("Show Zombie Paths", { 20, 151 }, { 255, 0, 0 });

		if (pGame->IsShowingRects())
			pGraphics->DrawString("Show Collision Rects", { 20, 171 }, { 0, 255, 0 });
		else
			pGraphics->DrawString("Show Collision Rects", { 20, 171 }, { 255, 0, 0 });

		pGraphics->DrawString("Give 1000000 Cash", { 20, 191 }, { 255, 255, 0 });

		if (pGame->IsShowingPos())
			pGraphics->DrawString("Show Player Position", { 20, 211 }, { 0, 255, 0 });
		else
			pGraphics->DrawString("Show Player Position", { 20, 211 }, { 255, 0, 0 });



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
		self->m_pEntities->AddEntity(beaver, BUCKET_ENEMIES);
		beaver->Release();
		beaver = nullptr;
	}
		break;
	case MessageID::MSG_CREATE_FAST_ZOMBIE:
	{
		const CreateFastZombieMessage* pCreateMessage = dynamic_cast<const CreateFastZombieMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*zambie = self->CreateFastZombie(pCreateMessage->GetX(), pCreateMessage->GetY());
		self->m_pEntities->AddEntity(zambie, BUCKET_ENEMIES);
		zambie->Release();
		zambie = nullptr;
	}
		break;
	case MessageID::MSG_CREATE_SLOW_ZOMBIE:
	{
		const CreateSlowZombieMessage* pCreateMessage = dynamic_cast<const CreateSlowZombieMessage*>(pMsg);
		GameplayState* self = GameplayState::GetInstance();
		Entity*zambie = self->CreateSlowZombie(pCreateMessage->GetX(), pCreateMessage->GetY());
		self->m_pEntities->AddEntity(zambie, BUCKET_ENEMIES);
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
			SGD::AudioManager::GetInstance()->PlayAudio(self->m_hShotgunShoot);
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

										  /*Enemy* enemy = dynamic_cast<Enemy*>(ent);
										  if (enemy && enemy->GetAIComponent()->GetAlpha() == nullptr)
										  {
											  if (ent->GetType() == Entity::ENT_ZOMBIE_SLOW)
												  g->zombieFactory->SetSlowAlpha(nullptr);
											  else if (ent->GetType() == Entity::ENT_ZOMBIE_FAST)
												  g->zombieFactory->SetFastAlpha(nullptr);
											  else if (ent->GetType() == Entity::ENT_ZOMBIE_BEAVER)
												  g->zombieFactory->SetBeaverAlpha(nullptr);
										  }

										vector<IEntity*> vec = g->m_pEntities->GetBucket(BUCKET_ENEMIES);
										bool alphaed = false;
										for (unsigned int i = 0; i < vec.size(); i++)
										{
											if (ent->GetType() == vec[i]->GetType())
											{
												if (alphaed)
												{
													if (ent->GetType() == Entity::ENT_ZOMBIE_SLOW)
														dynamic_cast<Enemy*>(vec[i])->GetAIComponent()->SetAlpha(g->zombieFactory->GetSlowAlpha());
													else if (ent->GetType() == Entity::ENT_ZOMBIE_FAST)
														dynamic_cast<Enemy*>(vec[i])->GetAIComponent()->SetAlpha(g->zombieFactory->GetFastAlpha());
													else if (ent->GetType() == Entity::ENT_ZOMBIE_BEAVER)
														dynamic_cast<Enemy*>(vec[i])->GetAIComponent()->SetAlpha(g->zombieFactory->GetBeaverAlpha());
												}
												else
												{
													dynamic_cast<Enemy*>(vec[i])->GetAIComponent()->SetAlpha(nullptr);
													if (ent->GetType() == Entity::ENT_ZOMBIE_SLOW)
													{
														g->zombieFactory->SetSlowAlpha(dynamic_cast<Enemy*>(vec[i]));
													}
													else if (ent->GetType() == Entity::ENT_ZOMBIE_FAST)
														g->zombieFactory->SetFastAlpha(dynamic_cast<Enemy*>(vec[i]));
													else if (ent->GetType() == Entity::ENT_ZOMBIE_BEAVER)
														g->zombieFactory->SetBeaverAlpha(dynamic_cast<Enemy*>(vec[i]));

													alphaed = true;
												}
											}
										}*/
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
	case MessageID::MSG_CREATE_VECTOR_PARTICLE:
	{
		//Here you go Ryan
		const CreateParticleMessage* pCreateMessage = dynamic_cast<const CreateParticleMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		ParticleManager::GetInstance()->activate(pCreateMessage->GetEmitterID(), pCreateMessage->GetParticleEntity(), pCreateMessage->GetXOffset(), pCreateMessage->GetYOffset(), true, pCreateMessage->GetVector().ComputeNormalized());
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
	case MessageID::MSG_CREATE_TRAP:
	{
		const CreateTrapMessage* pCreateMessage = dynamic_cast<const CreateTrapMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* trap = g->CreateTrap(pCreateMessage->x, pCreateMessage->y, pCreateMessage->trapType);
		g->m_pEntities->AddEntity(trap, BUCKET_TRAPS);
		trap->Release();
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
	case MessageID::MSG_CREATE_MAPLE_SYRUP_BULLET:
	{
		const CreateMapleSyrupBulletMessage* pCreateMessage = dynamic_cast<const CreateMapleSyrupBulletMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* bullet = g->CreateMapleSyrupBullet(pCreateMessage->x, pCreateMessage->y, pCreateMessage->velocity, pCreateMessage->slowTime);
		g->m_pEntities->AddEntity(bullet, BUCKET_PROJECTILES);
		bullet->Release();
	}
		break;
	case MessageID::MSG_CREATE_DRONE:
	{
		const CreateDroneMessage* pCreateMessage = dynamic_cast<const CreateDroneMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* drone = g->CreateDrone();
		g->m_pEntities->AddEntity(drone, BUCKET_PROJECTILES);
		drone->Release();
	}
		break;
	case MessageID::MSG_WAVE_COMPLETE:
	{
		GameplayState* g = GameplayState::GetInstance();
		g->m_fSlowHealth *= g->m_fHealthScaling;
		g->m_fFastHealth *= g->m_fHealthScaling;
		g->m_fBeaverHealth *= g->m_fHealthScaling;
	}
		break;
	case MessageID::MSG_CREATE_GRENADE:
	{
		const CreateGrenadeMessage* pCreateMessage = dynamic_cast<const CreateGrenadeMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* grenade = g->CreateGrenade(pCreateMessage->x, pCreateMessage->y, pCreateMessage->force);
		g->m_pEntities->AddEntity(grenade, BUCKET_GRENADES);
		grenade->Release();
	}
		break;

	case MessageID::MSG_CREATE_SHOP:
	{
		const CreateShopMessage* pShop = dynamic_cast<const CreateShopMessage*>(pMsg);
		GameplayState* g = GameplayState::GetInstance();
		Entity* shop = g->CreateShop(pShop->GetX(), pShop->GetY());
		g->m_pEntities->AddEntity(shop, BUCKET_SHOP);
		shop->Release();
	}
		break;

	case MessageID::MSG_CREATE_EXPLOSION:
	{
											const CreateExplosionMessage* msg = dynamic_cast<const CreateExplosionMessage*>(pMsg);
									   GameplayState* g = GameplayState::GetInstance();
									   Entity* explosion = g->CreateExplosion(msg->x, msg->y, msg->damage, msg->radius);
									   g->m_pEntities->AddEntity(explosion, BUCKET_EXPLOSIONS);
									   explosion->Release();
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
	tempBeav->SetMaxHealth(m_fBeaverHealth);
	tempBeav->SetCurrHealth(m_fBeaverHealth);
	tempBeav->SetSpeed(m_fBeaverSpeed);
	tempBeav->SetVelocity({ 0, 0 });
	tempBeav->SetAmmoChance(m_fBeaverAmmoChance);
	tempBeav->SetSuperChance(m_fBeaverSuperChance);
	tempBeav->SetHealthChance(m_fBeaverHealthChance);
	tempBeav->SetRegeneration(m_fBeaverRegeneration);
	// AIComponent
	tempBeav->SetPlayer(m_pPlayer);

	/*AIComponent* aiComponent = tempBeav->GetAIComponent();

	if (zombieFactory->GetBeaverAlpha() == nullptr)
	{
		zombieFactory->SetBeaverAlpha(tempBeav);
		aiComponent->SetAlpha(nullptr);
	}

	else
	{
		aiComponent->SetAlpha(zombieFactory->GetBeaverAlpha());
	}*/

	return tempBeav;
}

Entity* GameplayState::CreateFastZombie(int _x, int _y) const
{
	FastZombie* zambie = new FastZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(m_fFastHealth);
	zambie->SetCurrHealth(m_fFastHealth);
	zambie->SetSpeed(m_fFastSpeed);
	zambie->SetVelocity({ 0, 0 });
	zambie->SetAmmoChance(m_fFastAmmoChance);
	zambie->SetSuperChance(m_fFastSuperChance);
	zambie->SetHealthChance(m_fFastHealthChance);
	zambie->SetRegeneration(m_fFastRegeneration);
	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	/*AIComponent* aiComponent = zambie->GetAIComponent();

	if (zombieFactory->GetFastAlpha() == nullptr)
	{
		zombieFactory->SetFastAlpha(zambie);
		aiComponent->SetAlpha(nullptr);
	}

	else
	{
		aiComponent->SetAlpha(zombieFactory->GetFastAlpha());
	}*/

	return zambie;
}

Entity* GameplayState::CreateSlowZombie(int _x, int _y) const
{
	SlowZombie* zambie = new SlowZombie;
	zambie->SetDamage(10);
	zambie->SetPosition({ (float)_x, (float)_y });
	zambie->SetAttackRange(1.0f);
	zambie->SetMaxHealth(m_fSlowHealth);
	zambie->SetCurrHealth(m_fSlowHealth);
	zambie->SetSpeed(m_fSlowSpeed);
	zambie->SetVelocity({ 0, 0 });
	zambie->SetAmmoChance(m_fSlowAmmoChance);
	zambie->SetSuperChance(m_fSlowSuperChance);
	zambie->SetHealthChance(m_fSlowHealthChance);
	zambie->SetRegeneration(m_fSlowRegeneration);
	// AIComponent
	zambie->SetPlayer(m_pPlayer);

	/*AIComponent* aiComponent = zambie->GetAIComponent();

	if (zombieFactory->GetSlowAlpha() == nullptr)
	{
		zombieFactory->SetSlowAlpha(zambie);
		aiComponent->SetAlpha(nullptr);
	}

	else
	{
		aiComponent->SetAlpha(zombieFactory->GetSlowAlpha());
	}*/

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
	SGD::Point playerCenter = m_pPlayer->GetPosition();
	switch (_Weapon)
	{
	case 0://Assault Rifle
	{
				// Adjust for projectile to come from center
			   playerCenter.x += 12;
			   playerCenter.y += 12;
			   AssaultRifleBullet* tempProj = new AssaultRifleBullet;
			   tempProj->SetDamage(m_pShop->GetARDamage());
			   tempProj->SetLifeTime(5);
			   tempProj->SetPosition(playerCenter);
			   SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			   pos.x += Camera::x;
			   pos.y += Camera::y;
			   SGD::Vector vec = pos - playerCenter;
			   vec.Normalize();
			   vec *= 1000;
			   tempProj->SetVelocity(vec);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hGunShoot);
		return tempProj;
	}
		break;
	case 1://Shotgun
	{
			  // Adjust for projectile to come from center
			   playerCenter.x += 12;
			   playerCenter.y += 12;
			   ShotgunPellet* tempProj = new ShotgunPellet;
			   tempProj->SetDamage(m_pShop->GetShotgunDamage());
			   tempProj->SetLifeTime(5);
			   tempProj->SetPosition(playerCenter);
			   SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			   pos.x += Camera::x;
			   pos.y += Camera::y;
			   SGD::Vector vec = pos - playerCenter;
			   vec.Normalize();
			   vec *= (float)(750 + rand() % 500);

			   // Rotate bullet at random direction
			   float degree = (-50 + rand() % 100) / 100.0f;
			   vec.Rotate(degree);

			   tempProj->SetVelocity(vec);
			   return tempProj;
	}
		break;
	case 2://Rocket launcher
	{
			   Rocket* tempProj = new Rocket;
			   tempProj->SetDamage(m_pShop->GetRLDamage());
			   tempProj->SetRadius(100.0f);
			   tempProj->SetLifeTime(5);
			   tempProj->SetPosition(playerCenter);
			   SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			   pos.x += Camera::x;
			   pos.y += Camera::y;
			   SGD::Vector vec = pos - playerCenter;
			   vec.Normalize();
			   vec *= 1000;
			   tempProj->SetVelocity(vec);

			   //ParticleManager::GetInstance()->activate("Smoke_Particle", tempProj, 0, 0);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hRocketShoot);

			   return tempProj;
	}
		break;
	case 3://HatTrick
	{
			// Adjust for projectile to come from center
			playerCenter.x += 8;
			playerCenter.y += 8;
			TrickShotBullet* tsb = new TrickShotBullet;
			tsb->SetDamage(m_pShop->GetHTDamage());
			tsb->SetPosition(playerCenter);
			tsb->SetVelocity({0.0f, 0.0f});
			SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
			pos.x += Camera::x;
			pos.y += Camera::y;
			SGD::Vector vec = pos - playerCenter;
			vec.Normalize();
			vec *= 300;
			tsb->SetForce(vec);

			// ADD SOUND

			return tsb;
			
			   break;
	}
	}

	return nullptr;
}

Entity* GameplayState::CreatePickUp(int pick, SGD::Point pos) const
{
	switch ( pick )
	{
	case (int)Entity::ENT_PICKUP_WALL:
	{
		WallPickup* wall = new WallPickup ();
		wall->SetPosition ( pos );
		wall->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "wall" ) );
		wall->SetCurrFrame ( 0 );
		wall->SetTimeOfFrame ( 0 );
		wall->SetCurrAnimation ( "wall" );
		return wall;
		break;
	}
	case (int)Entity::ENT_PICKUP_WINDOW:
	{
		WindowPickup* window = new WindowPickup ();
		window->SetPosition ( pos );
		window->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "window" ) );
		window->SetCurrFrame ( 0 );
		window->SetTimeOfFrame ( 0 );
		window->SetCurrAnimation ( "window" );
		return window;
		break;
	}
	case (int)Entity::ENT_PICKUP_AMMO:
	{
		AmmoPickup* ammo = new AmmoPickup ();
		ammo->SetPosition ( pos );
		ammo->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "ammo" ) );
		ammo->SetCurrFrame ( 0 );
		ammo->SetTimeOfFrame ( 0 );
		ammo->SetCurrAnimation ( "ammo" );
		return ammo;
		break;
	}
	case (int)Entity::ENT_PICKUP_HEALTHPACK:
	{
		HealthPackPickup* hp = new HealthPackPickup ();
		hp->SetPosition ( pos );
		hp->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "health" ) );
		hp->SetCurrFrame ( 0 );
		hp->SetTimeOfFrame ( 0 );
		hp->SetCurrAnimation ( "health" );
		return hp;
		break;
	}
	case (int)Entity::ENT_PICKUP_SUPER:
	{
		SuperPack* super = new SuperPack ();
		super->SetPosition ( pos );
		super->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "super" ) );
		super->SetCurrFrame ( 0 );
		super->SetTimeOfFrame ( 0 );
		super->SetCurrAnimation ( "super" );
		return super;
		break;
	}
	}

	return nullptr;
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
												  tower->SetTowerFlyweight(m_pTowerFlyweight);
												  tower->SetSellValue((int)(m_pShop->GetTowerPrice(0) * 0.75f));
												  tower->SetDamage(m_pTowerFlyweight->GetMachineGunDamage(0));
												  tower->SetFireRate(m_pTowerFlyweight->GetMachineGunFireRate(0));
												  tower->SetRange(m_pTowerFlyweight->GetMachineGunRange(0));

												  return tower;

	}
		break;
	case CreateTowerMessage::TOWER_MAPLE_SYRUP:
	{
												  MapleSyrupTower* tower = new MapleSyrupTower;

												  tower->SetPosition(SGD::Point((float)_x, (float)_y));
												  tower->SetBaseImage(m_hMapleSyrupBaseImage);
												  tower->SetGunImage(m_hMapleSyrupGunImage);
												  tower->SetTowerFlyweight(m_pTowerFlyweight);
												  tower->SetSellValue((int)(m_pShop->GetTowerPrice(1) * 0.75f));
												  tower->SetSlowTime(m_pTowerFlyweight->GetMapleSyrupEffectDuration(0));
												  tower->SetFireRate(m_pTowerFlyweight->GetMapleSyrupFireRate(0));
												  tower->SetRange(m_pTowerFlyweight->GetMapleSyrupRange(0));

												  return tower;
	}
		break;
	case CreateTowerMessage::TOWER_HOCKEY_STICK:
	{
												   HockeyStickTower* tower = new HockeyStickTower;

												   tower->SetPosition(SGD::Point((float)_x, (float)_y));
												   tower->SetBaseImage(m_hHockeyStickBaseImage);
												   tower->SetGunImage(m_hHockeyStickGunImage);
												   tower->SetTowerFlyweight(m_pTowerFlyweight);
												   tower->SetSellValue((int)(m_pShop->GetTowerPrice(2) * 0.75f));
												   tower->SetDamage(m_pTowerFlyweight->GetHockeyStickDamage(0));
												   tower->SetRotationRate(m_pTowerFlyweight->GetHockeyStickSpinRate(0));

												   return tower;
	}
		break;
	case CreateTowerMessage::TOWER_LASER:
	{
											LaserTower* tower = new LaserTower;

											tower->SetPosition(SGD::Point((float)_x, (float)_y));
											tower->SetBaseImage(m_hLaserBaseImage);
											tower->SetTowerFlyweight(m_pTowerFlyweight);
											tower->SetSellValue((int)(m_pShop->GetTowerPrice(3) * 0.75f));
											tower->SetDamage(m_pTowerFlyweight->GetLaserDamage(0));
											tower->SetRange(m_pTowerFlyweight->GetLaserRange(0));

											SGD::Event* pEvent = new SGD::Event("ASSESS_LASERS");
											pEvent->QueueEvent();

											return tower;
	}
		break;
	
	}

	return nullptr;
}

Entity * GameplayState::CreateTrap( int _x, int _y, int _trapType) const
{
	switch ( _trapType )
	{
	case CreateTrapMessage::TRAP_SPIKE:
	{
		SpikeTrap* spike = new SpikeTrap;

		spike->SetPosition ( SGD::Point ( (float)_x , (float)_y ) );
		spike->SetBaseImage ( m_hSpikeTrapBaseImage );
		spike->SetGunImage ( m_hSpikeTrapSpikeImage );

		return spike;
	}
		break;
	case CreateTrapMessage::TRAP_LAVA:
	{
		LavaTrap* lava = new LavaTrap;

		lava->SetPosition(SGD::Point((float)_x, (float)_y));
		lava->SetBaseImage(m_hLavaTrapBaseImage);
		lava->SetGunImage(m_hLavaTrapFlameImage);
		//BUG: occasionally a fire particle is not spawned on the lava trap
		CreateParticleMessage* msg = new CreateParticleMessage("Fire_Particle1", lava, 8, 8);
		msg->QueueMessage();
		msg = nullptr;
		return lava;

	}
		break;
	}

	return nullptr;
}

Entity* GameplayState::CreateGrenade(float x, float y, SGD::Vector velocity) const
{
	Grenade* grenade = new Grenade;

	grenade->SetPosition(SGD::Point(x, y));
	grenade->SetVelocity({0.0f,0.0f});
	grenade->SetForce(velocity);
	grenade->SetSprite ( AnimationManager::GetInstance ()->GetSprite ( "grenade" ) );
	grenade->SetCurrFrame ( 0 );
	grenade->SetTimeOfFrame ( 0 );
	grenade->SetCurrAnimation ( "grenade" );
	grenade->SetRadius(128.0f);
	grenade->SetDamage(200.0f);
	grenade->SetMass(1);
	grenade->SetStaticFrictionCoefficient(0.7f);
	grenade->SetDynamicFrictionCoefficient(100.0f);
	grenade->SetDetonationLength(1.25f);
	grenade->SetDetonationTimer(grenade->GetDetonationLength());


	return grenade;

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

Entity* GameplayState::CreateMapleSyrupBullet(int _x, int _y, SGD::Vector _velocity, float _slowTime) const
{
	MapleSyrupBullet* bullet = new MapleSyrupBullet;

	bullet->SetPosition(SGD::Point((float)_x, (float)_y));
	bullet->SetSlowTime(_slowTime);
	bullet->SetImage(m_hMapleSyrupBulletImage);
	bullet->SetVelocity(_velocity);

	return bullet;
}

Entity* GameplayState::CreateDrone() const
{
	Drone* drone = new Drone();

	drone->SetPlayer((Player*)m_pPlayer);
	drone->SetHealth((int)dynamic_cast<Player*>(m_pPlayer)->GetMaxHealth());
	drone->SetEntityManager(GetEntityManager());
	drone->SetNumberID(0);
	return drone;
}

Entity* GameplayState::CreateShop(float x, float y) const
{
	ShopEntity* shop = new ShopEntity();
	shop->SetX(x);
	shop->SetY(y);
	return shop;

}

Entity* GameplayState::CreateExplosion(float _x, float _y, float _damage, float _radius) const
{
	Explosion* explosion = new Explosion();

	explosion->SetPosition(SGD::Point(_x, _y));
	explosion->SetImage(m_hExplosionImage);
	explosion->SetDamage(_damage);
	explosion->SetRadius(_radius);

	return explosion;
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
		pGraphics->TurnCursorOff();

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

void GameplayState::LoadEnemyRecipes(string fileName)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	doc.LoadFile(fileName.c_str());

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Temp variabes to store data
	double healthScaling;
	double slowHealth;
	double slowSpeed;
	double slowRegen;
	double slowHealthChance;
	double slowAmmoChance;
	double slowSuperChance;
	double fastHealth;
	double fastSpeed;
	double fastRegen;
	double fastHealthChance;
	double fastAmmoChance;
	double fastSuperChance;
	double beaverHealth;
	double beaverSpeed;
	double beaverRegen;
	double beaverHealthChance;
	double beaverAmmoChance;
	double beaverSuperChance;

	// Load health scaling value
	pRoot->FirstChildElement("health_scaling")->Attribute("value", &healthScaling);

	// Load slow zombie stats
	TiXmlElement* slowZombie = pRoot->FirstChildElement("slow_zombie");
	slowZombie->FirstChildElement("health")->Attribute("value", &slowHealth);
	slowZombie->FirstChildElement("speed")->Attribute("value", &slowSpeed);
	slowZombie->FirstChildElement("regeneration")->Attribute("value", &slowRegen);
	slowZombie->FirstChildElement("health_chance")->Attribute("value", &slowHealthChance);
	slowZombie->FirstChildElement("ammo_chance")->Attribute("value", &slowAmmoChance);
	slowZombie->FirstChildElement("super_chance")->Attribute("value", &slowSuperChance);

	// Load fast zombie stats
	TiXmlElement* fastZombie = pRoot->FirstChildElement("fast_zombie");
	fastZombie->FirstChildElement("health")->Attribute("value", &fastHealth);
	fastZombie->FirstChildElement("speed")->Attribute("value", &fastSpeed);
	fastZombie->FirstChildElement("regeneration")->Attribute("value", &fastRegen);
	fastZombie->FirstChildElement("health_chance")->Attribute("value", &fastHealthChance);
	fastZombie->FirstChildElement("ammo_chance")->Attribute("value", &fastAmmoChance);
	fastZombie->FirstChildElement("super_chance")->Attribute("value", &fastSuperChance);

	// Load beaver zombie stats
	TiXmlElement* beaverZombie = pRoot->FirstChildElement("beaver_zombie");
	beaverZombie->FirstChildElement("health")->Attribute("value", &beaverHealth);
	beaverZombie->FirstChildElement("speed")->Attribute("value", &beaverSpeed);
	beaverZombie->FirstChildElement("regeneration")->Attribute("value", &beaverRegen);
	beaverZombie->FirstChildElement("health_chance")->Attribute("value", &beaverHealthChance);
	beaverZombie->FirstChildElement("ammo_chance")->Attribute("value", &beaverAmmoChance);
	beaverZombie->FirstChildElement("super_chance")->Attribute("value", &beaverSuperChance);

	// Assign values
	m_fHealthScaling = (float)healthScaling;
	m_fSlowHealth = (float)slowHealth;
	m_fSlowSpeed = (float)slowSpeed;
	m_fSlowRegeneration = (float)slowRegen;
	m_fSlowHealthChance = (float)slowHealthChance;
	m_fSlowAmmoChance = (float)slowAmmoChance;
	m_fSlowSuperChance = (float)slowSuperChance;
	m_fFastHealth = (float)fastHealth;
	m_fFastSpeed = (float)fastSpeed;
	m_fFastRegeneration = (float)fastRegen;
	m_fFastHealthChance = (float)fastHealthChance;
	m_fFastAmmoChance = (float)fastAmmoChance;
	m_fFastSuperChance = (float)fastSuperChance;
	m_fBeaverHealth = (float)beaverHealth;
	m_fBeaverSpeed = (float)beaverSpeed;
	m_fBeaverRegeneration = (float)beaverRegen;
	m_fBeaverHealthChance = (float)beaverHealthChance;
	m_fBeaverAmmoChance = (float)beaverAmmoChance;
	m_fBeaverSuperChance = (float)beaverSuperChance;
}
