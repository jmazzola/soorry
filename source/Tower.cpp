#include "Tower.h"

#include "Game.h"
#include "EntityManager.h"
#include "GameplayState.h"
#include "Camera.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"


Tower::Tower()
{
	m_pEntityManager = GameplayState::GetInstance()->GetEntityManager();

	m_nUpgradeOne = 0;
	m_nUpgradeTwo = 0;

	m_bHoverOne = false;
	m_bHoverTwo = false;
	m_bHoverSell = false;
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

	SGD::Rectangle rect = GetRect();
	rect.left -= Camera::x;
	rect.right -= Camera::x;
	rect.top -= Camera::y;
	rect.bottom -= Camera::y;

	Game* pGame = Game::GetInstance();

	// -- Debugging --
	if (pGame->IsShowingRects())
		pGraphics->DrawRectangle(rect, SGD::Color(128, 0, 0, 255));

	if (m_bSelected)
		pGraphics->DrawRectangle(rect, SGD::Color(0, 0, 0, 0), SGD::Color(255, 255, 0), 2);
}

void Tower::PostRender()
{
	
}

void Tower::DrawMenu()
{

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

void Tower::Upgrade(int _slot, unsigned int* _points)
{

}

/**********************************************************/
// Accessors

bool Tower::IsSelected() const
{
	return m_bSelected;
}

int Tower::GetUpgradeOne() const
{
	return m_nUpgradeOne;
}

int Tower::GetUpgradeTwo() const
{
	return m_nUpgradeTwo;
}

int Tower::GetSellValue() const
{
	return m_nSellValue;
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

void Tower::SetTowerFlyweight(TowerFlyweight* _towerFlyweight)
{
	m_pTowerFlyweight = _towerFlyweight;
}

void Tower::SetUpgradeOne(int _tier)
{
	m_nUpgradeOne = _tier;
	this->ExternalUpgrade();
}

void Tower::SetUpgradeTwo(int _tier)
{
	m_nUpgradeTwo = _tier;
	this->ExternalUpgrade();
}

void Tower::SetSellValue(int _sellValue)
{
	m_nSellValue = _sellValue;
}

void Tower::ExternalUpgrade()
{

}