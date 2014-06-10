#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"

class StatTracker;
class Button;
class BitmapFont;

class StatsState :
	public IGameState
{
public:

	/**********************************************************/
	// Singleton Accessor:
	static StatsState* GetInstance(void);

	/**********************************************************/
	// IGameState Interface:
	virtual void Enter(void) override;
	virtual void Exit(void) override;

	virtual bool Input(void) override;
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

private:

	/**********************************************************/
	// SINGLETON (not-dynamically allocated)
	StatsState () = default;
	virtual ~StatsState () = default;

	StatsState(const StatsState&) = delete;
	StatsState& operator= (const StatsState&) = delete;

	// Button Factory - (NOT THE GUMDROP BUTTONS!)
	Button* CreateButton() const;

	// Menu Items
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	// Cursor Index
	int	m_nCursor;

	// StatTracker pointer
	StatTracker* m_pStats;

	// Scrolling text location
	float m_fScrollY;

	// Enum to check if they pressed Stats
	enum ResetStatsStatus { AREYOUSURE, NOSRS, AIGHTITSGONE };
	int m_nResetStatsStatus;

	// Textures
	SGD::HTexture m_hBackground;

	enum Stops { START = 250, BUILD = 530, SURVIVE = 810, WALK = 1090, ROUNDS = 1370, STREAK = 1650,
				 KILLS = 1930, BLOOD = 2210, GRENADE = 2490, SHOTS = 2770, MG = 3050, SG = 3330,
				 RPG = 3610, HT = 3890, WALLS = 4170, WINDOWS = 4450, TOWERS = 4730, TRAPS = 5100,
				 MONEY = 5290, END};
};

