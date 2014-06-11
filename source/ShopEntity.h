/***************************************************************
|	File:		ShopEntity.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will be the actual entity for entering
|				the shop during gameplay
***************************************************************/
#pragma once

#include "Entity.h"

class ShopEntity : public Entity
{
public:

	ShopEntity() = default;
	virtual ~ShopEntity() = default;

	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;

private:

	virtual int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);
};

