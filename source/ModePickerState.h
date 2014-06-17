/***************************************************************
|	File:		GameplayState.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will allow the player to pick what gamemode
|				they want
***************************************************************/
#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Button;
class BitmapFont;

class ModePickerState :	public IGameState
{
public:

	/**********************************************************/
	// Singleton Accessor:
	static ModePickerState* GetInstance(void);

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
	ModePickerState() = default;
	virtual ~ModePickerState() = default;

	ModePickerState(const ModePickerState&) = delete;
	ModePickerState& operator= (const ModePickerState&) = delete;

	// Button Factory
	Button* CreateButton() const;

	// Menu Items
	Button* m_pMainButton;
	BitmapFont* m_pFont;

	// Cursor Index
	int	m_nCursor;

	// Textures
	SGD::HTexture m_hBackground = SGD::INVALID_HANDLE;

	// Enumeration
	enum Modes { ARCADE, HARDCORE, SANDBOX, BEAVERFEVER, RUNNINGMAN, GOBACK, TOTAL };

	SGD::Point m_ptButtonPositions[TOTAL];

	// ARCADE_MODE
	bool m_bTHEBOOL;
	bool m_bAccept;
	SGD::Vector m_vtStick;
};

