/***************************************************************
|	File:		HowToPlayState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will show the player how to play with a slideshow
***************************************************************/
#include "HowToPlayState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "Button.h"
#include "BitmapFont.h"

#include "MainMenuState.h"
#include "GameplayState.h"

#include "MenuFlyweight.h"


/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ HowToPlayState* HowToPlayState::GetInstance(void)
{
	static HowToPlayState s_Instance;	// stored in global memory once
	return &s_Instance;
}

/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void HowToPlayState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	SetTransition(false);

	// Setup BitmapFont
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	m_pFont = pFont;

	// Set background
	m_hTutMain = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_01.png");
	m_hTutTwo = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_02.png");
	m_hTutThree = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_03.png");
	m_hTutFour = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_04.png");
	m_hTutFive = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_05.png");
	m_hTutSix = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_06.png");
	m_hTutSeven = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_07.png");
	m_hTutEight = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_08.png");
	m_hTutNine = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_09.png");
	m_hTutNineB = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_09b.png");
	m_hTut10 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_10.png");
	m_hTut11 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_11.png");
	m_hTut12 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_12.png");
	m_hTut13 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_13.png");
	m_hTut14 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_14.png");
	m_hTut15 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_15.png");
	m_hTut16 = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_16.png");
	m_hTutFinal = pGraphics->LoadTexture("resource/images/menus/HowToPlayBG_Final.png");

	// Setup the main button (text)
	m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/1405_RazorBalloon_BlankButton2.png", m_pFont);


	m_hTutorial = pAudio->LoadAudio("resource/audio/Tutorial1.wav");
	m_hTutorial2 = pAudio->LoadAudio("resource/audio/Tutorial2.wav");
	m_hTutorial3 = pAudio->LoadAudio("resource/audio/Tutorial3.wav");
	m_hTutorial4 = pAudio->LoadAudio("resource/audio/Tutorial4.wav");
	m_hTutorial5 = pAudio->LoadAudio("resource/audio/Tutorial5.wav");
	m_hTutorial6 = pAudio->LoadAudio("resource/audio/Tutorial6.wav");
	m_hTutorial7 = pAudio->LoadAudio("resource/audio/Tutorial7.wav");
	m_hRock = pAudio->LoadAudio(L"resource/audio/Background_Music.xwm");

	// Set the tab
	m_nTab = TUT_MAIN;


	// Show the cursor
	if (pGraphics->IsCursorShowing() == false)
		pGraphics->TurnCursorOn();

#if ARCADE_MODE
	m_bAccept = true;
	m_vtStick = SGD::Vector{ 0.0f, 0.0f };
#endif
}

/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void HowToPlayState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hTutMain);
	pGraphics->UnloadTexture(m_hTutTwo);
	pGraphics->UnloadTexture(m_hTutThree);
	pGraphics->UnloadTexture(m_hTutFour);
	pGraphics->UnloadTexture(m_hTutFive);
	pGraphics->UnloadTexture(m_hTutSix);
	pGraphics->UnloadTexture(m_hTutSeven);
	pGraphics->UnloadTexture(m_hTutEight);
	pGraphics->UnloadTexture(m_hTutNine);
	pGraphics->UnloadTexture(m_hTutNineB);
	pGraphics->UnloadTexture(m_hTut10);
	pGraphics->UnloadTexture(m_hTut11);
	pGraphics->UnloadTexture(m_hTut12);
	pGraphics->UnloadTexture(m_hTut13);
	pGraphics->UnloadTexture(m_hTut14);
	pGraphics->UnloadTexture(m_hTut15);
	pGraphics->UnloadTexture(m_hTut16);
	pGraphics->UnloadTexture(m_hTutFinal);



	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	pAudio->UnloadAudio(m_hTutorial);
	pAudio->UnloadAudio(m_hTutorial2);
	pAudio->UnloadAudio(m_hTutorial3);
	pAudio->UnloadAudio(m_hTutorial4);
	pAudio->UnloadAudio(m_hTutorial5);
	pAudio->UnloadAudio(m_hTutorial6);
	pAudio->UnloadAudio(m_hTutorial7); 
	pAudio->UnloadAudio(m_hRock);

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool HowToPlayState::Input(void)
{

	Game* pGame = Game::GetInstance();
	MenuFlyweight* mf = Game::GetInstance()->GetMenuFlyweight();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Leave tutorial
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyPressed(SGD::Key::Backspace) || pInput->IsKeyPressed(SGD::Key::Escape))
	{
		pGame->Transition(MainMenuState::GetInstance());
		return true;
	}

	if (pInput->IsKeyPressed(SGD::Key::Left))
	{
		if (m_nTab != TUT_MAIN)
		{
			--m_nTab;
			pAudio->PlayAudio(mf->GetPageTurnSound());
		}
	}
	
	if (pInput->IsKeyPressed(SGD::Key::Right))
	{
		if (m_nTab != TABS_TOTAL - 1)
		{
			++m_nTab;
			pAudio->PlayAudio(mf->GetPageTurnSound());
		}
		else if (m_nTab == TABS_TOTAL - 1)
		{
			GameplayState::GetInstance()->SetCurrentGameSlot(1);
			GameplayState::GetInstance()->SetGameMode(0);
			pGame->ChangeState(GameplayState::GetInstance());
		}
	}

	return true;
}

/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void HowToPlayState::Update(float elapsedTime)
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
/*virtual*/ void HowToPlayState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	static bool already = true;

	// If we're transitioning
	if (IsTransitioning())
	{
		pGraphics->DrawTexture(m_hTutMain, SGD::Point{ 0, 800 / TRANSITION_TIME * m_fTransitionTime });
	}
	else
	{
		// Too tired..cant remember to stop the audio from looping..
		if (m_nTab == TUT_MAIN)
		{
			pGraphics->DrawTexture(m_hTutMain, SGD::Point{ 0, 0 });
			/*if (already)
			{
				if (pAudio->IsAudioPlaying(m_hTutorial) == false)
				{
					pAudio->PlayAudio(m_hTutorial);
					already = true;
				}
			}*/
		}
		else if (m_nTab == TUT_INTRO2)
		{
			pGraphics->DrawTexture(m_hTutTwo, SGD::Point{ 0, 0 });

			/*if (already)
			{
				if (pAudio->IsAudioPlaying(m_hTutorial2) == false)
				{
					pAudio->PlayAudio(m_hTutorial2);
					already = true;
				}
			}*/
		}
		else if (m_nTab == TUT_INTRO3)
		{
			pGraphics->DrawTexture(m_hTutThree, SGD::Point{ 0, 0 });

			//if (already)
			//{
			//	if (pAudio->IsAudioPlaying(m_hTutorial3) == false)
			//	{
			//		pAudio->PlayAudio(m_hTutorial3);
			//		already = true;
			//	}
			//}
		}

		else if (m_nTab == TUT_INTRO4)
		{
			pGraphics->DrawTexture(m_hTutFour, SGD::Point{ 0, 0 });

			/*if (already)
			{
				if (pAudio->IsAudioPlaying(m_hTutorial4) == false)
				{
					pAudio->PlayAudio(m_hTutorial4);
					already = true;
				}
			}*/

		}
		else if (m_nTab == TUT_MOVEMENT)
			pGraphics->DrawTexture(m_hTutFive, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_BUILD1)
			pGraphics->DrawTexture(m_hTutSix, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_BUILD2)
			pGraphics->DrawTexture(m_hTutSeven, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_BUILD3)
			pGraphics->DrawTexture(m_hTutEight, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_BUILD4)
			pGraphics->DrawTexture(m_hTutNine, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_BUILD5)
			pGraphics->DrawTexture(m_hTutNineB, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_SHOP)
			pGraphics->DrawTexture(m_hTut10, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_TOWERS)
			pGraphics->DrawTexture(m_hTut11, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_SHOOTING)
			pGraphics->DrawTexture(m_hTut12, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_NADES)
			pGraphics->DrawTexture(m_hTut13, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_HEALING)
			pGraphics->DrawTexture(m_hTut14, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_PICKUPS)
			pGraphics->DrawTexture(m_hTut15, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_ENEMIES)
			pGraphics->DrawTexture(m_hTut16, SGD::Point{ 0, 0 });
		else if (m_nTab == TUT_THANKYOU)
			pGraphics->DrawTexture(m_hTutFinal, SGD::Point{ 0, 0 });

		// Draw what page we're on
		string page = "Page ";
		page += to_string(m_nTab + 1);
		page += " / ";
		page += to_string(TABS_TOTAL);
		m_pFont->Draw(page.c_str(), 50, 282, 0.5f, { 0, 0, 0 });

		/*if (m_nTab > TUT_INTRO4)
		{
			if (already)
			{
				if (pAudio->IsAudioPlaying(m_hTutorial4) == false)
				{
					pAudio->PlayAudio(m_hTutorial4);
					already = true;
				}
			}
		}*/
	}
}


/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* HowToPlayState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}
