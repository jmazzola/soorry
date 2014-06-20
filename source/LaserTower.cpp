#include "LaserTower.h"

#include "Camera.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "BitmapFont.h"
#include "Game.h"
#include "GameplayState.h"


LaserTower::LaserTower() : Listener(this)
{
	RegisterForEvent("ASSESS_LASERS");
}


LaserTower::~LaserTower()
{
	SGD::Event* pEvent = new SGD::Event("ASSESS_LASERS");
	pEvent->QueueEvent();
}


/**********************************************************/
// Interface Methods

void LaserTower::Update(float dt)
{
	for (unsigned int i = 0; i < m_vConnectedTowers.size(); i++)
	{
		SGD::Point connectedPos = m_vConnectedTowers[i]->m_ptPosition;

		SGD::Rectangle rect;

		if (m_ptPosition.x < connectedPos.x)
		{
			rect.left = m_ptPosition.x + 16;
			rect.right = connectedPos.x + 16;
		}
		else if (m_ptPosition.x > connectedPos.x)
		{
			rect.left = connectedPos.x + 16;
			rect.right = m_ptPosition.x + 16;
		}
		else
		{
			rect.left = m_ptPosition.x + 14;
			rect.right = m_ptPosition.x + 18;
		}

		if (m_ptPosition.y < connectedPos.y)
		{
			rect.top = m_ptPosition.y + 16;
			rect.bottom = connectedPos.y + 16;
		}
		else if (m_ptPosition.y > connectedPos.y)
		{
			rect.top = connectedPos.y + 16;
			rect.bottom = m_ptPosition.y + 16;
		}
		else
		{
			rect.top = m_ptPosition.y + 14;
			rect.bottom = m_ptPosition.y + 18;
		}

		Enemy* enemy = dynamic_cast<Enemy*>(m_pEntityManager->CheckCollision(rect, 2));

		if (enemy)
		{
			enemy->SetCurrHealth(enemy->GetCurrHealth() - m_nDamage * dt);
			GameplayState::GetInstance()->m_bPlayLaser = true;
		}
	}

	Tower::Update(dt);
}

void LaserTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void LaserTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	Tower::PostRender();

	for (unsigned int i = 0; i < m_vConnectedTowers.size(); i++)
	{
		pGraphics->DrawLine(m_ptPosition + SGD::Vector((float)(16 - Camera::x), (float)(16 - Camera::y)), m_vConnectedTowers[i]->GetPosition() + SGD::Vector((float)(16 - Camera::x), (float)(16 - Camera::y)),
			SGD::Color(100, 255, 0, 0), 4);
	}

	//pGraphics->DrawString(std::to_string(m_vConnectedTowers.size()).c_str(), m_ptPosition - SGD::Vector(Camera::x, Camera::y));
}

void LaserTower::DrawMenu()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Point pos = SGD::Point(m_ptPosition.x - 96 - Camera::x, m_ptPosition.y - 128 - Camera::y);

	string damage = "Damage: " + std::to_string(m_pTowerFlyweight->GetLaserDamage(m_nUpgradeOne));

	pFont->Draw(damage, (int)pos.x + 8, (int)pos.y + 82, 0.35f, SGD::Color(255, 255, 255));

	string fireRate = "Range: " + std::to_string(m_pTowerFlyweight->GetLaserRange(m_nUpgradeTwo));

	pFont->Draw(fireRate, (int)pos.x + 8, (int)pos.y + 100, 0.35f, SGD::Color(255, 255, 255));

	// Power Upgrade
	SGD::Rectangle upgradeOneRect;
	upgradeOneRect.left = pos.x + 8;
	upgradeOneRect.top = pos.y + 8;
	upgradeOneRect.right = pos.x + 108;
	upgradeOneRect.bottom = pos.y + 72;

	if (pInput->GetMousePosition().IsWithinRectangle(upgradeOneRect))
		pGraphics->DrawRectangle(upgradeOneRect, SGD::Color(100, 0, 255, 0), SGD::Color(255, 255, 255), 2);

	else
		pGraphics->DrawRectangle(upgradeOneRect, SGD::Color(0, 0, 0, 0), SGD::Color(255, 255, 255), 2);

	int mid = (int)((upgradeOneRect.left + upgradeOneRect.right) / 2.0f);
	const float scale = 0.35f;
	if (m_nUpgradeOne < 3)
	{
		pFont->Draw("Upgrade", mid - (int)(pFont->GetTextWidth("Upgrade") * scale / 2), (int)upgradeOneRect.top + 4, scale, SGD::Color(255, 255, 255));
		pFont->Draw("Damage", mid - (int)(pFont->GetTextWidth("Damage") * scale / 2), (int)upgradeOneRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetLaserDamageUpgradeCost(m_nUpgradeOne));
		pFont->Draw(cost, mid - (int)(pFont->GetTextWidth(cost) * scale / 2), (int)upgradeOneRect.top + 39, scale, SGD::Color(255, 255, 255));
	}

	else
		pFont->Draw("MAXED OUT!", mid - (int)(pFont->GetTextWidth("MAXED OUT!") * scale / 2), (int)upgradeOneRect.top + 21, scale, SGD::Color(255, 255, 255));

	// Range Upgrade
	SGD::Rectangle upgradeTwoRect;
	upgradeTwoRect.left = pos.x + 116;
	upgradeTwoRect.top = pos.y + 8;
	upgradeTwoRect.right = pos.x + 216;
	upgradeTwoRect.bottom = pos.y + 72;

	if (pInput->GetMousePosition().IsWithinRectangle(upgradeTwoRect))
		pGraphics->DrawRectangle(upgradeTwoRect, SGD::Color(100, 0, 255, 0), SGD::Color(255, 255, 255), 2);

	else
		pGraphics->DrawRectangle(upgradeTwoRect, SGD::Color(0, 0, 0, 0), SGD::Color(255, 255, 255), 2);

	int mid2 = (int)((upgradeTwoRect.left + upgradeTwoRect.right) / 2.0f);
	if (m_nUpgradeTwo < 3)
	{

		pFont->Draw("Upgrade", mid2 - (int)(pFont->GetTextWidth("Upgrade") * scale / 2), (int)upgradeTwoRect.top + 4, scale, SGD::Color(255, 255, 255));
		pFont->Draw("Range", mid2 - (int)(pFont->GetTextWidth("Range") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetLaserRangeUpgradeCost(m_nUpgradeTwo));
		pFont->Draw(cost, mid2 - (int)(pFont->GetTextWidth(cost) * scale / 2), (int)upgradeTwoRect.top + 39, scale, SGD::Color(255, 255, 255));
	}

	else
		pFont->Draw("MAXED OUT!", mid2 - (int)(pFont->GetTextWidth("MAXED OUT!") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
}

int LaserTower::GetType() const
{
	return ENT_TOWER_LASER;
}

void LaserTower::Upgrade(int _slot, unsigned int* _points)
{
	if (_slot == 0)
	{
		if (*_points >= (unsigned int)m_pTowerFlyweight->GetLaserDamageUpgradeCost(m_nUpgradeOne) && m_nUpgradeOne < 3)
		{
			*_points -= m_pTowerFlyweight->GetLaserDamageUpgradeCost(m_nUpgradeOne);
			m_nSellValue += (int)(m_pTowerFlyweight->GetLaserDamageUpgradeCost(m_nUpgradeOne) * 0.75f);
			m_nUpgradeOne++;
			m_nDamage = m_pTowerFlyweight->GetLaserDamage(m_nUpgradeOne);

			SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetPurchaseSound());
		}

		else
			SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
	}

	else if (_slot == 1)
	{
		if (*_points >= (unsigned int)m_pTowerFlyweight->GetLaserRangeUpgradeCost(m_nUpgradeTwo) && m_nUpgradeTwo < 3)
		{
			*_points -= m_pTowerFlyweight->GetLaserRangeUpgradeCost(m_nUpgradeTwo);
			m_nSellValue += (int)(m_pTowerFlyweight->GetLaserRangeUpgradeCost(m_nUpgradeTwo) * 0.75f);
			m_nUpgradeTwo++;
			m_nRange = m_pTowerFlyweight->GetLaserRange(m_nUpgradeTwo);

			SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetPurchaseSound());

			SGD::Event* pEvent = new SGD::Event("ASSESS_LASERS");
			pEvent->QueueEvent();
		}

		else
			SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetInvalidSound());
	}
}

void LaserTower::HandleEvent(const SGD::Event* _event)
{
	if (_event->GetEventID() == "ASSESS_LASERS")
	{
		m_vConnectedTowers.clear();

		// Right
		for (int x = 0; x < m_nRange; x++)
		{
			SGD::Rectangle rect;
			rect.left = m_ptPosition.x + 32 * x + 34;
			rect.top = m_ptPosition.y + 2;
			rect.right = rect.left + 28;
			rect.bottom = rect.top + 28;

			LaserTower* laserTower = dynamic_cast<LaserTower*>(m_pEntityManager->CheckCollision(rect, 3));

			if (laserTower)
			{
				m_vConnectedTowers.push_back(laserTower);
				break;
			}
		}

		// Left
		for (int x = 0; x < m_nRange; x++)
		{
			SGD::Rectangle rect;
			rect.left = m_ptPosition.x - 32 * x - 30;
			rect.top = m_ptPosition.y + 2;
			rect.right = rect.left + 28;
			rect.bottom = rect.top + 28;

			LaserTower* laserTower = dynamic_cast<LaserTower*>(m_pEntityManager->CheckCollision(rect, 3));

			if (laserTower)
			{
				m_vConnectedTowers.push_back(laserTower);
				break;
			}
		}

		// Up
		for (int y = 0; y < m_nRange; y++)
		{
			SGD::Rectangle rect;
			rect.left = m_ptPosition.x + 2;
			rect.top = m_ptPosition.y - 32 * y - 30;
			rect.right = rect.left + 28;
			rect.bottom = rect.top + 28;

			LaserTower* laserTower = dynamic_cast<LaserTower*>(m_pEntityManager->CheckCollision(rect, 3));

			if (laserTower)
			{
				m_vConnectedTowers.push_back(laserTower);
				break;
			}
		}

		// Down
		for (int y = 0; y < m_nRange; y++)
		{
			SGD::Rectangle rect;
			rect.left = m_ptPosition.x + 2;
			rect.top = m_ptPosition.y + 32 * y + 34;
			rect.right = rect.left + 28;
			rect.bottom = rect.top + 28;

			LaserTower* laserTower = dynamic_cast<LaserTower*>(m_pEntityManager->CheckCollision(rect, 3));

			if (laserTower)
			{
				m_vConnectedTowers.push_back(laserTower);
				break;
			}
		}
	}
}

/**********************************************************/
// Accessors

int LaserTower::GetDamage() const
{
	return m_nDamage;
}

int LaserTower::GetRange() const
{
	return m_nRange;
}

/**********************************************************/
// Mutators

void LaserTower::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void LaserTower::SetRange(int _range)
{
	m_nRange = _range;
}

void LaserTower::ExternalUpgrade()
{
	m_nDamage = m_pTowerFlyweight->GetLaserDamage(m_nUpgradeOne);
	m_nRange = m_pTowerFlyweight->GetLaserRange(m_nUpgradeTwo);
}
