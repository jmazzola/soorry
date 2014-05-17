/***************************************************************
|	File:		MainMenuState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will be the main menu of the game,
|				players will be shown a menu to allow for them to
|				enter the options, load/save, or leave the game.
***************************************************************/


#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H


#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio


/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class MenuFlyweight;

#include "../SGD Wrappers/SGD_Declarations.h"



/**************************************************************/
// MainMenuState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class MainMenuState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static MainMenuState* GetInstance( void );

	
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
	MainMenuState( void )			= default;	// default constructor
	virtual ~MainMenuState( void )	= default;	// destructor

	MainMenuState( const MainMenuState& )				= delete;	// copy constructor
	MainMenuState& operator= ( const MainMenuState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;
	Entity* m_pPlayer = nullptr;
	
	
	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents			= nullptr;
	SGD::MessageManager*	m_pMessages			= nullptr;


	MenuFlyweight* m_pMenuFlyweight;
	float m_fAttractionTimer;


	/**********************************************************/
	// Textures


	/**********************************************************/
	// Audio

	
	/**********************************************************/
	// Factory Methods


	// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );
};

#endif //MAINMENUSTATE_H
