/***************************************************************
|	File:		MenuFlyweight.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	A flyweight to allow every menu to share the audio
|				for selecting, and picking menu options.
***************************************************************/

#pragma once


#include "../SGD Wrappers/SGD_Handle.h"


class MenuFlyweight
{
public:

	MenuFlyweight();
	~MenuFlyweight();

	void Load();
	void Unload();

	/**********************************************************/
	// Accessors
	SGD::HAudio GetClickSound() const;
	SGD::HAudio GetPageTurnSound() const;
	SGD::HAudio GetMenuMusic() const;

protected:

	/**********************************************************/
	// Audio
	SGD::HAudio m_hClickSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPageTurnSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hMenuMusic = SGD::INVALID_HANDLE;
};

