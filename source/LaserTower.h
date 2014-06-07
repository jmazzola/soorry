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
	virtual int GetType() const override;

protected:

	vector<LaserTower*> m_vConnectedTowers;
};

