/***************************************************************
|	File:		CreditsState.h
|	Author:		
|	Course:		
|	Purpose:	
***************************************************************/

#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H


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
// CreditsState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class CreditsState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static CreditsState* GetInstance( void );

	
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
	CreditsState( void )			= default;	// default constructor
	virtual ~CreditsState( void )	= default;	// destructor

	CreditsState( const CreditsState& )				= delete;	// copy constructor
	CreditsState& operator= ( const CreditsState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;
	Entity* m_pPlayer = nullptr;
	
	
	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents			= nullptr;
	SGD::MessageManager*	m_pMessages			= nullptr;


	MenuFlyweight* m_pMenuFlyweight;


	/**********************************************************/
	// Textures


	/**********************************************************/
	// Audio

	
	/**********************************************************/
	// Factory Methods


	// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );
};

#endif //CREDITSSTATE_H
