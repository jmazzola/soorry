#include "MachineGunTower.h"

#include "Camera.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "CreateMachineGunBulletMessage.h"
#include "Game.h"
#include "BitmapFont.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"


MachineGunTower::MachineGunTower()
{
	m_nDamage = 25;
	m_fFireRate = 0.5f;
	m_fBulletSpeed = 750.0f;
	m_nRange = 128;

	m_fNextShotTimer = 0.0f;
}


MachineGunTower::~MachineGunTower()
{
}


/**********************************************************/
// Interface Methods

void MachineGunTower::Update(float dt)
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

			CreateMachineGunBulletMessage* msg = new CreateMachineGunBulletMessage((int)(m_ptPosition.x + 16 + offset.x), (int)(m_ptPosition.y + 16 + offset.y), bulletVelocity, m_nDamage);
			msg->QueueMessage();

			// Reset timer
			m_fNextShotTimer = m_fFireRate;
		}
	}
}

void MachineGunTower::Render()
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

void MachineGunTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));

	Tower::PostRender();
}

void MachineGunTower::DrawMenu()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Point pos = SGD::Point(m_ptPosition.x - 96 - Camera::x, m_ptPosition.y - 128 - Camera::y);

	string damage = "Damage: " + std::to_string(m_pTowerFlyweight->GetMachineGunDamage(m_nUpgradeOne));
	pFont->Draw(damage, (int)pos.x + 8, (int)pos.y + 82, 0.35f, SGD::Color(255, 255, 255));

	string fireRate = "Fire Rate: " + std::to_string(1 / m_pTowerFlyweight->GetMachineGunFireRate(m_nUpgradeOne));
	
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
		pFont->Draw("Power", mid - (int)(pFont->GetTextWidth("Power") * scale / 2), (int)upgradeOneRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetMachineGunPowerUpgradeCost(m_nUpgradeOne));
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
		string cost = std::to_string(m_pTowerFlyweight->GetMachineGunRangeUpgradeCost(m_nUpgradeTwo));
		pFont->Draw(cost, mid2 - (int)(pFont->GetTextWidth(cost) * scale / 2), (int)upgradeTwoRect.top + 39, scale, SGD::Color(255, 255, 255));
	}

	else
		pFont->Draw("MAXED OUT!", mid2 - (int)(pFont->GetTextWidth("MAXED OUT!") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
}

int MachineGunTower::GetType() const
{
	return ENT_TOWER_MACHINE_GUN;
}

void MachineGunTower::Upgrade(int _slot, unsigned int* _points)
{
	if (_slot == 0 && *_points >= (unsigned int)m_pTowerFlyweight->GetMachineGunPowerUpgradeCost(m_nUpgradeOne) && m_nUpgradeOne < 3)
	{
		*_points -= m_pTowerFlyweight->GetMachineGunPowerUpgradeCost(m_nUpgradeOne);
		m_nSellValue += (int)(m_pTowerFlyweight->GetMachineGunPowerUpgradeCost(m_nUpgradeOne) * 0.75f);
		m_nUpgradeOne++;
		m_nDamage = m_pTowerFlyweight->GetMachineGunDamage(m_nUpgradeOne);
		m_fFireRate = m_pTowerFlyweight->GetMachineGunFireRate(m_nUpgradeOne);
	}

	else if (_slot == 1 && *_points >= (unsigned int)m_pTowerFlyweight->GetMachineGunRangeUpgradeCost(m_nUpgradeTwo) && m_nUpgradeTwo < 3)
	{
		*_points -= m_pTowerFlyweight->GetMachineGunRangeUpgradeCost(m_nUpgradeTwo);
		m_nSellValue += (int)(m_pTowerFlyweight->GetMachineGunRangeUpgradeCost(m_nUpgradeTwo) * 0.75f);
		m_nUpgradeTwo++;
		m_nRange = m_pTowerFlyweight->GetMachineGunRange(m_nUpgradeTwo);
	}
}

/**********************************************************/
// Accessors

int MachineGunTower::GetDamage() const
{
	return m_nDamage;
}

float MachineGunTower::GetFireRate() const
{
	return m_fFireRate;
}

int MachineGunTower::GetRange() const
{
	return m_nRange;
}

/**********************************************************/
// Mutators

void MachineGunTower::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void MachineGunTower::SetFireRate(float _fireRate)
{
	m_fFireRate = _fireRate;
}

void MachineGunTower::SetRange(int _range)
{
	m_nRange = _range;
}
