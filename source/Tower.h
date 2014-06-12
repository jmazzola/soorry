#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

#include <vector>
using namespace std;

class Enemy;
class EntityManager;
class TowerFlyweight;

class Tower : public Entity
{
public:

	Tower();
	~Tower();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender();
	virtual void DrawMenu();
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;
	virtual void Upgrade(int slot, unsigned int* points);

	/**********************************************************/
	// Accessors
	bool IsSelected() const;
	int GetUpgradeOne() const;
	int GetUpgradeTwo() const;
	int GetSellValue() const;

	/**********************************************************/
	// Mutators
	void SetSelected(bool selected);
	void SetBaseImage(SGD::HTexture baseImage);
	void SetGunImage(SGD::HTexture gunImage);
	void SetTowerFlyweight(TowerFlyweight* towerFlyweight);
	void SetUpgradeOne(int tier);
	void SetUpgradeTwo(int tier);
	void SetSellValue(int sellValue);

protected:

	/**********************************************************/
	// Data members
	bool m_bHoverOne;
	bool m_bHoverTwo;
	bool m_bHoverSell;
	bool m_bSelected;
	float m_fRotation;
	int m_nUpgradeOne;
	int m_nUpgradeTwo;
	int m_nSellValue;
	EntityManager* m_pEntityManager;
	TowerFlyweight* m_pTowerFlyweight;
	SGD::HTexture m_hBaseImage;
	SGD::HTexture m_hGunImage;
};

