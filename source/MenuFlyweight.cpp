/***************************************************************
|	File:		MenuFlyweight.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	A flyweight to allow every menu to share the audio
|				for selecting, and picking menu options.
***************************************************************/

#include "MenuFlyweight.h"

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
}

void MenuFlyweight::Unload()
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	pAudio->UnloadAudio(m_hClickSound);
	pAudio->UnloadAudio(m_hPageTurnSound);
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
