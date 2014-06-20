/***************************************************************
|	File:		Game.h
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef GAME_H
#define GAME_H

#define ARCADE_MODE 0

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
class MenuFlyweight;

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
		m_pGraphics->ToggleFullScreen();
	}


	// Debug Mode Accessor:
	bool IsDebugMode() { return m_bDebugMode; }
	int GetDebugCurs() { return m_nDebugCurs; }
	bool IsGod() { return m_bGodMode; }
	bool IsShowingPaths() { return m_bShowPaths; }
	bool IsShowingRects() { return m_bShowCollisionRects; }
	bool HasInfAmmo() { return m_bInfiniteAmmo; }
	bool IsShowingPos() { return m_bShowPlayerPos; }

	// Debug Mode Mutator:
	void SetDebugging(bool debugging) { m_bDebugMode = debugging; }
	void SetDebugCurs(int pos) { m_nDebugCurs = pos; }
	void SetGod(bool god) { m_bGodMode = god; }
	void SetShowPaths(bool paths) { m_bShowPaths = paths; }
	void SetShowRects(bool rect) { m_bShowCollisionRects = rect; }
	void SetInfAmmo(bool inf) { m_bInfiniteAmmo = inf; }
	void SetShowPos(bool pos) { m_bShowPlayerPos = pos; }

	void MouseWheel(int direction);

	/**********************************************************/
	// Game State Machine:
	//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void ChangeState(IGameState* pNewState);


	void Transition(IGameState* to);

	MenuFlyweight* GetMenuFlyweight() const;


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


	/**********************************************************/
	// Game Time
	unsigned long			m_ulGameTime = 0;

	/**********************************************************/
	// FPS stuff
	unsigned int m_unFPS = 100000000;
	unsigned int m_unFrames = 0;
	float m_fFPSTimer = 1.0f;

	/**********************************************************/
	// Controller Check Timer
	float m_fControllerCheck;

	// Fullscreen
	bool m_bFullscreen = true;

	// Debug Mode
	bool m_bDebugMode = false;
	int m_nDebugCurs = 0;

	bool m_bGodMode = false;
	bool m_bInfiniteAmmo = false;
	bool m_bShowPaths = false;
	bool m_bShowCollisionRects = false;
	bool m_bShowPlayerPos = false;

	MenuFlyweight* m_pMenuFlyweight;
};

#endif //GAME_H
