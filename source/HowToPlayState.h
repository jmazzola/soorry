/***************************************************************
|	File:		HowToPlayState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will show the player how to play with a slideshow
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

class HowToPlayState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static HowToPlayState* GetInstance(void);


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
	HowToPlayState(void) = default;	// default constructor
	virtual ~HowToPlayState(void) = default;	// destructor

	HowToPlayState(const HowToPlayState&) = delete;	// copy constructor
	HowToPlayState& operator= (const HowToPlayState&) = delete;	// assignment operator


	MenuFlyweight* m_pMenuFlyweight;

	/**********************************************************/
	// Cursor Index
	int	m_nTab;

	/**********************************************************/
	// Textures
	SGD::HTexture m_hTutMain = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutTwo = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutThree = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutFour = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutFive = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutSix = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutSeven = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutEight = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutNine = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutNineB = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut10 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut11 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut12 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut13 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut14 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut15 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTut16 = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTutFinal = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Audio
	SGD::HAudio m_hSlide1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide4 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide5 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide6 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide7 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide8 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide9 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide10 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide11 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide12 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide13 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide14 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide15 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide16 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide17 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlide18 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRock = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Factory Methods
	Button* CreateButton() const;

	/**********************************************************/
	// Enumerations
	enum Tabs {
		TUT_MAIN, TUT_INTRO2, TUT_INTRO3, TUT_INTRO4, TUT_PICKUPS, TUT_ENEMIES,
		TUT_MOVEMENT, TUT_BUILD1, TUT_BUILD2, TUT_BUILD3, TUT_BUILD4,
		TUT_BUILD5, TUT_SHOP, TUT_TOWERS, TUT_SHOOTING, TUT_NADES, TUT_HEALING,
		TUT_THANKYOU, TABS_TOTAL};

	/**********************************************************/
	// Menu Items
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	// ARCADE_MODE
	bool m_bTHEBOOL;
	bool m_bAccept;
	SGD::Vector m_vtStick;
	//BOOLS
	bool m_bSlide1 = false;
	bool m_bSlide2 = false;
	bool m_bSlide3 = false;
	bool m_bSlide4 = false;
	bool m_bSlide5 = false;
	bool m_bSlide6 = false;
	bool m_bSlide7 = false;
	bool m_bSlide8 = false;
	bool m_bSlide9 = false;
	bool m_bSlide10 = false;
	bool m_bSlide11 = false;
	bool m_bSlide12 = false;
	bool m_bSlide13 = false;
	bool m_bSlide14 = false;
	bool m_bSlide15 = false;
	bool m_bSlide16 = false;
	bool m_bSlide17 = false;
	bool m_bSlide18 = false;
};