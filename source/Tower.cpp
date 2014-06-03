#include "Tower.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"


Tower::Tower()
{
}


Tower::~Tower()
{
}


/**********************************************************/
// Interface Methods
void Tower::Update(float dt)
{

}

void Tower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));
}

SGD::Rectangle Tower::GetRect() const
{
	SGD::Rectangle rect;

	rect.left = m_ptPosition.x;
	rect.top = m_ptPosition.y;
	rect.right = rect.left + 32;
	rect.bottom = rect.right + 32;

	return rect;
}

int Tower::GetType() const
{
	return ENT_TOWER;
}

/**********************************************************/
// Mutators

void Tower::SetBaseImage(SGD::HTexture _baseImage)
{
	m_hBaseImage = _baseImage;
}

void Tower::SetGunImage(SGD::HTexture _gunImage)
{
	m_hGunImage = _gunImage;
}
