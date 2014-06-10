/***************************************************************
|	File:		ShopEntity.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will be the actual entity for entering
|				the shop during gameplay
***************************************************************/

#include "ShopEntity.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

int ShopEntity::GetType() const
{
	return ENT_SHOP;
}

void ShopEntity::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
}