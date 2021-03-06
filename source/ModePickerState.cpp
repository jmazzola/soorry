/***************************************************************
|	File:		GameplayState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will allow the player to pick what gamemode
|				they want
***************************************************************/

#include "ModePickerState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "GameplayState.h"
#include "LoadSaveState.h"

#include "MenuFlyweight.h"

#include "BitmapFont.h"
#include "Button.h"

/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ ModePickerState* ModePickerState::GetInstance(void)
{
	static ModePickerState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void ModePickerState::Enter(void)
{

	Game* pGame = Game::GetInstance();
	SetTransition(false);

	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/1405_RazorBalloon_ModePicker.png");

	// Load Audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Set the cursor's index to the first option
	m_nCursor = 0;

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Setup the main button (text)
	m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/1405_RazorBalloon_BlankButton2.png", m_pFont);

	// Show the cursor
	if (pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();

	m_ptButtonPositions[ARCADE] = SGD::Point(120, 180);
	m_ptButtonPositions[HARDCORE] = SGD::Point(130, 240);
	m_ptButtonPositions[SANDBOX] = SGD::Point(160, 300);
	m_ptButtonPositions[BEAVERFEVER] = SGD::Point(130, 360);
	m_ptButtonPositions[RUNNINGMAN] = SGD::Point(150, 420);
	m_ptButtonPositions[GOBACK] = SGD::Point(140, 480);
}

/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void ModePickerState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hBackground);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool ModePickerState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	MenuFlyweight* mf = Game::GetInstance()->GetMenuFlyweight();

#if ARCADE_MODE
	 m_vtStick = pInput->GetLeftJoystick(0);
	 
	 if(abs(m_vtStick.x) < 0.2f)
		 m_vtStick.x = 0.0f;
	 if(abs(m_vtStick.y) < 0.2f)
		 m_vtStick.y = 0.0f;

	 if ( m_vtStick == SGD::Vector { 0.0f , 0.0f } )
		 m_bAccept = true;
#endif

	// --- Scrolling through options ---
	// If the down arrow (PC), or down dpad (Xbox 360) are pressed

	// Move the cursor (selected item) down
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = m_vtStick.y > 0 && m_bAccept;
#endif
	if (m_bTHEBOOL)
	{
		// TODO: Add sound fx for going up and down
		++m_nCursor;
		pAudio->PlayAudio(mf->GetClickSound());

		// Wrap around the options
		if (m_nCursor > GOBACK)
			m_nCursor = ARCADE;

#if ARCADE_MODE
		m_bAccept = false;
#endif
	}
	// If the up arrow (PC), or up dpad (Xbox 360) are pressed
	// Move the cursor (selected item) up
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = m_vtStick.y < 0 && m_bAccept;
#endif
	if (m_bTHEBOOL)
	{
		--m_nCursor;
		pAudio->PlayAudio(mf->GetClickSound());

		// Wrap around the options
		if (m_nCursor < ARCADE)
			m_nCursor = GOBACK;

#if ARCADE_MODE
		m_bAccept = false;
#endif
	}

	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
#if !ARCADE_MODE
	 m_bTHEBOOL = pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A);
#endif
#if ARCADE_MODE
	 m_bTHEBOOL = pInput->IsButtonPressed(0, 0);
#endif

	 // Mouse Selection
	 if (pInput->GetMouseMovement() != SGD::Vector(0, 0))
	 {
		 for (int i = 0; i < TOTAL; i++)
		 {
			 if (pInput->GetMousePosition().IsWithinRectangle(SGD::Rectangle(m_ptButtonPositions[i], m_pMainButton->GetSize() * 0.8f)))
			 {
				 if (m_nCursor != i)
				 {
					m_nCursor = i;
					pAudio->PlayAudio(mf->GetClickSound());
				 }
			 }
		 }
	 }
	 if (pInput->IsKeyPressed(SGD::Key::MouseLeft))
	 {
		 for (int i = 0; i < TOTAL; i++)
		 {
			 if (pInput->GetMousePosition().IsWithinRectangle(SGD::Rectangle(m_ptButtonPositions[i], m_pMainButton->GetSize() * 0.8f)))
			 {
				 m_bTHEBOOL = true;
			 }
		 }
	 }

	// Select the item
	if (m_bTHEBOOL)
	{
		GameplayState* pGameplay = GameplayState::GetInstance();
		// Switch table for the item selected
		switch (m_nCursor)
		{
		case ARCADE:
		{
					   pGameplay->SetGameMode(0);
					   pGame->ChangeState(pGameplay);
					   return true;
		}
			break;

		case HARDCORE:
		{
						 pGameplay->SetGameMode(1);
						 pGame->ChangeState(pGameplay);
						 return true;
		}
			break;

		case SANDBOX:
		{
						pGameplay->SetGameMode(2);
						pGame->ChangeState(pGameplay);
						return true;
		}
			break;

		case BEAVERFEVER:
		{
							pGameplay->SetGameMode(3);
							pGame->ChangeState(pGameplay);
							return true;
		}
			break;

		case RUNNINGMAN:
		{
						   pGameplay->SetGameMode(4);
						   pGame->ChangeState(pGameplay);
						   return true;
		}
			break;

		case GOBACK:
		{
			pGame->Transition(LoadSaveState::GetInstance());
			return true;
		}
			break;

		}
	}

	return true;
}

/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void ModePickerState::Update(float elapsedTime)
{
	// If we're changing menus, count down the timer
	if (IsTransitioning())
	{
		m_fTransitionTime -= elapsedTime;

		if (m_fTransitionTime <= 0)
			SetTransition(false);
	}
	else
	{
		// Reset the transition time to allow for transitions again
		m_fTransitionTime = TRANSITION_TIME;
	}
}

/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void ModePickerState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// If we're transitioning
	if (IsTransitioning())
	{
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 800 / TRANSITION_TIME * m_fTransitionTime });
	}
	else
	{
		// Render the background
		pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 0 });

		if (m_nCursor == ARCADE)
			m_pMainButton->Draw("ARCADE", { 125, 180 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
		else
			m_pMainButton->Draw("ARCADE", { 125, 180 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

		if (m_nCursor == HARDCORE)
			m_pMainButton->Draw("HARDCORE", { 170, 240 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
		else
			m_pMainButton->Draw("HARDCORE", { 170, 240 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

		if (m_nCursor == SANDBOX)
			m_pMainButton->Draw("SANDBOX", { 130, 300 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
		else
			m_pMainButton->Draw("SANDBOX", { 130, 300 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

		if (m_nCursor == BEAVERFEVER)
			m_pMainButton->Draw("BEAVER FEVER", { 110, 360 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
		else
			m_pMainButton->Draw("BEAVER FEVER", { 110, 360 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

		if (m_nCursor == RUNNINGMAN)
			m_pMainButton->Draw("RUNNING MAN", { 185, 420 }, { 255, 0, 0 }, { 0.8f, 0.8f }, 0);
		else
			m_pMainButton->Draw("RUNNING MAN", { 185, 420 }, { 0, 0, 0 }, { 0.8f, 0.8f }, 0);

		if (m_nCursor == GOBACK)
			m_pMainButton->Draw("Go Back", { 135, 480 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
		else
			m_pMainButton->Draw("Go Back", { 135, 480 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

		// Draw the description
		DrawDescription(m_nCursor);

	}
}

void ModePickerState::DrawDescription(int mode)
{
	switch (mode)
	{
	case ARCADE:
		m_pFont->Draw("Classic Arcade", 560, 500, 0.3f, { 0, 0, 0 });
		break;
	case HARDCORE:
		m_pFont->Draw("This mode is hard.", 560, 500, 0.3f, { 0, 0, 0 });
		break;
	case SANDBOX:
		m_pFont->Draw("Unlimited Blocks!", 560, 500, 0.3f, { 0, 0, 0 });
		break;
	case BEAVERFEVER:
		m_pFont->Draw("1000 beavers. What more", 560, 500, 0.3f, { 0, 0, 0 });
		m_pFont->Draw("could you want?", 560, 522, 0.3f, { 0, 0, 0 });
		break;
	case RUNNINGMAN:
		m_pFont->Draw("Keep moving. Or you'll die.", 560, 500, 0.3f, { 0, 0, 0 });
		m_pFont->Draw("(True Hardcore) - Ryan Simmons", 560, 522, 0.3f, { 0, 0, 0 });
		break;
	}
}


/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* ModePickerState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}