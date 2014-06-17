#include "HockeyStickTower.h"

#include "Camera.h"
#include "EntityManager.h"
#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "BitmapFont.h"
#include "Game.h"

#include "CreateParticleMessage.h"


HockeyStickTower::HockeyStickTower()
{
	m_fRotationRate = 12.0f;
	m_fRadius = 48.0f;
	m_fDamage = 5.0f;
}


HockeyStickTower::~HockeyStickTower()
{
}


/**********************************************************/
// Interface Methods

void HockeyStickTower::Update(float dt)
{
	// Spin the stick
	m_fRotation += m_fRotationRate * dt;

	// Update hit list
	for (unsigned int i = 0; i < m_vEnemiesHit.size(); i++)
	{
		m_vEnemiesHit[i].angleRemaining -= m_fRotationRate * dt;
		if (m_vEnemiesHit[i].angleRemaining <= 0.0f)
		{
			m_vEnemiesHit.erase(m_vEnemiesHit.begin() + i);
			i--;
		}
	}

	SGD::Vector heading = SGD::Vector(-1, 0);
	heading.Rotate(m_fRotation);

	// Attack enemies
	const vector<IEntity*> enemies = m_pEntityManager->GetBucket(2);

	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		// Make sure it's not on the hit list
		if (CheckHitList(enemies[i]))
			continue;

		// Compute vector
		SGD::Vector distanceVector = enemies[i]->GetRect().ComputeCenter() - (m_ptPosition + SGD::Vector(16, 16));
		float distance = distanceVector.ComputeLength();

		// Check if within radius
		if (distance < m_fRadius)
		{
			// Check if we hit it
			distanceVector.Normalize();

			if (heading.ComputeAngle(distanceVector) < 0.2 || heading.ComputeAngle(distanceVector) > -0.2)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(enemies[i]);

				// Add to the hit list
				m_vEnemiesHit.push_back(EnemyHit(enemy));

				enemy->SetCurrHealth(enemy->GetCurrHealth() - m_fDamage);

				SGD::Vector spurtVector = { 0, -1 };
				spurtVector.Rotate(m_fRotation);
				spurtVector *= 300;

				CreateParticleMessage* msg = new CreateParticleMessage("Blood_Spurt1", enemy, spurtVector, 16, 16);
				msg->QueueMessage();

				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_pTowerFlyweight->GetHockeyStickSlashSound()))
					SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetHockeyStickSlashSound());
			}
		}
	}

	Tower::Update(dt);
}

void HockeyStickTower::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render base image
	pGraphics->DrawTexture(m_hBaseImage, SGD::Point(m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y));

	Tower::Render();
}

void HockeyStickTower::PostRender()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	// Render gun
	pGraphics->DrawTexture(m_hGunImage, SGD::Point(m_ptPosition.x - Camera::x - 16, m_ptPosition.y - Camera::y - 16), m_fRotation, SGD::Vector(32.0f, 32.0f));

	Tower::PostRender();
}

void HockeyStickTower::DrawMenu()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Point pos = SGD::Point(m_ptPosition.x - 96 - Camera::x, m_ptPosition.y - 128 - Camera::y);

	string damage = "Damage: " + std::to_string(m_pTowerFlyweight->GetHockeyStickDamage(m_nUpgradeOne));

	while (damage.size() > 13)
		damage.erase(damage.begin() + 13);

	pFont->Draw(damage, (int)pos.x + 8, (int)pos.y + 82, 0.35f, SGD::Color(255, 255, 255));

	string fireRate = "Spin Rate: " + std::to_string(m_pTowerFlyweight->GetHockeyStickSpinRate(m_nUpgradeTwo));

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
		pFont->Draw("Damage", mid - (int)(pFont->GetTextWidth("Damage") * scale / 2), (int)upgradeOneRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetHockeyStickDamageUpgradeCost(m_nUpgradeOne));
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
		pFont->Draw("Spin Rate", mid2 - (int)(pFont->GetTextWidth("Spin Rate") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
		string cost = std::to_string(m_pTowerFlyweight->GetHockeyStickSpinRateUpgradeCost(m_nUpgradeTwo));
		pFont->Draw(cost, mid2 - (int)(pFont->GetTextWidth(cost) * scale / 2), (int)upgradeTwoRect.top + 39, scale, SGD::Color(255, 255, 255));
	}

	else
		pFont->Draw("MAXED OUT!", mid2 - (int)(pFont->GetTextWidth("MAXED OUT!") * scale / 2), (int)upgradeTwoRect.top + 21, scale, SGD::Color(255, 255, 255));
}

int HockeyStickTower::GetType() const
{
	return ENT_TOWER_HOCKEY_STICK;
}

void HockeyStickTower::Upgrade(int _slot, unsigned int* _points)
{
	if (_slot == 0 && *_points >= (unsigned int)m_pTowerFlyweight->GetHockeyStickDamageUpgradeCost(m_nUpgradeOne) && m_nUpgradeOne < 3)
	{
		*_points -= m_pTowerFlyweight->GetHockeyStickDamageUpgradeCost(m_nUpgradeOne);
		m_nSellValue += (int)(m_pTowerFlyweight->GetHockeyStickDamageUpgradeCost(m_nUpgradeOne) * 0.75f);
		m_nUpgradeOne++;
		m_fDamage = m_pTowerFlyweight->GetHockeyStickDamage(m_nUpgradeOne);

		SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetPurchaseSound());
	}

	else if (_slot == 1 && *_points >= (unsigned int)m_pTowerFlyweight->GetHockeyStickSpinRateUpgradeCost(m_nUpgradeTwo) && m_nUpgradeTwo < 3)
	{
		*_points -= m_pTowerFlyweight->GetHockeyStickSpinRateUpgradeCost(m_nUpgradeTwo);
		m_nSellValue += (int)(m_pTowerFlyweight->GetHockeyStickSpinRateUpgradeCost(m_nUpgradeTwo) * 0.75f);
		m_nUpgradeTwo++;
		m_fRotationRate = m_pTowerFlyweight->GetHockeyStickSpinRate(m_nUpgradeTwo);

		SGD::AudioManager::GetInstance()->PlayAudio(m_pTowerFlyweight->GetPurchaseSound());
	}
}

/**********************************************************/
// Helper functions

bool HockeyStickTower::CheckHitList(IEntity* _enemy) const
{
	for (unsigned int enemy = 0; enemy < m_vEnemiesHit.size(); enemy++)
	{
		if (m_vEnemiesHit[enemy].enemy == _enemy)
			return true;
	}

	return false;
}

/**********************************************************/
// Mutators

void HockeyStickTower::SetRotationRate(float _rotationRate)
{
	m_fRotationRate = _rotationRate;
}

void HockeyStickTower::SetDamage(float _damage)
{
	m_fDamage = _damage;
}

void HockeyStickTower::ExternalUpgrade()
{
	m_fDamage = m_pTowerFlyweight->GetHockeyStickDamage(m_nUpgradeOne);
	m_fRotationRate = m_pTowerFlyweight->GetHockeyStickSpinRate(m_nUpgradeTwo);
}