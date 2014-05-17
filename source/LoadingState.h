/***************************************************************
|	File:		LoadingState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	The state where the player will be shown a
|				"Loading.." screen to ensure the game hasnt frozen,
|				and have their data loaded to start gameplay
***************************************************************/

#pragma once

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
// LoadingState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class LoadingState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static LoadingState* GetInstance(void);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)				override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update game entities / animations
	virtual void	Render(void)				override;	// render game entities / menus

private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	LoadingState(void) = default;	// default constructor
	virtual ~LoadingState(void) = default;	// destructor

	LoadingState(const LoadingState&) = delete;	// copy constructor
	LoadingState& operator= (const LoadingState&) = delete;	// assignment operator


	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities = nullptr;
	Entity* m_pPlayer = nullptr;


	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents = nullptr;
	SGD::MessageManager*	m_pMessages = nullptr;


	MenuFlyweight* m_pMenuFlyweight;
	float m_fTimeRemaining;


	/**********************************************************/
	// Textures


	/**********************************************************/
	// Audio


	/**********************************************************/
	// Factory Methods


	// Message Callback Function:
	static void MessageProc(const SGD::Message* pMsg);
};