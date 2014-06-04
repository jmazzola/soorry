#include "Tower.h"

#include "EntityManager.h"
#include "GameplayState.h"
#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


Tower::Tower()
{
	m_pEntityManager = GameplayState::GetInstance()->GetEntityManager();
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

	if (m_bSelected)
	{
		SGD::Rectangle rect = GetRect();
		rect.left -= Camera::x;
		rect.right -= Camera::x;
		rect.top -= Camera::y;
		rect.bottom -= Camera::y;

		pGraphics->DrawRectangle(rect, SGD::Color(0, 0, 0, 0), SGD::Color(255, 255, 0), 2);
	}
}

SGD::Rectangle Tower::GetRect() const
{
	SGD::Rectangle rect;

	rect.left = m_ptPosition.x;
	rect.top = m_ptPosition.y;
	rect.right = rect.left + 32;
	rect.bottom = rect.top + 32;

	return rect;
}

int Tower::GetType() const
{
	return ENT_TOWER;
}

/**********************************************************/
// Accessors

bool Tower::IsSelected() const
{
	return m_bSelected;
}

/**********************************************************/
// Mutators

void Tower::SetSelected(bool _selected)
{
	m_bSelected = _selected;
}

void Tower::SetBaseImage(SGD::HTexture _baseImage)
{
	m_hBaseImage = _baseImage;
}

void Tower::SetGunImage(SGD::HTexture _gunImage)
{
	m_hGunImage = _gunImage;
}
