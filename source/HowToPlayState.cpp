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

	m_hSlide1	= pAudio->LoadAudio("resource/audio/Slides/Slide1.wav"	);
	m_hSlide2	= pAudio->LoadAudio("resource/audio/Slides/Slide2.wav"	);
	m_hSlide3	= pAudio->LoadAudio("resource/audio/Slides/Slide3.wav"	);
	m_hSlide4	= pAudio->LoadAudio("resource/audio/Slides/Slide4.wav"	);
	m_hSlide5	= pAudio->LoadAudio("resource/audio/Slides/Slide5.wav"	);
	m_hSlide6	= pAudio->LoadAudio("resource/audio/Slides/Slide6.wav"	);
	m_hSlide7	= pAudio->LoadAudio("resource/audio/Slides/Slide7.wav"	);
	m_hSlide8	= pAudio->LoadAudio("resource/audio/Slides/Slide8.wav"	);
	m_hSlide9	= pAudio->LoadAudio("resource/audio/Slides/Slide9.wav"	);
	m_hSlide10	= pAudio->LoadAudio("resource/audio/Slides/Slide10.wav"	);
	m_hSlide11	= pAudio->LoadAudio("resource/audio/Slides/Slide11.wav"	);
	m_hSlide12	= pAudio->LoadAudio("resource/audio/Slides/Slide12.wav"	);
	m_hSlide13	= pAudio->LoadAudio("resource/audio/Slides/Slide13.wav"	);
	m_hSlide14	= pAudio->LoadAudio("resource/audio/Slides/Slide14.wav"	);
	m_hSlide15	= pAudio->LoadAudio("resource/audio/Slides/Slide15.wav"	);
	m_hSlide16	= pAudio->LoadAudio("resource/audio/Slides/Slide16.wav"	);
	m_hSlide17	= pAudio->LoadAudio("resource/audio/Slides/Slide17.wav"	);
	m_hSlide18	= pAudio->LoadAudio("resource/audio/Slides/Slide18.wav"	);
	
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
	pAudio->UnloadAudio(m_hSlide1);
	pAudio->UnloadAudio(m_hSlide2);
	pAudio->UnloadAudio(m_hSlide3);
	pAudio->UnloadAudio(m_hSlide4);
	pAudio->UnloadAudio(m_hSlide5);
	pAudio->UnloadAudio(m_hSlide6);
	pAudio->UnloadAudio(m_hSlide7);
	pAudio->UnloadAudio(m_hSlide8);
	pAudio->UnloadAudio(m_hSlide9);
	pAudio->UnloadAudio(m_hSlide10);
	pAudio->UnloadAudio(m_hSlide11);
	pAudio->UnloadAudio(m_hSlide12);
	pAudio->UnloadAudio(m_hSlide13);
	pAudio->UnloadAudio(m_hSlide14);
	pAudio->UnloadAudio(m_hSlide15);
	pAudio->UnloadAudio(m_hSlide16);
	pAudio->UnloadAudio(m_hSlide17);
	pAudio->UnloadAudio(m_hSlide18);
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

#if !ARCADE_MODE
	if (pInput->IsKeyPressed(SGD::Key::Backspace) || pInput->IsKeyPressed(SGD::Key::Escape))
#else
	if (pInput->IsButtonPressed(0, 6))
#endif
	{
		pGame->Transition(MainMenuState::GetInstance());
		return true;
	}

#if ARCADE_MODE
	m_vtStick = pInput->GetLeftJoystick(0);

	if (abs(m_vtStick.x) < 0.2f)
		m_vtStick.x = 0.0f;
	if (abs(m_vtStick.y) < 0.2f)
		m_vtStick.y = 0.0f;

	if (m_vtStick == SGD::Vector{ 0.0f, 0.0f })
		m_bAccept = true;
#endif

#if !ARCADE_MODE
	if (pInput->IsKeyPressed(SGD::Key::Left))
#else
	if (m_vtStick.x < 0 && m_bAccept)
#endif
	{
		if (m_nTab != TUT_MAIN)
		{
			--m_nTab;
			pAudio->PlayAudio(mf->GetPageTurnSound());

#if ARCADE_MODE
			m_bAccept = false;
#endif
		}
	}
	
#if !ARCADE_MODE
	if (pInput->IsKeyPressed(SGD::Key::Right))
#else
	if (m_vtStick.x > 0 && m_bAccept)
#endif
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

#if ARCADE_MODE
		m_bAccept = false;
#endif
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
		SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

		switch (m_nTab)
		{
		case TUT_MAIN:
		{
			if (pAudio->IsAudioPlaying(m_hSlide1) == false)
			{
				pAudio->StopAudio(m_hSlide2);
				pAudio->PlayAudio(m_hSlide1);
			}
		}
			break;
		case TUT_INTRO2:
		{
			if (pAudio->IsAudioPlaying(m_hSlide2) == false)
			{
				pAudio->StopAudio(m_hSlide3);
				pAudio->StopAudio(m_hSlide1);
				pAudio->PlayAudio(m_hSlide2);
			}
		}
			break;
		case TUT_INTRO3:
		{
			if (pAudio->IsAudioPlaying(m_hSlide3) == false)
			{
				pAudio->StopAudio(m_hSlide2);
				pAudio->StopAudio(m_hSlide4);
				pAudio->PlayAudio(m_hSlide3);
			}
		}
			break;
		case TUT_INTRO4:
		{
			if (pAudio->IsAudioPlaying(m_hSlide4) == false)
			{
				pAudio->StopAudio(m_hSlide3);
				pAudio->StopAudio(m_hSlide5);
				pAudio->PlayAudio(m_hSlide4);
			}
		}
			break;
		case TUT_PICKUPS:
		{
			if (pAudio->IsAudioPlaying(m_hSlide5) == false)
			{
				pAudio->StopAudio(m_hSlide4);
				pAudio->StopAudio(m_hSlide6);
				pAudio->PlayAudio(m_hSlide5);
			}
		}
			break;
		case TUT_ENEMIES:
		{
			if (pAudio->IsAudioPlaying(m_hSlide6) == false)
			{
				pAudio->StopAudio(m_hSlide5);
				pAudio->StopAudio(m_hSlide7);
				pAudio->PlayAudio(m_hSlide6);
			}
		}
			break;
		case TUT_MOVEMENT:
		{
			if (pAudio->IsAudioPlaying(m_hSlide7) == false)
			{
				pAudio->StopAudio(m_hSlide6);
				pAudio->StopAudio(m_hSlide8);
				pAudio->PlayAudio(m_hSlide7);
			}
		}
			break;
		case TUT_BUILD1:
		{
			if (pAudio->IsAudioPlaying(m_hSlide8) == false)
			{
				pAudio->StopAudio(m_hSlide7);
				pAudio->StopAudio(m_hSlide9);
				pAudio->PlayAudio(m_hSlide8);
			}
		}
			break;
		case TUT_BUILD2:
		{
			if (pAudio->IsAudioPlaying(m_hSlide9) == false)
			{
				pAudio->StopAudio(m_hSlide8);
				pAudio->StopAudio(m_hSlide10);
				pAudio->PlayAudio(m_hSlide9);
			}
		}
			break;
		case TUT_BUILD3:
		{
			if (pAudio->IsAudioPlaying(m_hSlide10) == false)
			{
				pAudio->StopAudio(m_hSlide9);
				pAudio->StopAudio(m_hSlide11);
				pAudio->PlayAudio(m_hSlide10);
			}
		}
			break;
		case TUT_BUILD4:
		{
			if (pAudio->IsAudioPlaying(m_hSlide11) == false)
			{
				pAudio->StopAudio(m_hSlide10);
				pAudio->StopAudio(m_hSlide12);
				pAudio->PlayAudio(m_hSlide11);
			}
		}
			break;
		case TUT_BUILD5:
		{
			if (pAudio->IsAudioPlaying(m_hSlide12) == false)
			{
				pAudio->StopAudio(m_hSlide11);
				pAudio->StopAudio(m_hSlide13);
				pAudio->PlayAudio(m_hSlide12);
			}
		}
			break;
		case TUT_SHOP:
		{
			if (pAudio->IsAudioPlaying(m_hSlide13) == false)
			{
				pAudio->StopAudio(m_hSlide12);
				pAudio->StopAudio(m_hSlide14);
				pAudio->PlayAudio(m_hSlide13);
			}
		}
			break;
		case TUT_TOWERS:
		{
			if (pAudio->IsAudioPlaying(m_hSlide14) == false)
			{
				pAudio->StopAudio(m_hSlide13);
				pAudio->StopAudio(m_hSlide15); 
				pAudio->PlayAudio(m_hSlide14);
			}
		}
			break;
		case TUT_SHOOTING:
		{
			if (pAudio->IsAudioPlaying(m_hSlide15) == false)
			{
				pAudio->StopAudio(m_hSlide14);
				pAudio->StopAudio(m_hSlide16);
				pAudio->PlayAudio(m_hSlide15);
			}
		}
			break;
		case TUT_NADES:
		{
			if (pAudio->IsAudioPlaying(m_hSlide16) == false)
			{
				pAudio->StopAudio(m_hSlide15);
				pAudio->StopAudio(m_hSlide17);
				pAudio->PlayAudio(m_hSlide16);
			}
		}
			break;
		case TUT_HEALING:
		{
			if (pAudio->IsAudioPlaying(m_hSlide17) == false)
			{
				pAudio->StopAudio(m_hSlide16);
				pAudio->StopAudio(m_hSlide18);
				pAudio->PlayAudio(m_hSlide17);
			}
		}
			break;
		case TUT_THANKYOU:
		{
			if (pAudio->IsAudioPlaying(m_hSlide18) == false)
			{
				pAudio->StopAudio(m_hSlide17);
				pAudio->PlayAudio(m_hSlide18);
			}
		}
			break;
		case TABS_TOTAL:
		{

		}
			break;
		}


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
