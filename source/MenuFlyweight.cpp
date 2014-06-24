/***************************************************************
|	File:		MenuFlyweight.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	A flyweight to allow every menu to share the audio
|				for selecting, and picking menu options.
***************************************************************/

#include "MenuFlyweight.h"
#include "OptionsState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


MenuFlyweight::MenuFlyweight()
{
}


MenuFlyweight::~MenuFlyweight()
{
}


void MenuFlyweight::Load()
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	m_hClickSound = pAudio->LoadAudio("resource/audio/click.wav");
	m_hPageTurnSound = pAudio->LoadAudio("resource/audio/pageTurn.wav");
	m_hMenuMusic = pAudio->LoadAudio(L"resource/audio/O Canada.xwm");
	OptionsState::GetInstance()->LoadOptions("resource/data/config.xml");
}

void MenuFlyweight::Unload()
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	pAudio->UnloadAudio(m_hClickSound);
	pAudio->UnloadAudio(m_hPageTurnSound);
	pAudio->UnloadAudio(m_hMenuMusic);
}

/**********************************************************/
// Accessors

SGD::HAudio MenuFlyweight::GetClickSound() const
{
	return m_hClickSound;
}

SGD::HAudio MenuFlyweight::GetPageTurnSound() const
{
	return m_hPageTurnSound;
}

SGD::HAudio MenuFlyweight::GetMenuMusic() const
{
	return m_hMenuMusic;
}
