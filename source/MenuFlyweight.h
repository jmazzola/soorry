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
	SGD::HAudio GetClickUpSound() const;
	SGD::HAudio GetClickDownSound() const;
	SGD::HAudio GetSelectSound() const;

	/**********************************************************/
	// Mutators
	void SetClickUpSound(SGD::HAudio clickUpSound);
	void SetClickDownSound(SGD::HAudio clickDownSound);
	void SetSelectSound(SGD::HAudio selectSound);

protected:

	/**********************************************************/
	// Textures

	/**********************************************************/
	// Audio
	SGD::HAudio m_hClickUpSound;
	SGD::HAudio m_hClickDownSound;
	SGD::HAudio m_hSelectSound;
	//SGD::HAudio m_hClickSound = SGD::INVALID_HANDLE;
};

