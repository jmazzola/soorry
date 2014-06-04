/***************************************************************
|	File:		Game.h
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef GAME_H
#define GAME_H


/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class BitmapFont;
class IGameState;

#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class AnimationManager;

/**************************************************************/
// Game class
//	- runs the game logic
//	- controls the SGD wrappers
//	- SINGLETON!
//		- there is only ONE instance
//		- global access method (GetInstance)
class Game
{
public:
	/**********************************************************/
	// Singleton Accessors:
	static Game*	GetInstance(void);
	static void		DeleteInstance(void);


	/**********************************************************/
	// Setup, Play, Cleanup:
	bool Initialize(int width, int height);
	int	 Main(void);
	void Terminate(void);


	/**********************************************************/
	// Screen Size Accessors:
	int	GetScreenWidth(void) const	{ return m_nScreenWidth; }
	int	GetScreenHeight(void) const	{ return m_nScreenHeight; }

	// Font Accessor:
	BitmapFont* GetFont(void) const	{ return m_pFont; }


	bool GetFullscreen() const { return m_bFullscreen; }
	void ToggleFullscreen()
	{
		SGD::GraphicsManager::GetInstance()->Resize({ 800, 600 }, !m_bFullscreen);
		m_bFullscreen = !m_bFullscreen;
	}


	/**********************************************************/
	// Game State Machine:
	//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void ChangeState(IGameState* pNewState);


	void Transition(IGameState* to);


private:
	/**********************************************************/
	// Singleton Object:
	static Game*	s_pInstance;

	Game(void) = default;		// default constructor
	~Game(void) = default;		// destructor

	Game(const Game&) = delete;	// copy constructor
	Game& operator= (const Game&) = delete;	// assignment operator


	/**********************************************************/
	// SGD Wrappers
	SGD::AudioManager*		m_pAudio = nullptr;
	SGD::GraphicsManager*	m_pGraphics = nullptr;
	SGD::InputManager*		m_pInput = nullptr;


	/**********************************************************/
	// Screen Size
	int						m_nScreenWidth = 1;
	int						m_nScreenHeight = 1;


	/**********************************************************/
	// Game Font
	BitmapFont*				m_pFont = nullptr;


	/**********************************************************/
	// Current Game State
	IGameState*				m_pCurrState = nullptr;
	
	// Transition State
	IGameState*				m_pTransState	= nullptr;

	// Transition State
	IGameState*				m_pTransState = nullptr;


	/**********************************************************/
	// Game Time
	unsigned long			m_ulGameTime = 0;

	/**********************************************************/
	// FPS stuff
	unsigned int m_unFPS = 100000000;
	unsigned int m_unFrames = 0;
	float m_fFPSTimer = 1.0f;


	// Fullscreen
	bool m_bFullscreen = true;


};

#endif //GAME_H
