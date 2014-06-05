/***************************************************************
|	File:		MenuFlyweight.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	A flyweight to allow every menu to share the audio
|				for selecting, and picking menu options.
***************************************************************/

#include "MenuFlyweight.h"


MenuFlyweight::MenuFlyweight()
{
}


MenuFlyweight::~MenuFlyweight()
{
}


/**********************************************************/
// Accessors

SGD::HAudio MenuFlyweight::GetClickUpSound() const
{
	return m_hClickUpSound;
}

SGD::HAudio MenuFlyweight::GetClickDownSound() const
{
	return m_hClickDownSound;
}

SGD::HAudio MenuFlyweight::GetSelectSound() const
{
	return m_hSelectSound;
}

/**********************************************************/
// Mutators

void MenuFlyweight::SetClickUpSound(SGD::HAudio _clickUpSound)
{
	m_hClickUpSound = _clickUpSound;
}

void MenuFlyweight::SetClickDownSound(SGD::HAudio _clickDownSound)
{
	m_hClickDownSound = _clickDownSound;
}

void MenuFlyweight::SetSelectSound(SGD::HAudio _selectSound)
{
	m_hSelectSound = _selectSound;
}