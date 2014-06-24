/***************************************************************
|	File:		ShopEntity.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will be the actual entity for entering
|				the shop during gameplay
***************************************************************/

#include "ShopEntity.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Camera.h"
#include "WorldManager.h"
#include "Game.h"

int ShopEntity::GetType() const
{
	return ENT_SHOP;
}

void ShopEntity::Update(float dt)
{

}

SGD::Rectangle ShopEntity::GetRect() const
{

	SGD::Rectangle rect;
	rect.left = m_nX;
	rect.top = m_nY;
	rect.right = rect.left + 32 * 6;
	rect.bottom = rect.top + 32 * 5;

	return rect;
}

void ShopEntity::Render()
{
	//SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	//
	//SGD::Rectangle rect;

	//// The shop will always be a 6 x 5 area.
	//rect.left = GetX() - Camera::x;
	//rect.top = GetY() - Camera::y;
	//rect.right = rect.left + 32 * 6;
	//rect.bottom = rect.top + 32 * 5;

	//pGraphics->DrawRectangle(rect, { 255, 0, 0 });

}

void ShopEntity::HandleCollision(const IEntity* pOther)
{
}