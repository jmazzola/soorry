/***************************************************************
|	File:		OptionsState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will allow the player to adjust
|				in-game variables such as game volume.
***************************************************************/


#ifndef OPTIONSSTATE_H
#define OPTIONSSTATE_H


#include "IGameState.h"						// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"		// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"
#include <string>
using namespace std;


/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class MenuFlyweight;
class BitmapFont;
class Button;

#include "../SGD Wrappers/SGD_Declarations.h"



/**************************************************************/
// OptionsState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class OptionsState : public IGameState
{
public:
	/**********************************************************/
	// Singleton Accessor:
	static OptionsState* GetInstance( void );

	
	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Input	( void )				override;	// handle user input
	virtual void	Update	( float elapsedTime )	override;	// update game entities / animations
	virtual void	Render	( void )				override;	// render game entities / menus


	/**********************************************************/
	// Load and Save XML
	void LoadOptions(string xmlSavegame);
	void SaveOptions(string xmlSavegame);
	
private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	OptionsState( void )			= default;	// default constructor
	virtual ~OptionsState( void )	= default;	// destructor

	OptionsState( const OptionsState& )				= delete;	// copy constructor
	OptionsState& operator= ( const OptionsState& )	= delete;	// assignment operator

	
	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities			= nullptr;
	Entity* m_pPlayer = nullptr;
	
	
	/**********************************************************/
	// Event & Message Managers
	SGD::EventManager*		m_pEvents			= nullptr;
	SGD::MessageManager*	m_pMessages			= nullptr;


	MenuFlyweight* m_pMenuFlyweight;

	float m_fMusicVolume;
	float m_fSoundEffectsVolume;
	bool m_bMusicMute;
	bool m_bSoundEffectsMute;

	/**********************************************************/
	// Cursor Index
	int	m_nCursor;
	
	/**********************************************************/
	// Textures
	SGD::HTexture m_hBackground;

	/**********************************************************/
	// Audio

	
	/**********************************************************/
	// Factory Methods
	Button* CreateButton() const;

	/**********************************************************/
	// Enumerations
	enum MenuOption { MENU_MUSICVOL, MENU_SFXVOL, MENU_TOGGLEFULLSCREEN, MENU_STATS, MENU_GOBACK };

	/**********************************************************/
	// Menu Items
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	// ARCADE_MODE
	bool m_bTHEBOOL;
	bool m_bAccept;
	SGD::Vector m_vtStick;

	// Message Callback Function:
	/*static void MessageProc( const SGD::Message* pMsg );*/
};

#endif //OPTIONSSTATE_H
