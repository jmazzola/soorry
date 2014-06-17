#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "AnimationManager.h"
#include "Frame.h"
#include "Sprite.h"
#include "Game.h"
#include "WorldManager.h"
#include "Weapon.h"
#include "ShotgunPellet.h"
#include "Rocket.h"
#include "Projectile.h"
#include "AssaultRifleBullet.h"
#include "CreateProjectileMessage.h"
#include "GameplayState.h"
#include "CreatePlaceableMessage.h"
#include "Inventory.h"
#include "Tile.h"
#include "Camera.h"
#include "CreatePickupMessage.h"
#include "AIComponent.h"
#include "CreateParticleMessage.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "Tower.h"
#include "CreateTowerMessage.h"
#include "GameplayState.h"
#include "CreateTrapMessage.h"
#include "CreateGrenadeMessage.h"
#include "StatTracker.h"
#include "BitmapFont.h"
#include "DestroyEntityMessage.h"
#include "Shop.h"

#include "Game.h"

#include <queue>
using namespace std;

#define WALLPICK 0
#define WINDOWPICK 1
#define GRIDWIDTH 32
#define GRIDHEIGHT 32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define DEFAULT_MACHINE_GUN_RANGE 128.0f
#define DEFAULT_MAPLE_SYRUP_RANGE 96.0f
#define DEFAULT_HOCKEY_STICK_RANGE 48.0f

Player::Player () : Listener ( this )
{
	// Entity
	m_ptPosition = { 50 , 100 };
	m_vtVelocity = { 0 , 0 };


	// Animation/ Image
	m_pSprite = AnimationManager::GetInstance ()->GetSprite ( "player" );
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "player";


	// Player's variables
	m_nMaxHealth = 100.0f;
	m_nCurrHealth = 100.0f;
	m_nCurrWeapon = 0;
	m_nCurrPowerup = -1;
	m_nCurrPlaceable = 0;
	m_unScore = 0;
	m_unEnemiesKilled = 0;
	m_fSpeed = 250.0f;
	m_fScoreMultiplier = 0.0f;
	m_fTimeAlive = 0.0f;
	m_fRunningManTimer = 2.0f;
	m_fPickupMessageTimer = 0.0f;

	// Player Inventory
	m_pInventory = new Inventory();
	m_pInventory->SetBearTraps(100);
	m_pInventory->SetGrenades(10);
	m_pInventory->SetHealthPacks(3);
	m_pInventory->SetMines(1);
	m_pInventory->SetWalls(100);
	m_pInventory->SetWindows(100);
	m_pInventory->SetMachineGunTowers(20);
	m_pInventory->SetMapleSyrupTowers(20);
	m_pInventory->SetHockeyStickTowers(20);
	m_pInventory->SetLaserTowers(20);
	m_pInventory->SetLavaTraps(5);
	m_pInventory->SetSpikeTraps(5);
	m_pInventory->SetDroneCount(0);

	m_bHasAssaultRifle = true;
	m_bHasShotty = false;
	m_bHasRocketz = false;
	m_bHasHatTrick = false;

	//Camera Lock - starts static
	m_bStaticCamera = 1;
	m_fCameraLerpTimer = 1;

	m_pWeapons = new Weapon[ 4 ];
#pragma region Load Weapons
	Weapon tempWeapon;

	if (GameplayState::GetInstance()->GetGameMode() != 3)
	{
		//Assault rifle
		if (m_bHasAssaultRifle)
		{
			tempWeapon.SetCurrAmmo(200);
			tempWeapon.SetMaxAmmo(500);
			tempWeapon.SetFireRate(.2f);
			tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
			m_pWeapons[0] = tempWeapon;
		}
		else
		{
			tempWeapon.SetCurrAmmo(0);
			tempWeapon.SetMaxAmmo(500);
			tempWeapon.SetFireRate(.2f);
			tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
			m_pWeapons[0] = tempWeapon;
		}
	}
	else
	{
		//Assault rifle in beaver mode!
		tempWeapon.SetCurrAmmo(1000);
		tempWeapon.SetMaxAmmo(1000);
		tempWeapon.SetFireRate(.02f);
		tempWeapon.SetType(Guns::TYPE_ASSAULT_RIFLE);
		m_pWeapons[0] = tempWeapon;
	}

	if (GameplayState::GetInstance()->GetGameMode() != 3)
	{
		//Shotgun
		if (m_bHasShotty)
		{
			tempWeapon.SetCurrAmmo(100);
			tempWeapon.SetMaxAmmo(500);
			tempWeapon.SetFireRate(.5f);
			tempWeapon.SetType(Guns::TYPE_SHOTGUN);
			m_pWeapons[1] = tempWeapon;
		}
		else
		{
			tempWeapon.SetCurrAmmo(0);
			tempWeapon.SetMaxAmmo(500);
			tempWeapon.SetFireRate(.5f);
			tempWeapon.SetType(Guns::TYPE_SHOTGUN);
			m_pWeapons[1] = tempWeapon;
		}
	}

	if (GameplayState::GetInstance()->GetGameMode() != 3)
	{
		//rocket launcher
		if (m_bHasRocketz)
		{
			tempWeapon.SetCurrAmmo(20);
			tempWeapon.SetMaxAmmo(50);
			tempWeapon.SetFireRate(2);
			tempWeapon.SetType(Guns::TYPE_ROCKET_LAUNCHER);
			m_pWeapons[2] = tempWeapon;
		}
		else
		{
			tempWeapon.SetCurrAmmo(0);
			tempWeapon.SetMaxAmmo(50);
			tempWeapon.SetFireRate(2);
			tempWeapon.SetType(Guns::TYPE_ROCKET_LAUNCHER);
			m_pWeapons[2] = tempWeapon;
		}
	}

	if (GameplayState::GetInstance()->GetGameMode() != 3)
	{
		//Trick Shot
		if (m_bHasHatTrick)
		{
			tempWeapon.SetCurrAmmo(75);
			tempWeapon.SetMaxAmmo(75);
			tempWeapon.SetFireRate(0.75f);
			tempWeapon.SetType(Guns::TYPE_TRICKSHOT);
			m_pWeapons[3] = tempWeapon;
		}
		else
		{
			tempWeapon.SetCurrAmmo(0);
			tempWeapon.SetMaxAmmo(75);
			tempWeapon.SetFireRate(0.75f);
			tempWeapon.SetType(Guns::TYPE_TRICKSHOT);
			m_pWeapons[3] = tempWeapon;
		}
	}

#pragma endregion


	RegisterForEvent ( "TAKE_DAMAGE" );
	RegisterForEvent ( "INCREASE_SCORE" );

	// Create node chart
	WorldManager* pWorld = WorldManager::GetInstance ();
	int worldWidth = pWorld->GetWorldWidth ();
	int worldHeight = pWorld->GetWorldHeight ();
	m_nNodeChart = new int*[ worldWidth ];
	for ( int x = 0; x < worldWidth; x++ )
		m_nNodeChart[ x ] = new int[ worldHeight ];

	// load audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	m_hBlockPlace = pAudio->LoadAudio("resource/audio/block_place.wav");
	m_hBlockBreak = pAudio->LoadAudio("resource/audio/Block_Break.wav");
	m_hPickup = pAudio->LoadAudio("resource/audio/block_pickup.wav");
	m_hWalking = pAudio->LoadAudio("resource/audio/walking2.wav");
	m_hGunClick = pAudio->LoadAudio("resource/audio/Gun_Click.wav");
	m_hPlayerGrunt = pAudio->LoadAudio("resource/audio/Player_Grunt.wav");

	m_bCanLeftClick = true;
	m_bAccept = true;
	m_bTHEBOOL = false;
	m_vtStick = SGD::Vector{0.0f, 0.0f};

	m_bIsNearShop = false;
}


Player::~Player ()
{
	delete []m_pWeapons;
	delete m_pInventory;

	for ( int x = 0; x < WorldManager::GetInstance ()->GetWorldWidth (); x++ )
		delete [] m_nNodeChart[ x ];
	delete [] m_nNodeChart;

	//Delete audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	pAudio->UnloadAudio(m_hBlockPlace);
	pAudio->UnloadAudio(m_hBlockBreak);
	pAudio->UnloadAudio(m_hPickup);
	pAudio->UnloadAudio(m_hWalking);
	pAudio->UnloadAudio(m_hGunClick);
	pAudio->UnloadAudio(m_hPlayerGrunt);

	//for (unsigned int i = drones.size() -1; i > 0; i--)
	//{
		//m_pEntityManager->RemoveEntity(drones[i]);
		//delete drones[i];
	//}
}


/**********************************************************/
// Interface Methods

/*********************************************************/
// Update
//	- Handle input and move the character
void Player::Update ( float dt )
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance ();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance ();
	Game* pGame = Game::GetInstance ();
	WorldManager* pWorld = WorldManager::GetInstance ();
	//Update  all Timers
	m_pWeapons[m_nCurrWeapon].SetFireTimer(m_pWeapons[m_nCurrWeapon].GetFireTimer() - dt);
	m_fGrenadeTimer -= dt;
	m_fPlaceTimer -= dt;
	m_fSuperTimer -= dt;
	m_fRunningManTimer -= dt;
	m_fPickupMessageTimer -= dt;
	float trg = pInput->GetTrigger(0);

	if(abs(trg) < 0.1)
		trg = 0.0f;

	if ( trg == 0 )
	{
		m_bCanLeftTrigger = true;
		m_bCanRightTrigger = true;
	}

	if ( m_fRunningManTimer < 0.0f && isRunningMan )
	{
		m_fRunningManTimer = 0.3f;
		SGD::Point a = m_ptLastPos;
		SGD::Point b = m_ptPosition;
		float distance = sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
		if(distance < 70.0f)
			m_nCurrHealth -= 5.0f;
		m_ptLastPos = m_ptPosition;
	}

	SGD::Point pos = SGD::InputManager::GetInstance ()->GetMousePosition ();
	pos.x = (float)((int)(pos.x + Camera::x) / GRIDWIDTH);
	pos.y = (float)((int)(pos.y + Camera::y) / GRIDHEIGHT);

		// Grab the left stick for movement
	SGD::Vector move = pInput->GetLeftJoystick ( 0 );
	if ( abs ( move.x ) < 0.1f )
		move.x = 0.0f;
	if ( abs ( move.y ) < 0.1f )
		move.y = 0.0f;

	// Grab the right stick for shooting/placing
#if !ARCADE_MODE
	SGD::Vector shoot = pInput->GetRightJoystick ( 0 );
	if ( abs ( shoot.x ) < 0.2f )
		shoot.x = 0.0f;
	if ( abs ( shoot.y ) < 0.2f )
		shoot.y = 0.0f;
#endif
#if ARCADE_MODE
	SGD::Vector shoot = pInput->GetLeftJoystick ( 1 );
	if ( abs ( shoot.x ) < 0.2f )
		shoot.x = 0.0f;
	if ( abs ( shoot.y ) < 0.2f )
		shoot.y = 0.0f;
#endif



#if !ARCADE_MODE
		m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::E) && m_pZombieWave->IsBuildMode() && m_bIsNearShop;
#endif
#if ARCADE_MODE
		m_bTHEBOOL = (pInput->IsButtonPressed(0, 2) || pInput->IsButtonPressed(1,2) ) && m_bAccept;
#endif

	// Check if we're at the shop
	if (m_bTHEBOOL)
	{
		Shop* shop = GameplayState::GetInstance()->GetShop();
		shop->SetShopStatus(true);
#if ARCADE_MODE
		m_bAccept = false;
#endif
	}

	m_bIsNearShop = false;

	if (pInput->IsKeyUp(SGD::Key::MouseLeft))
		m_bCanLeftClick = true;


	if ( m_nCurrHealth <= 0.0f )
	{
		GameplayState::GetInstance ()->HasLost();
		return;
	}
	// Regenerate health
	m_nCurrHealth += 7.0f * dt;
	if ( m_nCurrHealth > m_nMaxHealth )
		m_nCurrHealth = m_nMaxHealth;

	//Update Timers



#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsButtonPressed(0, (unsigned int)SGD::Button::Y) || pInput->IsKeyPressed(SGD::Key::Spacebar);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = (pInput->IsButtonPressed(0, 2) || pInput->IsButtonPressed(1,2)) && m_bAccept;
#endif
	if (m_bTHEBOOL)
	{
		if (m_bStaticCamera == true)
		{
			m_bStaticCamera = false;
			m_fCameraLerpTimer = 1;
		}
		else
		{
			m_bStaticCamera = true;
			m_fCameraLerpTimer = 0;
		}

		m_bAccept = false;
	}
	if ( (shoot.x != 0.0f || shoot.y != 0.0f) && m_pZombieWave->IsBuildMode() == false)
	{
		SGD::Point pos = SGD::InputManager::GetInstance ()->GetMousePosition ();
		SGD::Point playerPos = GetPosition ();
		playerPos.x -= Camera::x;
		playerPos.y -= Camera::y;
		SGD::Point dir = playerPos;
		dir.x += shoot.x * 150 + 16;
		dir.y += shoot.y * 150 + 16;

		pInput->SetMousePosition ( dir );
	}
	else if ( (shoot.x != 0.0f || shoot.y != 0.0f) && m_pZombieWave->IsBuildMode () == true )
	{
		SGD::Point pos = SGD::InputManager::GetInstance ()->GetMousePosition ();
		if ( trg == 0.0f )
		{
			pos.x += shoot.x;
			pos.y += shoot.y;
		}
		else
		{
			pos.x += shoot.x * 1.5f;
			pos.y += shoot.y * 1.5f;
		}

		pInput->SetMousePosition( pos );
	}

	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance ();
	// Grab the mouse movement to hide the cursor if necessary
	SGD::Vector mouseMove = pInput->GetMouseMovement ();

	

	// Input
	// Move Left
	if ( pInput->IsKeyDown ( SGD::Key::A ) == true || move.x < 0.0f )
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x -= m_fSpeed * dt;

		if ( pWorld->CheckCollision ( this ) == true || m_ptPosition.x < 0 )
		{
			m_ptPosition.x = oldpos;
		}
		else
			StatTracker::GetInstance()->Walk(m_fSpeed*dt);
		if ( pAudio->IsAudioPlaying ( m_hWalking ) == false )
		{
			pAudio->PlayAudio ( m_hWalking );
		}
		AnimationManager::GetInstance ()->Update ( m_antsAnimation , dt );
	}
	// Move Right
	if ( pInput->IsKeyDown ( SGD::Key::D ) == true || move.x > 0.0f )
	{
		float oldpos = m_ptPosition.x;
		m_ptPosition.x += m_fSpeed * dt;

		if ( pWorld->CheckCollision ( this ) == true || m_ptPosition.x >= pWorld->GetWorldWidth () * pWorld->GetTileWidth () - pWorld->GetTileWidth () )
			m_ptPosition.x = oldpos;
		else
			StatTracker::GetInstance()->Walk(m_fSpeed*dt);
		if ( pAudio->IsAudioPlaying ( m_hWalking ) == false )
		{
			pAudio->PlayAudio ( m_hWalking );
		}
		AnimationManager::GetInstance ()->Update ( m_antsAnimation , dt );
	}
	// Move Up
	if ( pInput->IsKeyDown ( SGD::Key::W ) == true || move.y < 0.0f )
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y -= m_fSpeed * dt;

		if ( pWorld->CheckCollision ( this ) == true || m_ptPosition.y < 0 )
			m_ptPosition.y = oldpos;
		else
			StatTracker::GetInstance()->Walk(m_fSpeed*dt);
		if ( pAudio->IsAudioPlaying ( m_hWalking ) == false )
		{
			pAudio->PlayAudio ( m_hWalking , true );
		}
		AnimationManager::GetInstance ()->Update ( m_antsAnimation , dt );
	}
	// Move Down
	if ( pInput->IsKeyDown ( SGD::Key::S ) == true || move.y > 0.0f )
	{
		float oldpos = m_ptPosition.y;
		m_ptPosition.y += m_fSpeed * dt;

		if ( pWorld->CheckCollision ( this ) == true || m_ptPosition.y >= pWorld->GetWorldHeight () * pWorld->GetTileHeight () - pWorld->GetTileHeight () )
			m_ptPosition.y = oldpos;
		else
			StatTracker::GetInstance()->Walk(m_fSpeed*dt);
		if ( pAudio->IsAudioPlaying ( m_hWalking ) == false )
		{
			pAudio->PlayAudio ( m_hWalking , true );
		}
		AnimationManager::GetInstance ()->Update ( m_antsAnimation , dt );
	}
	// Use Healthpack
#if !ARCADE_MODE
		m_bTHEBOOL = (pInput->IsKeyDown ( SGD::Key::Q ) == true || pInput->IsButtonPressed ( 0 , (unsigned int)SGD::Button::A ));
#endif
#if ARCADE_MODE
		m_bTHEBOOL = pInput->IsButtonPressed(0, 3) || pInput->IsButtonPressed(1, 3);
#endif
		
	if ( m_bTHEBOOL && m_pInventory->GetHealthPacks () > 0 && m_nCurrHealth < m_nMaxHealth )
	{
		m_nCurrHealth = m_nMaxHealth;
		unsigned int newset = m_pInventory->GetHealthPacks ();
		--newset;
		m_pInventory->SetHealthPacks ( newset );
	}
	// Throw Grenade
#if !ARCADE_MODE
		m_bTHEBOOL = (pInput->IsKeyDown ( SGD::Key::F ) == true || (trg < -0.1f && m_bCanLeftTrigger));
#endif
#if ARCADE_MODE
		m_bTHEBOOL = pInput->IsButtonReleased(1, 0) || pInput->IsButtonReleased(0, 0);
#endif

	if ( m_bTHEBOOL && m_pInventory->GetGrenades () > 0 && m_pZombieWave->IsBuildMode() == false && m_fGrenadeTimer < 0.0f)
	{
		SGD::Vector force;
		SGD::Point self = GetPosition();
		self.x -= (float)Camera::x;
		self.y -= (float)Camera::y;
		self.x += 8;
		self.y += 8;

		force.x = pInput->GetMousePosition().x - self.x;
		force.y = pInput->GetMousePosition().y - self.y;
		if(force.x > 100)
			force.x = 100;
		if(force.x < -100)
			force.x = -100;
		if(force.y > 100)
			force.y = 100;
		if(force.y < -100)
			force.y = -100;

		CreateGrenadeMessage* grenade = new CreateGrenadeMessage(m_ptPosition.x, m_ptPosition.y, force);
		grenade->QueueMessage();

		m_fGrenadeTimer = 0.75f;
		m_pInventory->SetGrenades(m_pInventory->GetGrenades() - 1);
		StatTracker::GetInstance()->GrenadeThrown();
	}
#if !ARCADE_MODE
		m_bTHEBOOL = (pInput->IsKeyDown(SGD::Key::E) == true) || pInput->IsButtonPressed(0, (unsigned int)SGD::Button::X);
#endif
#if ARCADE_MODE
		m_bTHEBOOL = pInput->IsButtonPressed(0, 5) || pInput->IsButtonPressed(1, 5);
			        
#endif
	// Open Shop
	if(m_bTHEBOOL && m_pZombieWave->IsBuildMode() == true)
	{
		// IMPLEMENT ENTER SHOP HERE
	}
	// Turn walking audio off if you're not moving
	if ( pInput->IsKeyDown ( SGD::Key::W ) == false && pInput->IsKeyDown ( SGD::Key::A ) == false && pInput->IsKeyDown ( SGD::Key::S ) == false && 
		pInput->IsKeyDown ( SGD::Key::D ) == false && move == SGD::Vector { 0.0f , 0.0f })
	{
		pAudio->StopAudio(m_hWalking);
	}
	//GAH Weapons! - Arnold
#if !ARCADE_MODE
	//Switch to Machine Gun
	if ((pInput->IsKeyPressed(SGD::Key::One) == true || pInput->IsDPadPressed(0, SGD::DPad::Up)) && m_pZombieWave->IsBuildMode() == false && m_fSuperTimer <= 0.0f)
	{
		m_nCurrWeapon = MACHINE_GUN;
		m_pSprite = AnimationManager::GetInstance()->GetSprite("CarryAR");
		m_antsAnimation.m_fTimeOnFrame = 0;
		m_antsAnimation.m_nCurrFrame = 0;
		m_antsAnimation.m_nCurrAnimation = "CarryAR";
	}
	//Switch to Shotgun
	if (( pInput->IsKeyPressed(SGD::Key::Two) == true || pInput->IsDPadPressed(0, SGD::DPad::Right)) && m_pZombieWave->IsBuildMode() == false && m_fSuperTimer <= 0.0f)
	{
		m_nCurrWeapon = SHOT_GUN;
		//m_pSprite = AnimationManager::GetInstance()->GetSprite("player");
		//m_antsAnimation.m_fTimeOnFrame = 0;
		//m_antsAnimation.m_nCurrFrame = 0;
		//m_antsAnimation.m_nCurrAnimation = "player";
	}
	//Switch to Rocket Launcher
	if ((pInput->IsKeyPressed(SGD::Key::Three) == true || pInput->IsDPadPressed(0, SGD::DPad::Down)) && m_pZombieWave->IsBuildMode() == false && m_fSuperTimer <= 0.0f)
	{
		m_nCurrWeapon = ROCKET_LAUNCHER;
		m_pSprite = AnimationManager::GetInstance()->GetSprite("RocketCarry");
		m_antsAnimation.m_fTimeOnFrame = 0;
		m_antsAnimation.m_nCurrFrame = 0;
		m_antsAnimation.m_nCurrAnimation = "RocketCarry";
	}
	//Switch to Hat Trick Gun
	if ((pInput->IsKeyPressed(SGD::Key::Four) == true || pInput->IsDPadPressed(0, SGD::DPad::Left)) && m_pZombieWave->IsBuildMode() == false && m_fSuperTimer <= 0.0f)
	{
		m_nCurrWeapon = TRICK_SHOT_GUN;
		//m_pSprite = AnimationManager::GetInstance()->GetSprite("player");
		//m_antsAnimation.m_fTimeOnFrame = 0;
		//m_antsAnimation.m_nCurrFrame = 0;
		//m_antsAnimation.m_nCurrAnimation = "player";
	}
#endif

#if !ARCADE_MODE
	m_bTHEBOOL = pInput->IsKeyDown(SGD::Key::MouseLeft) || trg > 0.1f;
#endif
#if ARCADE_MODE
	m_bTHEBOOL = !(pInput->IsButtonDown(0, 0) || pInput->IsButtonDown(1, 0)) && shoot != SGD::Vector{ 0.0f, 0.0f };
#endif
	//Shoot
	if (m_pWeapons[m_nCurrWeapon].GetFireTimer() < 0 && m_pWeapons[m_nCurrWeapon].GetCurrAmmo() > 0 && m_pZombieWave->IsBuildMode() == false)
	{
		// Left click
		if (m_bTHEBOOL)
		{
			CreateProjectileMessage* msg = new CreateProjectileMessage ( m_nCurrWeapon );
			msg->QueueMessage ();
			msg = nullptr;
			//set the shot timer to the rate of fire
			int tempInt = m_pWeapons[ m_nCurrWeapon ].GetCurrAmmo ();
			m_pWeapons[m_nCurrWeapon].SetFireTimer(m_pWeapons[ m_nCurrWeapon ].GetFireRate ());
			
			// If you don't have the super buff
			if ( m_fSuperTimer <= 0.0f )
			{
				// If we have infinite ammo, don't subtract
				if (!pGame->HasInfAmmo())
					m_pWeapons[ m_nCurrWeapon ].SetCurrAmmo ( (m_pWeapons[ m_nCurrWeapon ].GetCurrAmmo () - 1) );
				m_pWeapons[ m_nCurrWeapon ].SetFireTimer ( m_pWeapons[ m_nCurrWeapon ].GetFireRate () );
			}
			else
			{
				m_pWeapons[ m_nCurrWeapon ].SetFireTimer ( m_pWeapons[ m_nCurrWeapon ].GetFireRate () / 2 );
			}
			StatTracker::GetInstance()->ShotsFired(m_nCurrWeapon);
		}
	}
	// Make the empty click noise
	else if (m_bTHEBOOL && m_pWeapons[m_nCurrWeapon].GetCurrAmmo() <= 0 && !GameplayState::GetInstance()->GetZombieFactory()->IsBuildMode())
	{
		// BUG - When the wave changes and you're out of ammo, the click is played
		// instead of the minecraft put block sound.
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hGunClick) == false)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hGunClick);
		}
	}
	
	// Cycle Selected Items
#if !ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed ( 0 , (unsigned int)SGD::Button::LB );
#endif
#if ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed(0,1) || pInput->IsButtonPressed(1,1);
#endif
	// Cycle Left
	if ( m_bTHEBOOL )
	{
		if ( m_pZombieWave->IsBuildMode () == true )
		{
			m_nCurrPlaceable--;
			if(m_nCurrPlaceable <= -1)
				m_nCurrPlaceable = NUMPLACEABLES - 1;
		}
		else if(m_fSuperTimer <= 0.0f)
		{
			m_nCurrWeapon--;
			if ( m_nCurrWeapon <= -1 )
				m_nCurrWeapon = TOTAL_GUNS - 1;
		}
	}
#if !ARCADE_MODE
	m_bTHEBOOL =  pInput->IsButtonPressed ( 0 , (unsigned int)SGD::Button::RB );
#endif
#if ARCADE_MODE
	m_bTHEBOOL = pInput->IsButtonPressed(0,2) || pInput->IsButtonPressed(1,2);
#endif

	// Cycle Right
	if ( m_bTHEBOOL)
	{
		if ( m_pZombieWave->IsBuildMode () == true )
		{
			m_nCurrPlaceable++;
			if(m_nCurrPlaceable >= NUMPLACEABLES)
				m_nCurrPlaceable = WALLS;
		}
		else if(m_fSuperTimer <= 0.0f)
		{
			m_nCurrWeapon++;
			if ( m_nCurrWeapon >= TOTAL_GUNS )
				m_nCurrWeapon = MACHINE_GUN;
		}
	}



#if !ARCADE_MODE

	// Selecting Walls
	if ( pInput->IsKeyPressed ( SGD::Key::One ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = WALLS;

	// Selecting Windows
	if ( pInput->IsKeyPressed ( SGD::Key::Two ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = WINDOWS;

	// Selecting Bear Trap
	if ( pInput->IsKeyPressed ( SGD::Key::Three ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = BEARTRAP;

	// Selecting Mine
	if ( pInput->IsKeyPressed ( SGD::Key::Four ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = MINE;

	// Selecting Machine gun towers
	if ( pInput->IsKeyPressed ( SGD::Key::Five ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = MGTOWER;

	// Selecting Maple Syrup towers
	if ( pInput->IsKeyPressed ( SGD::Key::Six ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = MSTOWER;

	// Selecting Hockey Stick towers
	if ( pInput->IsKeyPressed ( SGD::Key::Seven ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = HSTOWER;

	// Selecting Laser towers
	if ( pInput->IsKeyPressed ( SGD::Key::Eight ) == true && m_pZombieWave->IsBuildMode () == true )
		m_nCurrPlaceable = LTOWER;

	// Selecting lava trap
	if (pInput->IsKeyPressed(SGD::Key::Nine) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = LTRAP;

	// Selecting spike trap
	if (pInput->IsKeyPressed(SGD::Key::Zero) == true && m_pZombieWave->IsBuildMode() == true)
		m_nCurrPlaceable = STRAP;
#endif

	if ((pInput->IsKeyDown(SGD::Key::MouseRight) == true || trg > 0.1f) && m_pZombieWave->IsBuildMode() && Blockable(pos))	
	{
		// Test rect
		SGD::Rectangle rect;
		rect.left = pos.x * pWorld->GetTileWidth () + pWorld->GetTileWidth () / 4;
		rect.top = pos.y * pWorld->GetTileHeight () + pWorld->GetTileHeight () / 4;
		rect.right = rect.left + pWorld->GetTileWidth () / 2;
		rect.bottom = rect.top + pWorld->GetTileHeight () / 2;

		SetSelectedTower(nullptr);

		// Check if tower is there
		Tower* tower = dynamic_cast<Tower*>(m_pEntityManager->CheckCollision(rect, 3));

		if ( tower )
			SetSelectedTower(tower);

#if 1
		//Colliding with wall
		else
		{
			if (pWorld->GetColliderID((int)pos.x, (int)pos.y) == WALL)
			{
				pWorld->SetColliderID((int)pos.x, (int)pos.y, EMPTY);
				CreatePickupMessage*  pmsg = new CreatePickupMessage(ENT_PICKUP_WALL, { pos.x*GRIDWIDTH, pos.y * GRIDHEIGHT });
				pmsg->QueueMessage();
				pmsg = nullptr;
			}
			else if (pWorld->GetColliderID((int)pos.x, (int)pos.y) == WINDOW)
			{
				pWorld->SetColliderID((int)pos.x, (int)pos.y, EMPTY);
				CreatePickupMessage*  pmsg = new CreatePickupMessage(ENT_PICKUP_WINDOW, { pos.x*GRIDWIDTH, pos.y * GRIDHEIGHT });
				pmsg->QueueMessage();
				pmsg = nullptr;
			}
		}
#endif
	}

	if ( m_pZombieWave->IsBuildMode () == false )
	{
		//if (m_fShotTimer < 0)
		//{
		//	if (pInput->IsKeyDown(SGD::Key::MouseLeft) == true)
		//	{
		//		switch (m_nCurrWeapon)
		//		{
		//		case 0:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 1:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 2:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		case 3:
		//		{
		//			CreateProjectileMessage* msg = new CreateProjectileMessage(m_nCurrWeapon);
		//			msg->QueueMessage();
		//			msg = nullptr;
		//			//set the shot timer to the rate of fire
		//			m_fShotTimer = m_pWeapons[m_nCurrWeapon].GetFireRate();
		//		}
		//			break;
		//		}
		//	}
		//}
	}
	else
	{
		// Only place if cursor not on tower menu
		bool cursorInMenu = false;
		if (m_pSelectedTower)
		{
			SGD::Rectangle backgroundRect;
			backgroundRect.left = m_pSelectedTower->GetPosition().x - 96 - Camera::x;
			backgroundRect.top = m_pSelectedTower->GetPosition().y - 128 - Camera::y;
			backgroundRect.right = backgroundRect.left + 224;
			backgroundRect.bottom = backgroundRect.top + 128;

			if (pInput->GetMousePosition().IsWithinRectangle(backgroundRect))
			{
				cursorInMenu = true;
				m_bCanLeftClick = false;

				if (pInput->IsKeyPressed(SGD::Key::MouseLeft) || (trg < -0.1f && m_bCanLeftTrigger))
				{
					SGD::Point topLeft = SGD::Point(backgroundRect.left, backgroundRect.top);

					// Sell button
					SGD::Rectangle sellButton;
					sellButton.right = backgroundRect.right - 8;
					sellButton.bottom = backgroundRect.bottom - 8;
					sellButton.left = sellButton.right - 80;
					sellButton.top = sellButton.bottom - 32;

					// Upgrade one rect
					SGD::Rectangle upgradeOneRect;
					upgradeOneRect.left = topLeft.x + 8;
					upgradeOneRect.top = topLeft.y + 8;
					upgradeOneRect.right = topLeft.x + 108;
					upgradeOneRect.bottom = topLeft.y + 72;

					// Upgrade two rect
					SGD::Rectangle upgradeTwoRect;
					upgradeTwoRect.left = topLeft.x + 116;
					upgradeTwoRect.top = topLeft.y + 8;
					upgradeTwoRect.right = topLeft.x + 216;
					upgradeTwoRect.bottom = topLeft.y + 72;

					if (pInput->GetMousePosition().IsWithinRectangle(sellButton))
					{
						m_unScore += m_pSelectedTower->GetSellValue();
						StatTracker::GetInstance()->TowerExchange(false);
						DestroyEntityMessage* msg = new DestroyEntityMessage(m_pSelectedTower);
						msg->QueueMessage();

						// Empty world collider
						int wx = (int)(m_pSelectedTower->GetPosition().x / GRIDWIDTH);
						int wy = (int)(m_pSelectedTower->GetPosition().y / GRIDHEIGHT);

						pWorld->SetColliderID(wx, wy, EMPTY);

						m_pSelectedTower = nullptr;

						return;
					}

					else if (pInput->GetMousePosition().IsWithinRectangle(upgradeOneRect))
					{
						m_pSelectedTower->Upgrade(0, &m_unScore);
					}

					else if (pInput->GetMousePosition().IsWithinRectangle(upgradeTwoRect))
					{
						m_pSelectedTower->Upgrade(1, &m_unScore);
					}
				}
			}
		}

		// Place item
		if ( m_bCanLeftClick && !cursorInMenu && m_nCurrPlaceable != -1 &&  (pInput->IsKeyDown ( SGD::Key::MouseLeft ) == true || pInput->GetTrigger(0) < -0.1f) && m_fPlaceTimer <= 0 && 
			((PlacementCheck ( pos ) && m_nCurrPlaceable < 8) || (PlacementCheck( pos, true) && (m_nCurrPlaceable >= 8 || (m_nCurrPlaceable == 2 || m_nCurrPlaceable == 3))) ))
		{
			// Bear trap
			if ( m_nCurrPlaceable == BEARTRAP && m_pInventory->GetBearTraps () > 0 )
			{
				// Cooldown for placing objects
				SGD::Point p((pos.x * pWorld->GetTileWidth()), (pos.y * pWorld->GetTileHeight()));
				CreatePlaceableMessage* pmsg = new CreatePlaceableMessage ( p , m_nCurrPlaceable );
				pmsg->QueueMessage ();
				pmsg = nullptr;

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, false);

				// Decreasing the amount of bear traps left for the player
				unsigned int newset = m_pInventory->GetBearTraps ();
				--newset;
				m_pInventory->SetBearTraps ( newset );
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Mine
			else if ( m_nCurrPlaceable == MINE && m_pInventory->GetMines () > 0)
			{
				// Cooldown for placing objects
				SGD::Point p((pos.x * pWorld->GetTileWidth()), (pos.y * pWorld->GetTileHeight()));
				CreatePlaceableMessage* pmsg = new CreatePlaceableMessage ( p , m_nCurrPlaceable );
				pmsg->QueueMessage ();
				pmsg = nullptr;

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, false);

				// Decreasing the amount of mines left for the player
				unsigned int newset = m_pInventory->GetMines ();
				--newset;
				m_pInventory->SetMines ( newset );
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Walls
			else if ( m_nCurrPlaceable == WALLS && m_pInventory->GetWalls () > 0 )
			{
				pWorld->SetColliderID ( (int)pos.x , (int)pos.y , WALL );
				// Decreasing the amount of mines left for the player
				if (GameplayState::GetInstance()->GetGameMode() != 2)
				{
					unsigned int newset = m_pInventory->GetWalls();
					--newset;
					m_pInventory->SetWalls(newset);
				}
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				StatTracker::GetInstance()->WallExchange(true);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Windows
			else if ( m_nCurrPlaceable == WINDOWS && m_pInventory->GetWindows () > 0 )
			{
				pWorld->SetColliderID ( (int)pos.x , (int)pos.y , WINDOW );
				// Decreasing the amount of mines left for the player
				if (GameplayState::GetInstance()->GetGameMode() != 2)
				{
					unsigned int newset = m_pInventory->GetWindows();
					--newset;
					m_pInventory->SetWindows(newset);
				}
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				StatTracker::GetInstance()->WindowExchange(true);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", {pos.x*32,pos.y*32}, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Machine gun tower
			else if ( m_nCurrPlaceable == MGTOWER && m_pInventory->GetMachineGunTowers () > 0 )
			{
				CreateTowerMessage* pmsg = new CreateTowerMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
					CreateTowerMessage::TOWER_MACHINE_GUN);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, true);

				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetMachineGunTowers ( m_pInventory->GetMachineGunTowers () - 1 );
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Maple Syrup tower
			else if (m_nCurrPlaceable == MSTOWER && m_pInventory->GetMapleSyrupTowers() > 0)
			{
				CreateTowerMessage* pmsg = new CreateTowerMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
					CreateTowerMessage::TOWER_MAPLE_SYRUP);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, true);
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetMapleSyrupTowers(m_pInventory->GetMapleSyrupTowers() - 1);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Hockey Stick tower
			else if (m_nCurrPlaceable == HSTOWER && m_pInventory->GetHockeyStickTowers() > 0)
			{
				CreateTowerMessage* pmsg = new CreateTowerMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
						CreateTowerMessage::TOWER_HOCKEY_STICK);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, true);
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetHockeyStickTowers(m_pInventory->GetHockeyStickTowers() - 1);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}

			// Laser tower
			else if (m_nCurrPlaceable == LTOWER && m_pInventory->GetLaserTowers() > 0)
			{
				CreateTowerMessage* pmsg = new CreateTowerMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
						CreateTowerMessage::TOWER_LASER);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, true);
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetLaserTowers(m_pInventory->GetLaserTowers() - 1);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}
			else if ( m_nCurrPlaceable == LTRAP && m_pInventory->GetLavaTraps () > 0 )
			{
				CreateTrapMessage* pmsg = new CreateTrapMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
						CreateTrapMessage::TRAP_LAVA);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, false);
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetLavaTraps(m_pInventory->GetLavaTraps() - 1);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;

			}
			else if ( m_nCurrPlaceable == STRAP && m_pInventory->GetSpikeTraps () > 0 )
			{
				CreateTrapMessage* pmsg = new CreateTrapMessage((int)(pos.x * pWorld->GetTileWidth()), (int)(pos.y * pWorld->GetTileHeight()),
						CreateTrapMessage::TRAP_SPIKE);
				pmsg->QueueMessage();

				pWorld->SetSolidAtPosition((int)pos.x, (int)pos.y, false);
				if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBlockPlace) == false)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hBlockPlace);
				}
				// Decreasing the amount of machine gun towers left for the player
				m_pInventory->SetSpikeTraps(m_pInventory->GetSpikeTraps() - 1);
				CreateParticleMessage* msg = new CreateParticleMessage("Dust_Particle1", { pos.x * 32, pos.y * 32 }, 8, 8);
				msg->QueueMessage();
				msg = nullptr;
			}
		}

	}
	//If the camera is static
	if (m_bStaticCamera)
	{
		if (m_fCameraLerpTimer < 1)
		{
			m_fCameraLerpTimer += (dt*0.25f);
		}
		m_vCameraStart = { (float)Camera::x, (float)Camera::y };
		m_vCameraEnd = { m_ptPosition.x - 384, m_ptPosition.y - 284 };
		// Set camera
		SGD::Vector tempVector;
		tempVector = m_vCamera.Lerp(m_vCameraStart, m_vCameraEnd, m_fCameraLerpTimer);
		Camera::x = (int)tempVector.x;
		Camera::y = (int)tempVector.y;
	}
	else
	{
		//reset the lerp and set the next position
		m_fCameraLerpTimer = 0;
		m_vCameraStart = { (float)Camera::x, (float)Camera::y };
		m_vCameraEnd = { (float)(pInput->GetMousePosition().x + m_ptPosition.x - (384 * 2)), (float)(pInput->GetMousePosition().y + m_ptPosition.y - (284 * 2)) };
		SGD::Vector tempVector;
		tempVector = m_vCamera.Lerp(m_vCameraStart, m_vCameraEnd, 0.025f);
		Camera::x = (int)tempVector.x;
		Camera::y = (int)tempVector.y;
	}

	
}

void Player::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Draw selected tower's menu
	if (m_pSelectedTower)
	{
		// Menu bacground
		SGD::Rectangle backgroundRect;
		backgroundRect.left = m_pSelectedTower->GetPosition().x - 96 - Camera::x;
		backgroundRect.top = m_pSelectedTower->GetPosition().y - 128 - Camera::y;
		backgroundRect.right = backgroundRect.left + 224;
		backgroundRect.bottom = backgroundRect.top + 128;

		pGraphics->DrawRectangle(backgroundRect, SGD::Color(150, 0, 0, 0), SGD::Color(255, 255, 0), 2);

		m_pSelectedTower->DrawMenu();

		// Sell button
		SGD::Rectangle sellButton;
		sellButton.right = backgroundRect.right - 8;
		sellButton.bottom = backgroundRect.bottom - 8;
		sellButton.left = sellButton.right - 80;
		sellButton.top = sellButton.bottom - 32;

		if (pInput->GetMousePosition().IsWithinRectangle(sellButton))
			pGraphics->DrawRectangle(sellButton, SGD::Color(100, 0, 255, 0), SGD::Color(255, 255, 255), 2);

		else
			pGraphics->DrawRectangle(sellButton, SGD::Color(0, 0, 0, 0), SGD::Color(255, 255, 255), 2);

		string sell = "Sell for " + std::to_string(m_pSelectedTower->GetSellValue());
		pFont->Draw(sell, (int)sellButton.left + 2, (int)sellButton.top + 2, 0.3f, SGD::Color(255, 255, 255));

		// Ignore drawing item preview if cursor inside tower menu
		if (pInput->GetMousePosition().IsWithinRectangle(backgroundRect))
			return;
	}

	// Show the item in the location of the placement
	if (m_pZombieWave->IsBuildMode())
	{
		// Get the tile
		SGD::Point tilePos = SGD::InputManager::GetInstance()->GetMousePosition();
		tilePos.x = (float)((int)(tilePos.x + Camera::x) / GRIDWIDTH);
		tilePos.y = (float)((int)(tilePos.y + Camera::y) / GRIDHEIGHT);

		// Get tiles world position
		SGD::Point worldPosition = tilePos;
		worldPosition.x *= WorldManager::GetInstance()->GetTileWidth();
		worldPosition.y *= WorldManager::GetInstance()->GetTileHeight();
		worldPosition.x -= Camera::x;
		worldPosition.y -= Camera::y;

		// Check if legal placement
		bool legalPlacement;
		if(m_nCurrPlaceable < 8)
			legalPlacement = PlacementCheck(tilePos);
		else
			legalPlacement = PlacementCheck(tilePos, true);
		// Walls
		if (m_nCurrPlaceable == WALLS)
		{
			// Make sure we have walls
			if (m_pInventory->GetWalls() == 0)
				legalPlacement = false;

			// Draw wall image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 0, 32, 32));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 0, 64, 32));
		}

		// Windows
		else if (m_nCurrPlaceable == WINDOWS)
		{
			// Make sure we have windows
			if (m_pInventory->GetWindows() == 0)
				legalPlacement = false;

			// Draw window image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 32, 32, 64));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 32, 64, 64));
		}

		// Bear traps
		else if (m_nCurrPlaceable == BEARTRAP)
		{
			// Make sure we have bear traps
			if (m_pInventory->GetBearTraps() == 0)
				legalPlacement = false;

			// Draw bear trap image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 64, 32, 96));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 64, 64, 96));
		}

		// Mines
		else if (m_nCurrPlaceable == MINE)
		{
			// Make sure we have mines
			if (m_pInventory->GetMines() == 0)
				legalPlacement = false;

			// Draw mine image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 96, 32, 128));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 96, 64, 128));
		}

		// Machine gun towers
		else if (m_nCurrPlaceable == MGTOWER)
		{
			// Make sure we have machine gun towers
			if (m_pInventory->GetMachineGunTowers() == 0)
				legalPlacement = false;

			// Draw machine gun tower image
			if (legalPlacement)
			{
				float scale = DEFAULT_MACHINE_GUN_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(0, 0, 128, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(0, 128, 32, 192));
			}

			// Non-legal placement
			else
			{
				float scale = DEFAULT_MACHINE_GUN_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(128, 0, 256, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(32, 128, 64, 192));
			}
		}

		// Maple syrup towers
		else if (m_nCurrPlaceable == MSTOWER)
		{
			// Make sure we have maple syrup towers
			if (m_pInventory->GetMapleSyrupTowers() == 0)
				legalPlacement = false;

			// Draw maple syrup tower image
			if (legalPlacement)
			{
				float scale = DEFAULT_MAPLE_SYRUP_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(0, 0, 128, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(0, 192, 32, 256));
			}

			// Non-legal placement
			else
			{
				float scale = DEFAULT_MAPLE_SYRUP_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(128, 0, 256, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(32, 192, 64, 256));
			}
		}

		// Hockey stick towers
		else if (m_nCurrPlaceable == HSTOWER)
		{
			// Make sure we have hockey stick towers
			if (m_pInventory->GetHockeyStickTowers() == 0)
				legalPlacement = false;

			// Draw hockey stick tower image
			if (legalPlacement)
			{
				float scale = DEFAULT_HOCKEY_STICK_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(0, 0, 128, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(0, 256, 32, 320));
			}

			// Non-legal placement
			else
			{
				float scale = DEFAULT_HOCKEY_STICK_RANGE / 128.0f;

				pGraphics->DrawTextureSection(m_hRangeCirclesImage, SGD::Point(worldPosition.x + 16 - scale * 128.0f, worldPosition.y + 16 - scale * 128.0f), SGD::Rectangle(128, 0, 256, 128),
					0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));

				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, SGD::Point(worldPosition.x, worldPosition.y - 32), SGD::Rectangle(32, 256, 64, 320));
			}
		}

		// Laser tower
		if (m_nCurrPlaceable == LTOWER)
		{
			// Make sure we have laser towers
			if (m_pInventory->GetLaserTowers() == 0)
				legalPlacement = false;

			// Draw laser tower image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 320, 32, 352));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 320, 64, 352));
		}

		// Lava trap
		if (m_nCurrPlaceable == LTRAP)
		{
			// Make sure we have lava traps
			if (m_pInventory->GetLavaTraps() == 0)
				legalPlacement = false;

			// Draw lava trap image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 352, 32, 384));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 352, 64, 384));
		}

		// Spike trap
		if (m_nCurrPlaceable == STRAP)
		{
			// Make sure we have spike traps
			if (m_pInventory->GetSpikeTraps() == 0)
			legalPlacement = false;

			// Draw spike trap image
			if (legalPlacement)
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(0, 384, 32, 416));

			// Non-legal placement
			else
				pGraphics->DrawTextureSectionSimple(m_hPlaceablesImage, worldPosition, SGD::Rectangle(32, 384, 64, 416));
		}
	}

	// If we're next to the shop, show the popup
	if (m_bIsNearShop && m_pZombieWave->IsBuildMode())
		pFont->Draw("Press E to enter the shop", 180, 60, 0.8f, { 255, 255, 255 });


	// Debugging
	Game* pGame = Game::GetInstance();
	if (pGame->IsShowingPos())
	{
		string position = "Player Pos [";
		position += std::to_string(m_ptPosition.x);
		position += " , ";
		position += std::to_string(m_ptPosition.y);
		position += " ]";
		pGraphics->DrawString(position.c_str(), { 200, 60 }, { 255, 255, 255 });
	}
}

int Player::GetType () const
{
	return ENT_PLAYER;
}

void Player::HandleCollision ( const IEntity* pOther )
{

	//if ( pOther->GetType () == ENT_ZOMBIE_BEAVER )
	//{
	//	m_nCurrHealth--;
	//}
	if ( pOther->GetType () == ENT_PICKUP_WALL )
	{
		unsigned int newset = m_pInventory->GetWalls ();
		++newset;
		m_pInventory->SetWalls ( newset );

	}
	if ( pOther->GetType () == ENT_PICKUP_WINDOW )
	{
		unsigned int newset = m_pInventory->GetWindows ();
		++newset;
		m_pInventory->SetWindows ( newset );
	}
	if( pOther->GetType() == ENT_PICKUP_AMMO )
	{
		// Still need to tweak the amount of ammo given
		(m_bHasAssaultRifle) ? m_pWeapons[0].SetCurrAmmo(m_pWeapons[0].GetCurrAmmo() + 50) : __noop;
		(m_bHasShotty) ? m_pWeapons[1].SetCurrAmmo(m_pWeapons[1].GetCurrAmmo() + 30) : __noop;
		(m_bHasRocketz) ? m_pWeapons[2].SetCurrAmmo(m_pWeapons[2].GetCurrAmmo() + 5) : __noop;
		(m_bHasHatTrick) ? m_pWeapons[3].SetCurrAmmo(m_pWeapons[3].GetCurrAmmo() + 50) : __noop;
		m_sPickupMessage = "More Bullets Eh?";
		m_fPickupMessageTimer = 1.5f;
		GameplayState::GetInstance()->PlayAmmoPickup();
	}
	if ( pOther->GetType () == ENT_PICKUP_HEALTHPACK )
	{
		int curr = m_pInventory->GetHealthPacks();
		m_pInventory->SetHealthPacks ( curr + 1 );
		m_sPickupMessage = "More Bandaids Eh?";
		m_fPickupMessageTimer = 1.5f;
	}
	if ( pOther->GetType() == ENT_PICKUP_SUPER )
	{
		m_fSuperTimer = m_fSuperLength;
		m_sPickupMessage = "Super Canadian!";
		m_fPickupMessageTimer = 1.5f;
	}

	// If we are touching the shop
	if (pOther->GetType() == ENT_SHOP)
		m_bIsNearShop = true;
}

void Player::HandleEvent ( const SGD::Event* pEvent )
{
	if ( pEvent->GetEventID () == "TAKE_DAMAGE" )
	{
		float damage = *((float*)pEvent->GetData ());
		m_nCurrHealth -= damage;
		CreateParticleMessage* msg = new CreateParticleMessage("Blood_Particle1", this, 8, 8);
		msg->QueueMessage();
		msg = nullptr;
		// Make sure we don't underflow
		if ( m_nCurrHealth < 0.0f )
			m_nCurrHealth = 0.0f;
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hPlayerGrunt) == false)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hPlayerGrunt);
		}
	}

	if ( pEvent->GetEventID () == "INCREASE_SCORE" )
	{
		int score = *((int*)pEvent->GetData ());
		m_unScore += score;
	}
}

bool Player::Blockable ( SGD::Point mouse )
{
	return (mouse.x >= 1 && mouse.x < WorldManager::GetInstance ()->GetWorldWidth () - 1
		&& mouse.y >= 1 && mouse.y < WorldManager::GetInstance ()->GetWorldHeight () - 1);
}

/**********************************************************/
// Accessors

float Player::GetMaxHealth () const
{
	return m_nMaxHealth;
}

float Player::GetCurrHealth () const
{
	return m_nCurrHealth;
}

int Player::GetCurrWeapon () const
{
	return m_nCurrWeapon;
}

int Player::GetCurrPowerup () const
{
	return m_nCurrPowerup;
}

int Player::GetCurrPlaceable () const
{
	return m_nCurrPlaceable;
}

unsigned int Player::GetScore () const
{
	return m_unScore;
}

unsigned int Player::GetEnemiesKilled () const
{
	return m_unEnemiesKilled;
}

float Player::GetSpeed () const
{
	return m_fSpeed;
}

float Player::GetScoreMultiplier () const
{
	return m_fScoreMultiplier;
}

float Player::GetTimeAlive () const
{
	return m_fTimeAlive;
}

float Player::GetSuperTimer() const
{
	return m_fSuperTimer;
}

float Player::GetSuperLength() const
{
	return m_fSuperLength;
}

Inventory* Player::GetInventory () const
{
	return m_pInventory;
}


Weapon* Player::GetWeapons () const
{
	return m_pWeapons;
}

/**********************************************************/
// Mutators

void Player::SetMaxHealth ( float _maxHealth )
{
	m_nMaxHealth = _maxHealth;
}

void Player::SetCurrHealth ( float _currHealth )
{
	m_nCurrHealth = _currHealth;
}

void Player::SetCurrWeapon ( int _currWeapon )
{
	m_nCurrWeapon = _currWeapon;
}

void Player::SetCurrPowerup ( int _currPowerup )
{
	m_nCurrPowerup = _currPowerup;
}

void Player::SetCurrPlaceable ( int _currPlaceable )
{
	m_nCurrPlaceable = _currPlaceable;
}

void Player::SetScore ( unsigned int _score )
{
	m_unScore = _score;
}

void Player::SetEnemiesKilled ( unsigned int _enemiesKilled )
{
	m_unEnemiesKilled = _enemiesKilled;
}

void Player::SetSpeed ( float _speed )
{
	m_fSpeed = _speed;
}

void Player::SetScoreMultiplier ( float _multiplier )
{
	m_fScoreMultiplier = _multiplier;
}

void Player::SetTimeAlive ( float _timeAlive )
{
	m_fTimeAlive = _timeAlive;
}

void Player::SetInventory ( Inventory* _inventory )
{
	m_pInventory = _inventory;
}

void Player::SetWeapons ( Weapon* _weapons )
{
	m_pWeapons = _weapons;
}

void Player::SetSelectedTower(Tower* _tower)
{
	if (m_pSelectedTower)
		m_pSelectedTower->SetSelected(false);

	m_pSelectedTower = nullptr;
	if (_tower)
	{
		m_pSelectedTower = _tower;
		m_pSelectedTower->SetSelected(true);
	}
}

void Player::SetPlaceablesImage(SGD::HTexture _placeablesImage)
{
	m_hPlaceablesImage = _placeablesImage;
}

void Player::SetRangeCirclesImage(SGD::HTexture _rangeCirclesImage)
{
	m_hRangeCirclesImage = _rangeCirclesImage;
}

void Player::SetSuperTimer(float timer)
{
	m_fSuperTimer = timer;
}

void Player::SetSuperLength(float timer)
{
	m_fSuperLength = timer;
}

bool Player::CheckLegalPlacement(Node end, Node block)
{
	WorldManager* pWorld = WorldManager::GetInstance ();

	Node start;
	start.x = 0;
	start.y = 0;

	// Reset node chart
	for ( int x = 0; x < pWorld->GetWorldWidth (); x++ )
	{
		for ( int y = 0; y < pWorld->GetWorldHeight (); y++ )
		{
			if ( pWorld->IsSolidAtPosition ( x , y ) )
				m_nNodeChart[ x ][ y ] = -1;
			else
				m_nNodeChart[ x ][ y ] = 0;
		}
	}

	queue<Node> nodes;

	m_nNodeChart[ block.x ][ block.y ] = -1;

	nodes.push ( end );
	m_nNodeChart[ end.x ][ end.y ] = 1;

	int highestNode = 1;

	while ( !nodes.empty () )
	{
		Node node;
		node = nodes.front ();
		nodes.pop ();

		if ( node.x == start.x && node.y == start.y )
			return true;

		/*if (node.x < 1 || node.y < 1 || node.x >= m_nWorldWidth - 1 || node.y >= m_nWorldHeight - 1)
		continue;*/

		if ( node.x > 0 && m_nNodeChart[ node.x - 1 ][ node.y ] == 0 )
		{
			nodes.push ( Node ( node.x - 1 , node.y ) );
			m_nNodeChart[ node.x - 1 ][ node.y ] = m_nNodeChart[ node.x ][ node.y ] + 1;
		}

		if ( node.x < pWorld->GetWorldWidth () - 1 && m_nNodeChart[ node.x + 1 ][ node.y ] == 0 )
		{
			nodes.push ( Node ( node.x + 1 , node.y ) );
			m_nNodeChart[ node.x + 1 ][ node.y ] = m_nNodeChart[ node.x ][ node.y ] + 1;
		}

		if ( node.y < pWorld->GetWorldHeight () - 1 && m_nNodeChart[ node.x ][ node.y + 1 ] == 0 )
		{
			nodes.push ( Node ( node.x , node.y + 1 ) );
			m_nNodeChart[ node.x ][ node.y + 1 ] = m_nNodeChart[ node.x ][ node.y ] + 1;
		}

		if ( node.y > 0 && m_nNodeChart[ node.x ][ node.y - 1 ] == 0 )
		{
			nodes.push ( Node ( node.x , node.y - 1 ) );
			m_nNodeChart[ node.x ][ node.y - 1 ] = m_nNodeChart[ node.x ][ node.y ] + 1;
		}
	}

	return false;
}

bool Player::PlacementCheck ( SGD::Point mouse , bool isPassable)
{
	WorldManager* pWorld = WorldManager::GetInstance();

	if (mouse.x < 0 || mouse.y < 0 || (int)mouse.x >= pWorld->GetWorldWidth() || (int)mouse.y >= pWorld->GetWorldHeight())
		return false;

	bool a = Blockable(mouse);
	bool b = WorldManager::GetInstance()->IsSolidAtPosition((int)mouse.x, (int)mouse.y) == false;
	bool c = m_pEntityManager->CheckCollision({ mouse.x * GRIDWIDTH, mouse.y * GRIDHEIGHT, mouse.x * GRIDWIDTH + GRIDWIDTH, mouse.y * GRIDHEIGHT + GRIDHEIGHT }) == false;
	bool d;
	if(isPassable == false)
		d = CheckLegalPlacement(Node((int)(m_ptPosition.x + 16) / GRIDWIDTH, (int)(m_ptPosition.y + 16) / GRIDHEIGHT), Node((int)mouse.x, (int)mouse.y));
	else
		d = true;

	if (a
		&& b
		&& c
		&& d)
	{
		return true;
	}
	else
		return false;
}

void Player::Render ( void )
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance ();

	// Get the current frame
	int frame = GetSprite()->GetCurrFrame();

	// Find the center of the image
	SGD::Vector center;
	center.x = GetSprite()->GetFrame(frame).GetFrameRect().right - GetSprite()->GetFrame(frame).GetFrameRect().left;
	center.y = GetSprite()->GetFrame(frame).GetFrameRect().bottom - GetSprite()->GetFrame(frame).GetFrameRect().top;
	center.x /= 2;
	center.y /= 2;
	
	// Calculate the rotation
	SGD::Point pos = SGD::InputManager::GetInstance()->GetMousePosition();
	pos.x -= 16;
	pos.y -= 16;
	SGD::Point playerPos = GetPosition();
	playerPos.x -= Camera::x;
	playerPos.y -= Camera::y;
	SGD::Vector dir = pos - playerPos;
	dir.Normalize();
	float rot = SGD::Vector ( 0.0f , -1.0f ).ComputeSteering(dir);
	float rotation = 0;
	if(rot > 0)
		rotation = SGD::Vector(0.0f, -1.0f).ComputeAngle(dir);
	else
		rotation = -SGD::Vector(0.0f, -1.0f).ComputeAngle(dir);
	SGD::Color col = SGD::Color(255, 255 ,255, 255);
	// Testing purposes but also will do something like this with actual art
	if(m_fSuperTimer > 0 && rand() % 2 == 0)
		col = SGD::Color(255, 255, 0, 0);

	AnimationManager::GetInstance ()->Render ( m_antsAnimation , m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y , rotation, center, col );

	if(m_fPickupMessageTimer > 0)
		SGD::GraphicsManager::GetInstance()->DrawString(m_sPickupMessage.c_str(), SGD::Point(m_ptPosition.x - Camera::x - 60, m_ptPosition.y - 20 - Camera::y), SGD::Color(255, 0, 0));

	SGD::Rectangle drawRect = GetRect ();
	drawRect.left -= Camera::x;
	drawRect.right -= Camera::x;
	drawRect.top -= Camera::y;
	drawRect.bottom -= Camera::y; 
	

	//SGD::GraphicsManager* g = SGD::GraphicsManager::GetInstance();
	//g->DrawLine({ m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y }, { (m_ptPosition.x - Camera::x) + 1, (m_ptPosition.y - Camera::y) + 1 });

	

	// -- Debugging Mode --
	Game* pGame = Game::GetInstance();
	if (pGame->IsShowingRects())
		pGraphics->DrawRectangle(drawRect, { 128, 255, 255, 0 });

	
}

bool Player::IsRunningMan( void ) const
{
	return isRunningMan;
}

void Player::SetRunningMan( bool yes)
{
	isRunningMan = yes;
}
