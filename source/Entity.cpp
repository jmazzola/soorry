/***************************************************************
|	File:		Entity.cpp
|	Author:		
|	Course:		
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#include "Entity.h"
#include "Game.h"

#include "Sprite.h"
#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>

/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void Entity::Update( float elapsedTime )
{
	m_ptPosition += m_vtVelocity * elapsedTime;
}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void Entity::Render( void )
{
	// Verify the image
	//assert( m_hImage != SGD::INVALID_HANDLE && "Entity::Render - image was not set!" );
	
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	
	// Get the current frame
	int frame = GetSprite()->GetCurrFrame();


	// Find the center of the image
	SGD::Vector center;
	center.x = GetSprite()->GetFrame(frame).GetFrameRect().right - GetSprite()->GetFrame(frame).GetFrameRect().left;
	center.y = GetSprite()->GetFrame(frame).GetFrameRect().bottom - GetSprite()->GetFrame(frame).GetFrameRect().top;
	center.x /= 2;
	center.y /= 2;

	// Calculate the rotation
	SGD::Vector rotate = m_vtVelocity;
	rotate.Normalize();
	float rot = SGD::Vector(0.0f, -1.0f).ComputeSteering(rotate);

	float rotation = 0;
	if(rot > 0)
		rotation = SGD::Vector(0.0f, -1.0f).ComputeAngle(rotate);
	else
		rotation = -SGD::Vector(0.0f, -1.0f).ComputeAngle(rotate);
	
	// Render
	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y, rotation, center);

	// Why is this here?
	SGD::Rectangle drawRect = GetRect();
	drawRect.left -= Camera::x;
	drawRect.right -= Camera::x;
	drawRect.top -= Camera::y;
	drawRect.bottom -= Camera::y;

	// HACK: Modify the rotation
	//m_fRotation += 0.01f;

	// Draw the image

	// -- Debugging Mode --
	Game* pGame = Game::GetInstance();
	if (pGame->IsShowingRects())
		SGD::GraphicsManager::GetInstance()->DrawRectangle(drawRect, SGD::Color(128, 255, 0, 0));
}


void Entity::PostRender()
{

}


/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Entity::GetRect( void ) const
{
	SGD::Rectangle rect = m_pSprite->GetFrame(0).GetCollisionRect();
	rect.left += m_ptPosition.x;
	rect.top += m_ptPosition.y;
	rect.right += m_ptPosition.x;
	rect.bottom += m_ptPosition.y;
	return rect;
}


int Entity::GetType() const
{
	return ENT_BASE;
}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Entity::HandleCollision( const IEntity* pOther )
{
	/* DO NOTHING */
	(void)pOther;		// unused parameter
}


/**************************************************************/
// AddRef
//	- increase the reference count
/*virtual*/ void Entity::AddRef( void )
{
	assert( m_unRefCount != 0xFFFFFFFF && "Entity::AddRef - maximum reference count has been exceeded" );

	++m_unRefCount;
}


/**************************************************************/
// Release
//	- decrease the reference count
//	- self-destruct when the count is 0
/*virtual*/ void Entity::Release( void )
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}


/**********************************************************/
// Accessors

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}

SGD::Point Entity::GetPosition() const
{
	return m_ptPosition;
}

SGD::Vector Entity::GetVelocity() const
{
	return m_vtVelocity;
}

/**********************************************************/
// Mutators

void Entity::SetSprite(Sprite* _sprite)
{
	m_pSprite = _sprite;
}

void Entity::SetPosition(SGD::Point _position)
{
	m_ptPosition = _position;
}

void Entity::SetVelocity(SGD::Vector _velocity)
{
	m_vtVelocity = _velocity;
}