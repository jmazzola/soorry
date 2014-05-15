/***************************************************************
|	File:		MenuFlyweight.h
|	Author:
|	Course:
|	Purpose:
***************************************************************/

#pragma once


#include "../SGD Wrappers/SGD_Handle.h"


class MenuFlyweight
{
public:

	MenuFlyweight();
	~MenuFlyweight();

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
};

