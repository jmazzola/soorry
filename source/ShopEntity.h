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

	virtual SGD::Rectangle GetRect() const override;

	// Accessors
	float GetX() { return m_nX; }
	float GetY() { return m_nY; }

	// Mutators
	void SetX(float x) { m_nX = x; }
	void SetY(float y) { m_nY = y; }


private:

	virtual int GetType() const;
	virtual void HandleCollision(const IEntity* pOther);

	// Position
	float m_nX, m_nY;

};

