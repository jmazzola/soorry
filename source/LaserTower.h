#pragma once

#include "Tower.h"

#include <vector>
using namespace std;

#include "../SGD Wrappers/SGD_Listener.h"


class LaserTower : public Tower, public SGD::Listener
{
public:

	LaserTower();
	~LaserTower();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void DrawMenu() override;
	virtual int GetType() const override;
	virtual void Upgrade(int slot, unsigned int* points) override;
	virtual void HandleEvent(const SGD::Event* event) override;

	/**********************************************************/
	// Accessors
	int GetDamage() const;
	int GetRange() const;

	/**********************************************************/
	// Mutators
	void SetDamage(int damage);
	void SetRange(int range);

protected:

	int m_nDamage;
	int m_nRange;
	vector<LaserTower*> m_vConnectedTowers;

	virtual void ExternalUpgrade() override;
};

