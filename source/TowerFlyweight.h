#pragma once

#include "../SGD Wrappers/SGD_Handle.h"

#include <string>
using namespace std;


class TowerFlyweight
{
public:

	TowerFlyweight();
	~TowerFlyweight();

	/**********************************************************/
	// Interface Methods
	void Load(string fileName);
	void Unload();

	/**********************************************************/
	// Accessors
	int GetMachineGunDamage(int index) const;
	float GetMachineGunFireRate(int index) const;
	int GetMachineGunRange(int index) const;
	int GetMachineGunPowerUpgradeCost(int index) const;
	int GetMachineGunRangeUpgradeCost(int index) const;
	float GetMapleSyrupEffectDuration(int index) const;
	float GetMapleSyrupFireRate(int index) const;
	int GetMapleSyrupRange(int index) const;
	int GetMapleSyrupPowerUpgradeCost(int index) const;
	int GetMapleSyrupRangeUpgradeCost(int index) const;
	float GetHockeyStickDamage(int index) const;
	float GetHockeyStickSpinRate(int index) const;
	int GetHockeyStickDamageUpgradeCost(int index) const;
	int GetHockeyStickSpinRateUpgradeCost(int index) const;
	int GetLaserDamage(int index) const;
	int GetLaserRange(int index) const;
	int GetLaserDamageUpgradeCost(int index) const;
	int GetLaserRangeUpgradeCost(int index) const;
	SGD::HTexture GetRangeCirclesImage() const;
	SGD::HAudio GetPurchaseSound() const;
	SGD::HAudio GetClickSound() const;
	SGD::HAudio GetSellSound() const;
	SGD::HAudio GetInvalidSound() const;
	SGD::HAudio GetMachineGunShotSound() const;
	SGD::HAudio GetMapleSyrupShotSound() const;
	SGD::HAudio GetHockeyStickSlashSound() const;

	/**********************************************************/
	// Mutators
	void SetRangeCirclesImage(SGD::HTexture rangeCirclesImage);
	void SetPurchaseSound(SGD::HAudio purchaseSound);
	void SetClickSound(SGD::HAudio clickSound);

protected:

	/**********************************************************/
	// Data Members
	int m_nMachineGunDamage[4];
	float m_fMachineGunFireRate[4];
	int m_nMachineGunRange[4];
	int m_nMachineGunPowerUpgradeCost[3];
	int m_nMachineGunRangeUpgradeCost[3];
	float m_fMapleSyrupEffectDuration[4];
	float m_fMapleSyrupFireRate[4];
	int m_nMapleSyrupRange[4];
	int m_nMapleSyrupPowerUpgradeCost[3];
	int m_nMapleSyrupRangeUpgradeCost[3];
	float m_fHockeyStickDamage[4];
	float m_fHockeyStickSpinRate[4];
	int m_nHockeyStickDamageUpgradeCost[3];
	int m_nHockeyStickSpinRateUpgradeCost[3];
	int m_nLaserDamage[4];
	int m_nLaserRange[4];
	int m_nLaserDamageUpgradeCost[3];
	int m_nLaserRangeUpgradeCost[3];
	SGD::HTexture m_hRangeCirclesImage = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPurchaseSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hClickSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSellSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hInvalidSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hMachineGunShotSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hMapleSyrupShotSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHockeyStickSlashSound = SGD::INVALID_HANDLE;
};

