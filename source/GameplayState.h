/***************************************************************
|	File:		GameplayState.h
|	Author:		Justin Mazzola & Justin Patterson & Matthew Salow & James Sylvester
|	Course:		SGP
|	Purpose:	This state is the game. Like the whole game.
***************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H


#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Listener.h"
#include "AnimationManager.h"

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
class Drone;
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

	// Create and or save the game
	void SaveGame(bool newFile);
	void LoadGameFromSlot(int slot);

	// Player calls this once their hp is <= 0 to lose the game
	void HasLost(void);

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
	Entity* m_pPlayer;
	Entity* m_pPuppet;
	unsigned int m_unWave;
	unsigned int m_unEnemiesRemaining;
	float m_fTimeUntilWave;
	ZombieFactory* zombieFactory;
	SGD::Point m_ptPlayerSpawnPoint;

	char m_nCurrGameSlot;

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
	SGD::HTexture m_hHockeyStickBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHockeyStickGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLaserBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLaserGunImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlaceablesImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hRangeCirclesImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBuildModeHUD = SGD::INVALID_HANDLE;

	// Trap textures
	SGD::HTexture m_hSpikeTrapBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSpikeTrapSpikeImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLavaTrapBaseImage = SGD::INVALID_HANDLE;
	SGD::HTexture m_hLavaTrapFlameImage = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Pause Menu Stuff
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	int m_nPauseMenuCursor;
	int m_nPauseMenuTab;
	enum PauseMenuOption { PAUSE_RESUME, PAUSE_OPTION, PAUSE_EXIT };
	enum PauseMenuTab { TAB_MAIN, TAB_OPTION, TAB_EXIT };
	enum PauseMenuOptionsOption { OPTION_MUSIC, OPTION_SFX, OPTION_FULLSCREEN, OPTION_GOBACK };

	/**********************************************************/
	// Audio
	SGD::HAudio m_hBackgroundMus = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPistol = SGD::INVALID_HANDLE;
	SGD::HAudio m_hShopMusic = SGD::INVALID_HANDLE;
	SGD::HAudio m_hGunShoot = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRocketShoot = SGD::INVALID_HANDLE;
	SGD::HAudio m_hShotgunShoot = SGD::INVALID_HANDLE;
	SGD::HAudio m_hBulletHit = SGD::INVALID_HANDLE;
	/**********************************************************/
	// Factory Methods
	Entity* CreateBeaverZombie(int _x, int _y) const;
	Entity* CreateFastZombie(int _x, int _y) const;
	Entity* CreateSlowZombie(int _x, int _y) const;
	Entity* CreateProjectile(int _Weapon) const;
	Entity* CreatePlaceable(int trap) const;
	Entity* CreatePickUp(int pick, SGD::Point pos) const;
	Entity* CreateTower(int x, int y, int type) const;
	Entity* CreateTrap(int x, int y, int type) const;
	Entity* CreateMachineGunBullet(int x, int y, SGD::Vector velocity, int damage) const;
	Entity* CreateDrone() const;

	// Create a button
	Button* CreateButton() const;

	// Creates a player
	Entity*	CreatePlayer() const;

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
};

#endif //GAMEPLAYSTATE_H
