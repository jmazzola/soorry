#include "StatsState.h"
#include "BitmapFont.h"
#include "StatTracker.h"
#include "Game.h"
#include "MainMenuState.h"
#include "Button.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"

StatsState* StatsState::GetInstance(void)
{
	static StatsState m_pInstance;
	return &m_pInstance;
}


 void StatsState::Enter(void)
{
	 m_fScrollY = 250.0f;
	 SetTransition(false);
	 m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/images/menus/Blank.png");
	 m_pStats = StatTracker::GetInstance();
	 m_pStats->Load("resource/data/stats.xml");

	 m_pMainButton = CreateButton();
	m_pMainButton->SetSize({ 350, 70 });
	m_pMainButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);
	m_pFont = Game::GetInstance()->GetFont();
#if ARCADE_MODE
	m_vtStick = {0.0f, 0.0f};
	m_bAccept = true;
#endif
	
}

 void StatsState::Exit(void)
{
	 SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	 // Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;
}

 bool StatsState::Input(void)
{
	 SGD::InputManager* pInput = SGD::InputManager::GetInstance();

#if !ARCADE_MODE
	 if ( pInput->IsKeyDown ( SGD::Key::Down ) || pInput->IsDPadDown ( 0 , SGD::DPad::Down ) )
	 {
		 m_fScrollY -= 0.5f;
		
	 }
	 else if ( pInput->IsKeyDown ( SGD::Key::Up ) || pInput->IsDPadDown ( 0 , SGD::DPad::Up ) )
	 {
		 
		m_fScrollY += 0.5f;
		
	 }

	 if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
	{
		// Since there's only one state..go back to main menu
		Game::GetInstance()->Transition(MainMenuState::GetInstance());
		return true;
	}
		
#endif
	
#if ARCADE_MODE
		 m_vtStick = pInput->GetLeftJoystick(0);
	 
	 if(abs(m_vtStick.x) < 0.2f)
		 m_vtStick.x = 0.0f;
	 if(abs(m_vtStick.y) < 0.2f)
		 m_vtStick.y = 0.0f;

	 // CHANGE ACCEPT TO FALSE IF HARD SWITCHING IS IMPLEMENTED
	 if(m_bAccept == true)
	 {
		if(m_vtStick.y < 0)
			m_fScrollY += 0.5f;

		if(m_vtStick.y > 0)
			m_fScrollY -= 0.5f;
	 }

	 if(pInput->IsButtonPressed(0, 0))
	 {
	 // Since there's only one state..go back to main menu
		Game::GetInstance()->Transition(MainMenuState::GetInstance());
		return true;
	 }
#endif
	 return true;
}

 void StatsState::Update(float elapsedTime)
{

}

 void StatsState::Render ( void )
 {
	 SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance ();
	 BitmapFont * pFont = Game::GetInstance()->GetFont();
	 // If we're transitioning
	 if ( IsTransitioning () )
	 {
		 // Draw the background coming from the bottom - up
		 pGraphics->DrawTexture ( m_hBackground , SGD::Point { 0 , 800 / TRANSITION_TIME * m_fTransitionTime } );
	 }
	 else
	 {
		 // Draw the background
		 pGraphics->DrawTexture ( m_hBackground , { 0 , 0 } );


		 m_pStats->Render ( m_fScrollY );


		 // Draw rectangles to cut off the words to give an illusion of margins
		 pGraphics->DrawTextureSection ( m_hBackground , { 0 , 0 } ,
			 SGD::Rectangle ( 0 , 0 , 800 , 228 ) , {} , {} );

		 pGraphics->DrawTextureSection ( m_hBackground , { 0 , 475 } ,
			 SGD::Rectangle ( 0 , 475 , 800 , 600 ) , {} , {} );

		 // Render button
		// m_pMainButton->Draw ( "Go Back" , { 200 , 500 } , { 255 , 0 , 0 } , { 1 , 1 } , 0 );

		 pFont->Draw ( "Stats" , Game::GetInstance ()->GetScreenWidth () / 2 - (int)((m_pFont->GetTextWidth ( "Stats" ) / 2) * 1.2f) - 20 , 100 , 1.2f , SGD::Color ( 255 , 0 , 0 , 0 ) );
	 }
 }

 // CreateButton
// - factory method for buttons
Button* StatsState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}