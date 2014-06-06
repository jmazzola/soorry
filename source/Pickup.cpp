#include "Pickup.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnimationManager.h"
#include "Sprite.h"
#include "Camera.h"

Pickup::Pickup()
{
}


Pickup::~Pickup()
{
}

/*virtual*/ void Pickup::Update(float dt)
{
	m_fRotation += dt;
	if(m_fRotation > 2 * SGD::PI)
		m_fRotation = 0.0f;
}
/*virtual*/ int Pickup::GetType() const
{
	return ENT_PICKUP;
}

void Pickup::Render( void )
{
	// Verify the image
	//assert( m_hImage != SGD::INVALID_HANDLE && "Entity::Render - image was not set!" );
	
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	
	// Get the current frame
	int frame = Entity::GetSprite()->GetCurrFrame();

	// Find the center of the image
	SGD::Vector center;
	center.x = Entity::GetSprite()->GetFrame(frame).GetFrameRect().right - Entity::GetSprite()->GetFrame(frame).GetFrameRect().left;
	center.y = Entity::GetSprite()->GetFrame(frame).GetFrameRect().bottom - Entity::GetSprite()->GetFrame(frame).GetFrameRect().top;
	center.x /= 2;
	center.y /= 2;
	
	// Render
	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y, m_fRotation, center);

	// Why is this here?
	SGD::Rectangle drawRect = GetRect();
	drawRect.left -= Camera::x;
	drawRect.right -= Camera::x;
	drawRect.top -= Camera::y;
	drawRect.bottom -= Camera::y;
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(drawRect, SGD::Color(100, 0, 0, 0));

	// HACK: Modify the rotation
	//m_fRotation += 0.01f;

	// Draw the image
}
