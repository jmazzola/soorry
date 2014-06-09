#include "MapleSyrupTower.h"

#include "Camera.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "CreateMapleSyrupBulletMessage.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "BitmapFont.h"
#include "Game.h"


MapleSyrupTower::MapleSyrupTower()
{
	m_fSlowTime = 3.0f;
	m_fFireRate = 1.2f;
	m_fBulletSpeed = 250.0f;
	m_nRange = 96;

	m_fNextShotTimer = 0.0f;
}


MapleSyrupTower::~MapleSyrupTower()
{
}


/**********************************************************/
// Interface Methods

void MapleSyrupTower::Update(float dt)
{
	// Update timers
	m_fNextShotTimer -= dt;

	// Aim at enemy
	Enemy* enemy = dynamic_cast<Enemy*>(m_pEntityManager->CheckCollision(SGD::Point(m_ptPosition.x + 16.0f, m_ptPosition.y + 16.0f), (float)m_nRange, 2));
	SGD::Vector toEnemy;
	if (enemy)
	{
		SGD::Vector orientation = { 0, -1 };
		toEnemy = enemy->GetPosition() - m_ptPosition;
		float distance = toEnemy.ComputeLength();
		toEnemy.Normalize();

		// Create bullet's velocity
		SGD::Vector bulletVelocity;

		// Get estimated bullet travel time
		float bulletTime = distance / m_fBulletSpeed;

		// Get estimated enemy position
		SGD::Point estimatedPos = enemy->GetPosition() + enemy->GetVelocity() * bulletTime;

		bulletVelocity = estimatedPos - m_ptPosition;
		bulletVelocity.Normalize();

		if (orientation.ComputeSteering(bulletVelocity) > 0)
			m_fRotation = orientation.ComputeAngle(bulletVelocity);
		else
			m_fRotation = -orientation.ComputeAngle(bulletVelocity);

		// Shoot, if time
		if (m_fNextShotTimer <= 0.0f)
		{
			bulletVelocity *= m_fBulletSpeed;
			SGD::Vector offset = bulletVelocity.ComputeNormalized() * 16;

			CreateMapleSyrupBulletMessage* msg = new CreateMapleSyrupBulletMessage((int)(m_ptPosition.x + 8 + offset.x), (int)(m_ptPosition.y + 8 + offset.y), bulletVelocity, m_fSlowTime);
			msg->QueueMessage();

			// Reset timer
			m_fNextShotTimer = m_fFireRate;
		}
	}
}

void MapleSyrupTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	if (m_bSelected)
	{
		SGD::Point pos = m_ptPosition;
		pos.x -= Camera::x;
		pos.y -= Camera::y;
		float scale = m_nRange / 128.0f;
		pGraphics->DrawTextureSection(m_pTowerFlyweight->GetRangeCirclesImage(), SGD::Point(pos.x + 16 - scale * 128.0f, pos.y + 16 - scale * 128.0f), SGD::Rectangle(0, 0, 128, 128),
			0.0f, {}, {}, SGD::Size(scale * 2, scale * 2));
	}

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void MapleSyrupTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));

	Tower::PostRender();
}

void MapleSyrupTower::DrawMenu()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Point pos = SGD::Point(m_ptPosition.x - 96 - Camera::x, m_ptPosition.y - 128 - Camera::y);

	string damage = "Duration: " + std::to_string(m_pTowerFlyweight->GetMapleSyrupEffectDuration(m_nUpgradeOne));

	while (damage.size() > 14)
		damage.erase(damage.begin() + 14);

	pFont->Draw(damage, (int)pos.x + 8, (int)pos.y + 82, 0.35f, SGD::Color(255, 255, 255));

	string fireRate = "Fire Rate: " + std::to_string(1 / m_pTowerFlyweight->GetMapleSyrupFireRate(m_nUpgradeOne));

	while (fireRate.size() > 15)
		fireRate.erase(fireRate.begin() + 15);

	fireRate += "/s";

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
		pFont->Draw("Effect Duration", mid - (int)(pFont->GetTextWidth("Effect Duration") * scale / 2), (int)upgradeOneRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetMapleSyrupPowerUpgradeCost(m_nUpgradeOne));
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
		string cost = std::to_string(m_pTowerFlyweight->GetMapleSyrupRangeUpgradeCost(m_nUpgradeTwo));
		pFont->Draw(cost, mid2 - (int)(pFont->GetTextWidth(cost) * scale / 2), (int)upgradeTwoRect.top + 39, scale, SGD::Color(255, 255, 255));
	}

	else
		pFont->Draw("MAXED OUT!", mid2 - (int)(pFont->GetTextWidth("MAXED OUT!") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
}

int MapleSyrupTower::GetType() const
{
	return ENT_TOWER_MAPLE_SYRUP;
}

void MapleSyrupTower::Upgrade(int _slot, unsigned int* _points)
{
	if (_slot == 0 && *_points >= (unsigned int)m_pTowerFlyweight->GetMapleSyrupPowerUpgradeCost(m_nUpgradeOne) && m_nUpgradeOne < 3)
	{
		*_points -= m_pTowerFlyweight->GetMapleSyrupPowerUpgradeCost(m_nUpgradeOne);
		m_nSellValue += (int)(m_pTowerFlyweight->GetMapleSyrupPowerUpgradeCost(m_nUpgradeOne) * 0.75f);
		m_nUpgradeOne++;
		m_fSlowTime = m_pTowerFlyweight->GetMapleSyrupEffectDuration(m_nUpgradeOne);
		m_fFireRate = m_pTowerFlyweight->GetMapleSyrupFireRate(m_nUpgradeOne);
	}

	else if (_slot == 1 && *_points >= (unsigned int)m_pTowerFlyweight->GetMapleSyrupRangeUpgradeCost(m_nUpgradeTwo) && m_nUpgradeTwo < 3)
	{
		*_points -= m_pTowerFlyweight->GetMapleSyrupRangeUpgradeCost(m_nUpgradeTwo);
		m_nSellValue += (int)(m_pTowerFlyweight->GetMapleSyrupRangeUpgradeCost(m_nUpgradeTwo) * 0.75f);
		m_nUpgradeTwo++;
		m_nRange = m_pTowerFlyweight->GetMapleSyrupRange(m_nUpgradeTwo);
	}
}

/**********************************************************/
// Accessors

float MapleSyrupTower::GetSlowTime() const
{
	return m_fSlowTime;
}

float MapleSyrupTower::GetFireRate() const
{
	return m_fFireRate;
}

int MapleSyrupTower::GetRange() const
{
	return m_nRange;
}

/**********************************************************/
// Mutators

void MapleSyrupTower::SetSlowTime(float _slowTime)
{
	m_fSlowTime = _slowTime;
}

void MapleSyrupTower::SetFireRate(float _fireRate)
{
	m_fFireRate = _fireRate;
}

void MapleSyrupTower::SetRange(int _range)
{
	m_nRange = _range;
}
