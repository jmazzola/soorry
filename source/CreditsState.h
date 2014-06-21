/***************************************************************
|	File:		CreditsState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will show the player the credits
|				of this game.
***************************************************************/

#pragma once

#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"


/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class MenuFlyweight;
class Button;
class BitmapFont;

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


	MenuFlyweight* m_pMenuFlyweight;


	/**********************************************************/
	// Textures
	SGD::HTexture m_hBackground;
	SGD::HTexture m_hMainMenuSnap;

	/**********************************************************/
	// Audio


	
	/**********************************************************/
	// Factory Methods
	Button* CreateButton() const;

	/**********************************************************/
	// Menu Items
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	/**********************************************************/
	// Credits Items
	SGD::Point textPosition;
	int topMargin;
	int bottomMargin;

	bool m_bAccept;
	SGD::Vector m_vtStick;

	// Message Callback Function:
	//static void MessageProc( const SGD::Message* pMsg );
};