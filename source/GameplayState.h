/***************************************************************
|	File:		GameplayState.h
|	Author:		Justin Mazzola & Justin Patterson & Matthew Salow &
|				Ryan Simmons & James Sylvester
|	Course:		SGP
|	Purpose:	This state is the game. Like the whole game.
***************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "AnimationManager.h"
#include "Drone.h"

/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class Shop;
class Player;
class ParticleManager;
class Button;
class BitmapFont;

class StatTracker;
class TowerFlyweight;
class RZBN;

#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Geometry.h"

#include "ZombieFactory.h"



/**************************************************************/
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static GameplayState* GetInstance(void);

	enum GameModes { ARCADE_GAME_MODE = 0, HARDCORE_MODE = 1, SANDBOX_MODE = 2, BEAVER_FEAVER_MODE = 3, RUNNING_MAN_MODE = 4 };

	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)				override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update game entities / animations
	virtual void	Render(void)				override;	// render game entities / menus

	EntityManager* GetEntityManager() const;
	ZombieFactory* GetZombieFactory() const;
	char GetCurrentGameSlot() const;
	void SetCurrentGameSlot(char slot);
	int GetGameMode() const;
	void SetGameMode(int gameMode);

	Shop* GetShop() const { return m_pShop; }

	bool m_bPlayLaser;

	// Create and or save the game
	void SaveGame();
	void LoadGameFromSlot(int slot);

	// Player calls this once their hp is <= 0 to lose the game
	void HasLost(void);

	void PlayAmmoPickup(void);

	bool IsPaused() { return m_bIsPaused; }
	void SetPaused(bool pause) { m_bIsPaused = pause; }
	void MouseWheel(int direction);

	void SetLoadingBar(float percent, const char* message);
	int droneCount = 0;

private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	GameplayState(void) = default;	// default constructor
	virtual ~GameplayState(void) = default;	// destructor

	GameplayState(const GameplayState&) = delete;	// copy constructor
	GameplayState& operator= (const GameplayState&) = delete;	// assignment operator


	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities = nullptr;


	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents = nullptr;
	SGD::MessageManager*	m_pMessages = nullptr;
	ParticleManager*		m_pParticleManager = nullptr;

	/**********************************************************/
	// Game Data
	Shop* m_pShop;
	bool m_bIsPaused;
	Player* m_pPlayer;
	Entity* m_pPuppet;
	unsigned int m_unWave;
	unsigned int m_unEnemiesRemaining;
	float m_fTimeUntilWave;
	ZombieFactory* zombieFactory;
	SGD::Point m_ptPlayerSpawnPoint;
	StatTracker* m_pStatTracker;
	char m_nCurrGameSlot;
	int m_nGamemode;
	std::vector<Drone*> drones;
	/**********************************************************/
	// FPS
	// FPS stuff
	unsigned int m_unFPS = 60;
	unsigned int m_unFrames = 0;
	float m_fFPSTimer = 1.0f;

	

	/**********************************************************/
	// Textures
	SGD::HTexture m_hPlayerImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBeaverImg = SGD::INVALID_HANDLE;

	SGD::HTexture m_hPauseMainBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPauseOptionsBackground = SGD::INVALID_HANDLE;

	SGD::HTexture m_hHUD = SGD::INVALID_HANDLE;

	SGD::HTexture m_hShotgunPic = SGD::INVALID_HANDLE;
	SGD::HTexture m_hShotgunThumb = SGD::INVALID_HANDLE;

	SGD::HTexture m_hARPic = SGD::INVALID_HANDLE;
	SGD::HTexture m_hARThumb = SGD::INVALID_HANDLE;

	SGD::HTexture m_hRLPic = SGD::INVALID_HANDLE;
	SGD::HTexture m_hRLThumb = SGD::INVALID_HANDLE;

	SGD::HTexture m_hFireAxePic = SGD::INVALID_HANDLE;
	SGD::HTexture m_hFireAxeThumb = SGD::INVALID_HANDLE;

	// Tower textures
	SGD::HTexture m_hMachineGunBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMachineGunGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMachineGunBulletImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMapleSyrupBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMapleSyrupGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMapleSyrupBulletImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHockeyStickBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHockeyStickGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLaserBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLaserGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlaceablesImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hRangeCirclesImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBuildModeHUD = SGD::INVALID_HANDLE;
	SGD::HTexture m_hUnderBackground = SGD::INVALID_HANDLE;

	// Trap textures
	SGD::HTexture m_hSpikeTrapBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSpikeTrapSpikeImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLavaTrapBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLavaTrapFlameImage = SGD::INVALID_HANDLE;

	SGD::HTexture m_hExplosionImage = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Pause Menu Stuff
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	int m_nPauseMenuCursor;
	int m_nPauseMenuTab;
	enum PauseMenuOption { PAUSE_RESUME, PAUSE_OPTION, PAUSE_EXIT, PAUSE_TOTAL };
	enum PauseMenuTab { TAB_MAIN, TAB_OPTION, TAB_EXIT };
	enum PauseMenuOptionsOption { OPTION_MUSIC, OPTION_SFX, OPTION_FULLSCREEN, OPTION_GOBACK, OPTION_TOTAL };

	SGD::Point m_ptPausePositions[PAUSE_TOTAL];
	SGD::Point m_ptOptionPositions[OPTION_TOTAL];

	/**********************************************************/
	// Audio
	SGD::HAudio m_hBackgroundMus	= SGD::INVALID_HANDLE;
	SGD::HAudio m_hPistol			= SGD::INVALID_HANDLE;
	SGD::HAudio m_hShopMusic		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hGunShoot			= SGD::INVALID_HANDLE;
	SGD::HAudio m_hRocketShoot		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hShotgunShoot		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hBulletHit		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hBulletImpact		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hPurchase			= SGD::INVALID_HANDLE;
	SGD::HAudio m_hExplosion		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hClickSound		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hChaChing			= SGD::INVALID_HANDLE;
	SGD::HAudio m_hBeaverFever		= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hGoToShop			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hGoodJob			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hSoory1			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hSoory2			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hTrueHero			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hUpgrade1			= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hWelcomeShop		= SGD::INVALID_HANDLE;
	SGD::HAudio	m_hWinTheGame		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hAmmoPickup		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hBeaverFeverMusic = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSandboxMusic		= SGD::INVALID_HANDLE;
	SGD::HAudio m_hRunningManMusic	= SGD::INVALID_HANDLE;
	SGD::HAudio m_hLaserSound = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Factory Methods
	Entity* CreateBeaverZombie(int _x, int _y) const;
	Entity* CreateFastZombie(int _x, int _y) const;
	Entity* CreateSlowZombie(int _x, int _y) const;
	Entity* CreateProjectile(int _Weapon) const;
	Entity* CreatePlaceable(SGD::Point pos, int trap) const;
	Entity* CreatePickUp(int pick, SGD::Point pos) const;
	Entity* CreateTower(int x, int y, int type) const;
	Entity* CreateTrap(int x, int y, int type) const;
	Entity* CreateMachineGunBullet(int x, int y, SGD::Vector velocity, int damage) const;
	Entity* CreateMapleSyrupBullet(int x, int y, SGD::Vector velocity, float slowTime) const;
	Entity* CreateDrone() const;
	Entity* CreateGrenade(float x, float y, SGD::Vector velocity) const;
	Entity* CreateShop(float x, float y) const;
	Entity* CreateExplosion(float x, float y, float damage, float radius) const;

	// Create a button
	Button* CreateButton() const;

	// Creates a player
	Player*	CreatePlayer(string playerStatsFileName) const;

	// Message Callback Function:
	static void MessageProc(const SGD::Message* pMsg);

	// Animation Manager
	AnimationManager* m_pAnimation = nullptr;

	// Winning Functions and Variables
	void RenderCredits(void);
	bool m_bCreditsStarted;
	int m_nTopMargin;
	int m_nBottomMargin;
	float m_fCreditsTimer;
	float m_fWinTimer;
	SGD::Point m_ptTextPosition;
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;

	// Losing Functions and Variables
	void RenderLoss(void);
	float m_fLossTimer;
	bool m_bHasLost;
	bool m_bReplay;
	bool m_bEnterShop = false;
	bool m_bBuildStart = false;

	// RZBN File Format
	RZBN* rzbn;

	// Enemy recipes
	void LoadEnemyRecipes(string fileName);
	float m_fHealthScaling;
	float m_fSlowHealth;
	float m_fSlowSpeed;
	float m_fSlowRegeneration;
	float m_fSlowHealthChance;
	float m_fSlowAmmoChance;
	float m_fSlowSuperChance;
	float m_fFastHealth;
	float m_fFastSpeed;
	float m_fFastRegeneration;
	float m_fFastHealthChance;
	float m_fFastAmmoChance;
	float m_fFastSuperChance;
	float m_fBeaverHealth;
	float m_fBeaverSpeed;
	float m_fBeaverRegeneration;
	float m_fBeaverHealthChance;
	float m_fBeaverAmmoChance;
	float m_fBeaverSuperChance;

	// Tower recipes
	TowerFlyweight* m_pTowerFlyweight;

	// ARCADEMODE STUFF
	bool m_bTHEBOOL;
	bool m_bAccept;
	SGD::Vector m_vtStick;

	
};

#endif //GAMEPLAYSTATE_H
