/***************************************************************
|	File:		IntroState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will play an animation or video that
|				attracts the player during idling OR when the game
|				is first run.
***************************************************************/


#ifndef INTROSTATE_H
#define INTROSTATE_H


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
// IntroState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class IntroState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static IntroState* GetInstance( void );

	
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
	IntroState( void )			= default;	// default constructor
	virtual ~IntroState( void )	= default;	// destructor

	IntroState( const IntroState& )				= delete;	// copy constructor
	IntroState& operator= ( const IntroState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;
	Entity* m_pPlayer = nullptr;
	
	
	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents			= nullptr;
	SGD::MessageManager*	m_pMessages			= nullptr;


	MenuFlyweight* m_pMenuFlyweight;
	float m_fTimeRemaining;


	/**********************************************************/
	// Textures


	/**********************************************************/
	// Audio

	
	/**********************************************************/
	// Factory Methods


	// Message Callback Function:
	//static void MessageProc( const SGD::Message* pMsg );
};

#endif //INTROSTATE_H
