/***************************************************************
|	File:		GameplayState.h
|	Author:		
|	Course:		
|	Purpose:	
***************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H


#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Listener.h"

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
#include "../SGD Wrappers/SGD_Declarations.h"

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
	static GameplayState* GetInstance( void );

	
	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Input	( void )				override;	// handle user input
	virtual void	Update	( float elapsedTime )	override;	// update game entities / animations
	virtual void	Render	( void )				override;	// render game entities / menus
	
private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;
	
	
	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents			= nullptr;
	SGD::MessageManager*	m_pMessages			= nullptr;
	ParticleManager*		m_pParticleManager	= nullptr;

	/**********************************************************/
	// Game Data
	Shop* m_pShop;
	bool m_bIsPaused;
	Player* m_pPlayer;
	unsigned int m_unWave;
	unsigned int m_unEnemiesRemaining;
	float m_fTimeUntilWave;
	ZombieFactory zombieFactory;

	/**********************************************************/
	// Textures
	SGD::HTexture m_hPlayerImg = SGD::INVALID_HANDLE;

	SGD::HTexture m_hPauseMainBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPauseOptionsBackground = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Pause Menu Stuff
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	int m_nPauseMenuCursor;
	int m_nPauseMenuTab;
	enum PauseMenuOption { PAUSE_RESUME, PAUSE_OPTION, PAUSE_EXIT };
	enum PauseMenuTab { TAB_MAIN, TAB_OPTION, TAB_EXIT };
	enum PauseMenuOptionsOption { OPTION_MUSIC, OPTION_SFX, OPTION_GOBACK };


	/**********************************************************/
	// 
	SGD::HAudio m_hBackgroundMus = SGD::INVALID_HANDLE;

	
	/**********************************************************/
	// Factory Methods

	// Create a button
	Button* CreateButton() const;

	// Creates a player
	Player*	CreatePlayer() const;

	// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );
};

#endif //GAMEPLAYSTATE_H
