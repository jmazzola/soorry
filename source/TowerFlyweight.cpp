#include "TowerFlyweight.h"

#include "../SGD Wrappers/SGD_AudioManager.h"

#include "../TinyXML/tinyxml.h"


TowerFlyweight::TowerFlyweight()
{
}


TowerFlyweight::~TowerFlyweight()
{
}


/**********************************************************/
// Interface Methods

void TowerFlyweight::Load(string fileName)
{
	// Create a TinyXML document
	TiXmlDocument doc;

	// Attempt to load from the file
	doc.LoadFile(fileName.c_str());

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Double for temp storage
	double temp;

#pragma region Machine Gun Tower

	// Machine Gun Tower
	TiXmlElement* machineGun = pRoot->FirstChildElement("machine_gun");

	// Starting Values
	machineGun->FirstChildElement("damage")->Attribute("value", &m_nMachineGunDamage[0]);
	machineGun->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMachineGunFireRate[0] = (float)temp;
	machineGun->FirstChildElement("range")->Attribute("value", &m_nMachineGunRange[0]);

	// Power Upgrade Values
	TiXmlElement* machineGunPower = machineGun->FirstChildElement("power_upgrade");

	// Tier 1
	TiXmlElement* machineGunPowerOne = machineGunPower->FirstChildElement("tier_one");
	machineGunPowerOne->FirstChildElement("cost")->Attribute("value", &m_nMachineGunPowerUpgradeCost[0]);
	machineGunPowerOne->FirstChildElement("damage")->Attribute("value", &m_nMachineGunDamage[1]);
	machineGunPowerOne->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMachineGunFireRate[1] = (float)temp;

	// Tier 2
	TiXmlElement* machineGunPowerTwo = machineGunPower->FirstChildElement("tier_two");
	machineGunPowerTwo->FirstChildElement("cost")->Attribute("value", &m_nMachineGunPowerUpgradeCost[1]);
	machineGunPowerTwo->FirstChildElement("damage")->Attribute("value", &m_nMachineGunDamage[2]);
	machineGunPowerTwo->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMachineGunFireRate[2] = (float)temp;

	// Tier 3
	TiXmlElement* machineGunPowerThree = machineGunPower->FirstChildElement("tier_three");
	machineGunPowerThree->FirstChildElement("cost")->Attribute("value", &m_nMachineGunPowerUpgradeCost[2]);
	machineGunPowerThree->FirstChildElement("damage")->Attribute("value", &m_nMachineGunDamage[3]);
	machineGunPowerThree->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMachineGunFireRate[3] = (float)temp;

	// Range Upgrade Values
	TiXmlElement* machineGunRange = machineGun->FirstChildElement("range_upgrade");

	// Tier 1
	TiXmlElement* machineGunRangeOne = machineGunRange->FirstChildElement("tier_one");
	machineGunRangeOne->FirstChildElement("cost")->Attribute("value", &m_nMachineGunRangeUpgradeCost[0]);
	machineGunRangeOne->FirstChildElement("range")->Attribute("value", &m_nMachineGunRange[1]);

	// Tier 2
	TiXmlElement* machineGunRangeTwo = machineGunRange->FirstChildElement("tier_two");
	machineGunRangeTwo->FirstChildElement("cost")->Attribute("value", &m_nMachineGunRangeUpgradeCost[1]);
	machineGunRangeTwo->FirstChildElement("range")->Attribute("value", &m_nMachineGunRange[2]);

	// Tier 3
	TiXmlElement* machineGunRangeThree = machineGunRange->FirstChildElement("tier_three");
	machineGunRangeThree->FirstChildElement("cost")->Attribute("value", &m_nMachineGunRangeUpgradeCost[2]);
	machineGunRangeThree->FirstChildElement("range")->Attribute("value", &m_nMachineGunRange[3]);

#pragma endregion

#pragma region Maple Syrup Tower

	// Maple Syrup Tower
	TiXmlElement* mapleSyrup = pRoot->FirstChildElement("maple_syrup");

	// Starting Values
	mapleSyrup->FirstChildElement("effect_duration")->Attribute("value", &temp);
	m_fMapleSyrupEffectDuration[0] = (float)temp;
	mapleSyrup->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMapleSyrupFireRate[0] = (float)temp;
	mapleSyrup->FirstChildElement("range")->Attribute("value", &m_nMapleSyrupRange[0]);

	// Power Upgrade Values
	TiXmlElement* mapleSyrupPower = mapleSyrup->FirstChildElement("power_upgrade");

	// Tier 1
	TiXmlElement* mapleSyrupPowerOne = mapleSyrupPower->FirstChildElement("tier_one");
	mapleSyrupPowerOne->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupPowerUpgradeCost[0]);
	mapleSyrupPowerOne->FirstChildElement("effect_duration")->Attribute("value", &temp);
	m_fMapleSyrupEffectDuration[1] = (float)temp;
	mapleSyrupPowerOne->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMapleSyrupFireRate[1] = (float)temp;

	// Tier 2
	TiXmlElement* mapleSyrupPowerTwo = mapleSyrupPower->FirstChildElement("tier_two");
	mapleSyrupPowerTwo->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupPowerUpgradeCost[1]);
	mapleSyrupPowerTwo->FirstChildElement("effect_duration")->Attribute("value", &temp);
	m_fMapleSyrupEffectDuration[2] = (float)temp;
	mapleSyrupPowerTwo->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMapleSyrupFireRate[2] = (float)temp;

	// Tier 3
	TiXmlElement* mapleSyrupPowerThree = mapleSyrupPower->FirstChildElement("tier_three");
	mapleSyrupPowerThree->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupPowerUpgradeCost[2]);
	mapleSyrupPowerThree->FirstChildElement("effect_duration")->Attribute("value", &temp);
	m_fMapleSyrupEffectDuration[3] = (float)temp;
	mapleSyrupPowerThree->FirstChildElement("fire_rate")->Attribute("value", &temp);
	m_fMapleSyrupFireRate[3] = (float)temp;

	// Range Upgrade Values
	TiXmlElement* mapleSyrupRange = mapleSyrup->FirstChildElement("range_upgrade");

	// Tier 1
	TiXmlElement* mapleSyrupRangeOne = mapleSyrupRange->FirstChildElement("tier_one");
	mapleSyrupRangeOne->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupRangeUpgradeCost[0]);
	mapleSyrupRangeOne->FirstChildElement("range")->Attribute("value", &m_nMapleSyrupRange[1]);

	// Tier 2
	TiXmlElement* mapleSyrupRangeTwo = mapleSyrupRange->FirstChildElement("tier_two");
	mapleSyrupRangeTwo->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupRangeUpgradeCost[1]);
	mapleSyrupRangeTwo->FirstChildElement("range")->Attribute("value", &m_nMapleSyrupRange[2]);

	// Tier 3
	TiXmlElement* mapleSyrupRangeThree = mapleSyrupRange->FirstChildElement("tier_three");
	mapleSyrupRangeThree->FirstChildElement("cost")->Attribute("value", &m_nMapleSyrupRangeUpgradeCost[2]);
	mapleSyrupRangeThree->FirstChildElement("range")->Attribute("value", &m_nMapleSyrupRange[3]);

#pragma endregion

#pragma region Hockey Stick Tower

	// Hockey Stick Tower
	TiXmlElement* hockeyStick = pRoot->FirstChildElement("hockey_stick");

	// Starting Values
	hockeyStick->FirstChildElement("damage")->Attribute("value", &temp);
	m_fHockeyStickDamage[0] = (float)temp;
	hockeyStick->FirstChildElement("spin_rate")->Attribute("value", &temp);
	m_fHockeyStickSpinRate[0] = (float)temp;

	// Damage Upgrade Values
	TiXmlElement* hockeyStickDamage = hockeyStick->FirstChildElement("damage_upgrade");

	// Tier 1
	TiXmlElement* hockeyStickDamageOne = hockeyStickDamage->FirstChildElement("tier_one");
	hockeyStickDamageOne->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickDamageUpgradeCost[0]);
	hockeyStickDamageOne->FirstChildElement("damage")->Attribute("value", &temp);
	m_fHockeyStickDamage[1] = (float)temp;

	// Tier 2
	TiXmlElement* hockeyStickDamageTwo = hockeyStickDamage->FirstChildElement("tier_two");
	hockeyStickDamageTwo->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickDamageUpgradeCost[1]);
	hockeyStickDamageTwo->FirstChildElement("damage")->Attribute("value", &temp);
	m_fHockeyStickDamage[2] = (float)temp;

	// Tier 3
	TiXmlElement* hockeyStickDamageThree = hockeyStickDamage->FirstChildElement("tier_three");
	hockeyStickDamageThree->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickDamageUpgradeCost[2]);
	hockeyStickDamageThree->FirstChildElement("damage")->Attribute("value", &temp);
	m_fHockeyStickDamage[3] = (float)temp;

	// Spin Rate Upgrade Values
	TiXmlElement* hockeyStickSpinRate = hockeyStick->FirstChildElement("spin_rate_upgrade");

	// Tier 1
	TiXmlElement* hockeyStickSpinRateOne = hockeyStickSpinRate->FirstChildElement("tier_one");
	hockeyStickSpinRateOne->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickSpinRateUpgradeCost[0]);
	hockeyStickSpinRateOne->FirstChildElement("spin_rate")->Attribute("value", &temp);
	m_fHockeyStickSpinRate[1] = (float)temp;

	// Tier 2
	TiXmlElement* hockeyStickSpinRateTwo = hockeyStickSpinRate->FirstChildElement("tier_two");
	hockeyStickSpinRateTwo->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickSpinRateUpgradeCost[1]);
	hockeyStickSpinRateTwo->FirstChildElement("spin_rate")->Attribute("value", &temp);
	m_fHockeyStickSpinRate[2] = (float)temp;

	// Tier 3
	TiXmlElement* hockeyStickSpinRateThree = hockeyStickSpinRate->FirstChildElement("tier_three");
	hockeyStickSpinRateThree->FirstChildElement("cost")->Attribute("value", &m_nHockeyStickSpinRateUpgradeCost[2]);
	hockeyStickSpinRateThree->FirstChildElement("spin_rate")->Attribute("value", &temp);
	m_fHockeyStickSpinRate[3] = (float)temp;

#pragma endregion

#pragma region Laser Tower

	// Laser Tower
	TiXmlElement* laser = pRoot->FirstChildElement("laser");

	// Starting Values
	laser->FirstChildElement("damage")->Attribute("value", &m_nLaserDamage[0]);
	laser->FirstChildElement("range")->Attribute("value", &m_nLaserRange[0]);

	// Damage Upgrade Values
	TiXmlElement* laserDamage = laser->FirstChildElement("damage_upgrade");

	// Tier 1
	TiXmlElement* laserDamageOne = laserDamage->FirstChildElement("tier_one");
	laserDamageOne->FirstChildElement("cost")->Attribute("value", &m_nLaserDamageUpgradeCost[0]);
	laserDamageOne->FirstChildElement("damage")->Attribute("value", &m_nLaserDamage[1]);

	// Tier 2
	TiXmlElement* laserDamageTwo = laserDamage->FirstChildElement("tier_two");
	laserDamageTwo->FirstChildElement("cost")->Attribute("value", &m_nLaserDamageUpgradeCost[1]);
	laserDamageTwo->FirstChildElement("damage")->Attribute("value", &m_nLaserDamage[2]);

	// Tier 3
	TiXmlElement* laserDamageThree = laserDamage->FirstChildElement("tier_three");
	laserDamageThree->FirstChildElement("cost")->Attribute("value", &m_nLaserDamageUpgradeCost[2]);
	laserDamageThree->FirstChildElement("damage")->Attribute("value", &m_nLaserDamage[3]);

	// Range Upgrade Values
	TiXmlElement* laserRange = laser->FirstChildElement("range_upgrade");

	// Tier 1
	TiXmlElement* laserRangeOne = laserRange->FirstChildElement("tier_one");
	laserRangeOne->FirstChildElement("cost")->Attribute("value", &m_nLaserRangeUpgradeCost[0]);
	laserRangeOne->FirstChildElement("range")->Attribute("value", &m_nLaserRange[1]);

	// Tier 2
	TiXmlElement* laserRangeTwo = laserRange->FirstChildElement("tier_two");
	laserRangeTwo->FirstChildElement("cost")->Attribute("value", &m_nLaserRangeUpgradeCost[1]);
	laserRangeTwo->FirstChildElement("range")->Attribute("value", &m_nLaserRange[2]);

	// Tier 3
	TiXmlElement* laserRangeThree = laserRange->FirstChildElement("tier_three");
	laserRangeThree->FirstChildElement("cost")->Attribute("value", &m_nLaserRangeUpgradeCost[2]);
	laserRangeThree->FirstChildElement("range")->Attribute("value", &m_nLaserRange[3]);

#pragma endregion

	// Load Sounds
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	m_hSellSound = pAudio->LoadAudio("resource/audio/doorClose.wav");
	m_hInvalidSound = pAudio->LoadAudio("resource/audio/invalidPurchase.wav");
	m_hMachineGunShotSound = pAudio->LoadAudio("resource/audio/machineGunShot.wav");
	m_hMapleSyrupShotSound = pAudio->LoadAudio("resource/audio/mapleSyrupShot.wav");
	m_hHockeyStickSlashSound = pAudio->LoadAudio("resource/audio/hockeyStickSlash.wav");
}

void TowerFlyweight::Unload()
{
	// Unload sounds
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	pAudio->UnloadAudio(m_hSellSound);
	pAudio->UnloadAudio(m_hInvalidSound);
	pAudio->UnloadAudio(m_hMachineGunShotSound);
	pAudio->UnloadAudio(m_hMapleSyrupShotSound);
	pAudio->UnloadAudio(m_hHockeyStickSlashSound);
}

/**********************************************************/
// Accessors

int TowerFlyweight::GetMachineGunDamage(int _index) const
{
	return m_nMachineGunDamage[_index];
}

float TowerFlyweight::GetMachineGunFireRate(int _index) const
{
	return m_fMachineGunFireRate[_index];
}

int TowerFlyweight::GetMachineGunRange(int _index) const
{
	return m_nMachineGunRange[_index];
}

int TowerFlyweight::GetMachineGunPowerUpgradeCost(int _index) const
{
	return m_nMachineGunPowerUpgradeCost[_index];
}

int TowerFlyweight::GetMachineGunRangeUpgradeCost(int _index) const
{
	return m_nMachineGunRangeUpgradeCost[_index];
}

float TowerFlyweight::GetMapleSyrupEffectDuration(int _index) const
{
	return m_fMapleSyrupEffectDuration[_index];
}

float TowerFlyweight::GetMapleSyrupFireRate(int _index) const
{
	return m_fMapleSyrupFireRate[_index];
}

int TowerFlyweight::GetMapleSyrupRange(int _index) const
{
	return m_nMapleSyrupRange[_index];
}

int TowerFlyweight::GetMapleSyrupPowerUpgradeCost(int _index) const
{
	return m_nMapleSyrupPowerUpgradeCost[_index];
}

int TowerFlyweight::GetMapleSyrupRangeUpgradeCost(int _index) const
{
	return m_nMapleSyrupRangeUpgradeCost[_index];
}

float TowerFlyweight::GetHockeyStickDamage(int _index) const
{
	return m_fHockeyStickDamage[_index];
}

float TowerFlyweight::GetHockeyStickSpinRate(int _index) const
{
	return m_fHockeyStickSpinRate[_index];
}

int TowerFlyweight::GetHockeyStickDamageUpgradeCost(int _index) const
{
	return m_nHockeyStickDamageUpgradeCost[_index];
}

int TowerFlyweight::GetHockeyStickSpinRateUpgradeCost(int _index) const
{
	return m_nHockeyStickSpinRateUpgradeCost[_index];
}

int TowerFlyweight::GetLaserDamage(int _index) const
{
	return m_nLaserDamage[_index];
}

int TowerFlyweight::GetLaserRange(int _index) const
{
	return m_nLaserRange[_index];
}

int TowerFlyweight::GetLaserDamageUpgradeCost(int _index) const
{
	return m_nLaserDamageUpgradeCost[_index];
}

int TowerFlyweight::GetLaserRangeUpgradeCost(int _index) const
{
	return m_nLaserRangeUpgradeCost[_index];
}

SGD::HTexture TowerFlyweight::GetRangeCirclesImage() const
{
	return m_hRangeCirclesImage;
}

SGD::HAudio TowerFlyweight::GetPurchaseSound() const
{
	return m_hPurchaseSound;
}

SGD::HAudio TowerFlyweight::GetClickSound() const
{
	return m_hClickSound;
}

SGD::HAudio TowerFlyweight::GetSellSound() const
{
	return m_hSellSound;
}

SGD::HAudio TowerFlyweight::GetInvalidSound() const
{
	return m_hInvalidSound;
}

SGD::HAudio TowerFlyweight::GetMachineGunShotSound() const
{
	return m_hMachineGunShotSound;
}

SGD::HAudio TowerFlyweight::GetMapleSyrupShotSound() const
{
	return m_hMapleSyrupShotSound;
}

SGD::HAudio TowerFlyweight::GetHockeyStickSlashSound() const
{
	return m_hHockeyStickSlashSound;
}

/**********************************************************/
// Mutators

void TowerFlyweight::SetRangeCirclesImage(SGD::HTexture _rangeCirclesImage)
{
	m_hRangeCirclesImage = _rangeCirclesImage;
}

void TowerFlyweight::SetPurchaseSound(SGD::HAudio _purchaseSound)
{
	m_hPurchaseSound = _purchaseSound;
}

void TowerFlyweight::SetClickSound(SGD::HAudio _clickSound)
{
	m_hClickSound = _clickSound;
}
