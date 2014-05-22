/***************************************************************
|	File:		OptionsState.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This state will allow the player to adjust 
|				in-game variables such as game volume.
***************************************************************/

#include "OptionsState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "GameplayState.h"

#include "Button.h"

#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"

#include "BitmapFont.h"

#include "Entity.h"
#include "EntityManager.h"

#include <cstdlib>
#include <cassert>
#include <sstream>
using namespace std;


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cfloat>

#define STRING_CONFIGPATH "resource/data/config.xml"

/**************************************************************/
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState s_Instance;	// stored in global memory once
	return &s_Instance;
}



/**************************************************************/
// Enter
//	- reset game
//	- load resources
//	- set up entities
/*virtual*/ void OptionsState::Enter(void)
{
	Game* pGame = Game::GetInstance();

	// Initialize the Event Manager
	m_pEvents = SGD::EventManager::GetInstance();
	m_pEvents->Initialize();

	//// Initialize the Message Manager
	//m_pMessages = SGD::MessageManager::GetInstance();
	//m_pMessages->Initialize(&MessageProc);


	// Allocate the Entity Manager
	m_pEntities = new EntityManager;


	// Load Textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	m_hBackground = pGraphics->LoadTexture("resource/images/menus/optionsBG.png");


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
	m_pMainButton->Initialize("resource/images/menus/mainMenuButton.png", m_pFont);

	// Load the Options
	LoadOptions(STRING_CONFIGPATH);

}


/**************************************************************/
// Exit
//	- deallocate entities
//	- unload resources
/*virtual*/ void OptionsState::Exit(void)
{
	// Release textures
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	pGraphics->UnloadTexture(m_hBackground);

	// Release audio
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();


	// Deallocate the Entity Manager
	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;


	/*m_pMessages->Terminate();
	m_pMessages = nullptr;
	SGD::MessageManager::DeleteInstance();*/


	// Terminate & deallocate the SGD wrappers
	m_pEvents->Terminate();
	m_pEvents = nullptr;
	SGD::EventManager::DeleteInstance();

	// Terminate & deallocate menu items
	m_pMainButton->Terminate();
	delete m_pMainButton;
	m_pMainButton = nullptr;

	// Save the options to a config file
	SaveOptions(STRING_CONFIGPATH);

}


/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool OptionsState::Input(void)
{
	Game* pGame = Game::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// --- Scrolling through options ---
	// If the down arrow (PC), or down dpad (Xbox 360) are pressed
	// Move the cursor (selected item) down
	if (pInput->IsKeyPressed(SGD::Key::Down) || pInput->IsDPadPressed(0, SGD::DPad::Down))
	{
		// TODO: Add sound fx for going up and down
		++m_nCursor;

		// Wrap around the options
		if (m_nCursor > MENU_GOBACK)
			m_nCursor = MENU_MUSICVOL;
	}
	// If the up arrow (PC), or up dpad (Xbox 360) are pressed
	// Move the cursor (selected item) up
	else if (pInput->IsKeyPressed(SGD::Key::Up) || pInput->IsDPadPressed(0, SGD::DPad::Up))
	{
		--m_nCursor;

		// Wrap around the options
		if (m_nCursor < MENU_MUSICVOL)
			m_nCursor = MENU_GOBACK;
	}
	// --- Selecting an option ---
	// If the enter key (PC) or A button (Xbox 360) are pressed
	// Select the item
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonReleased(0, (unsigned int)SGD::Button::A))
	{
		// Switch table for the item selected
		switch (m_nCursor)
		{

			case MENU_GOBACK:
			{
					//Go to Main Menu
					pGame->ChangeState(MainMenuState::GetInstance());
					// Exit immediately
					return true;
			}
			break;
		}
	}
	// --- Increasing an option ---
	// If the right key (PC) or right dpad (Xbox 360) are pressed
	// Increase the value
	if (pInput->IsKeyPressed(SGD::Key::Right) || pInput->IsDPadPressed(0, SGD::DPad::Right))
	{
		switch (m_nCursor)
		{
			case MENU_MUSICVOL:
			{
				// Increase the music volume += 5
				pAudio->SetMasterVolume(SGD::AudioGroup::Music, pAudio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
			}
				break;

			case MENU_SFXVOL:
			{
				// Increase the sound effects volume += 5
				pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
			}
				break;
		}
	}
	// --- Decreasing an option ---
	// If the left key (PC) or left dpad (Xbox 360) are pressed
	// Decrease the value
	if (pInput->IsKeyPressed(SGD::Key::Left) || pInput->IsDPadPressed(0,SGD::DPad::Left))
	{
		switch (m_nCursor)
		{
			case MENU_MUSICVOL:
			{
				// Increase the music volume -= 5
				pAudio->SetMasterVolume(SGD::AudioGroup::Music, pAudio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
			}
				break;

			case MENU_SFXVOL:
			{
				// Increase the sound effects volume -= 5
				pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);
			}
				break;
		}
	}

	return true;	// keep playing
}


/**************************************************************/
// Update
//	- update game entities
/*virtual*/ void OptionsState::Update(float elapsedTime)
{


	// Update the entities
	m_pEntities->UpdateAll(elapsedTime);


	// Process the events & messages
	m_pEvents->Update();
	//m_pMessages->Update();


	// Check collisions
}


/**************************************************************/
// Render
//	- render the game entities
/*virtual*/ void OptionsState::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	// Render the background
	pGraphics->DrawTexture(m_hBackground, SGD::Point{ 0, 0 });


	// Render the entities
	m_pEntities->RenderAll();

	// TODO: Add Strings to STRING TABLE for easy localization
	// Draw the buttons and text (Super JIT, later make a conditional for the selected color)
	
	// Create the string for the button
	string musicVol = "Music Vol: ";
	// Grab the volume
	int musicVolValue = pAudio->GetMasterVolume(SGD::AudioGroup::Music);
	// Add it to the string since C++ doesn't support [ string + "" ] 
	musicVol.append(std::to_string(musicVolValue));

	// Same stuff here for the sfx vol
	string sfxVol = "SFX Vol: ";
	int sfxVolValue = pAudio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
	sfxVol.append(std::to_string(sfxVolValue));

	if (m_nCursor == MENU_MUSICVOL)
		m_pMainButton->Draw(musicVol, { 140, 200 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw(musicVol, { 140, 200 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

	if (m_nCursor == MENU_SFXVOL)
		m_pMainButton->Draw(sfxVol, { 120, 290 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw(sfxVol, { 120, 290 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);

	if (m_nCursor == MENU_GOBACK)
		m_pMainButton->Draw("Go Back", { 160, 380 }, { 255, 0, 0 }, { 0.9f, 0.9f }, 0);
	else
		m_pMainButton->Draw("Go Back", { 160, 380 }, { 0, 0, 0 }, { 0.9f, 0.9f }, 0);
}

/**************************************************************/
// Factory Methods

// CreateButton
// - factory method for buttons
Button* OptionsState::CreateButton() const
{
	Button* pButton = new Button();
	pButton->SetColor({ 0, 0, 0 });
	pButton->SetPosition({ 0, 0 });
	pButton->SetScale({ 1, 1 });
	pButton->SetText("");
	pButton->SetSize({ 314, 70 });

	return pButton;
}

/*************************************************************/
// Functions

// LoadOptions
//	- load options configuration (volumes and such)
// [in] xmlSavegame - the file path to the savegame where you want to load it from
void OptionsState::LoadOptions(string xmlSavegame)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load the file, if not gtfo
	if (!doc.LoadFile(xmlSavegame.c_str()))
		return;

	// Access the root element (volume)
	TiXmlElement* pRoot = doc.RootElement();

	// Is the root there, if not, gtfo
	if (pRoot == nullptr)
		return;

	// Get the volume
	TiXmlElement* pVol = pRoot->FirstChildElement("vol");

	// Grab the game volume according to the element
	const char* gameVolume = pVol->Attribute("music");
	// Set the game volume
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, atoi(gameVolume));

	// Grab the sound effects volume according to the element
	const char* sfxVolume = pVol->Attribute("sfx");
	// Set the sfx volume
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, atoi(sfxVolume));

}

// SaveOptions
//	- save the player's configuration to the xml file
// [in] xmlSavegame - the file path to the savegame where you want to save to
void OptionsState::SaveOptions(string xmlSavegame)
{
	// Create a TinyXML doc
	TiXmlDocument doc;

	// Allocate a Tiny XML Declaration
	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "utf-8", "");

	// Attach the declaration to the document
	doc.LinkEndChild(pDecl);

	// Add a new element 'volume'
	TiXmlElement* pRoot = new TiXmlElement("volume");

	// Add a new element 'vol'
	TiXmlElement* pVol = new TiXmlElement("vol");

	// Attach vol to volume
	pRoot->LinkEndChild(pVol);

	// Set the vol's attributes to the settings set by the player
	pVol->SetAttribute("music", SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music));
	pVol->SetAttribute("sfx", SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects));

	// Attach the root to the doc
	doc.LinkEndChild(pRoot);

	// Save and write the savegame
	doc.SaveFile(xmlSavegame.c_str());
}