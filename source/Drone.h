#pragma once
#include "Entity.h"

class Player;

class Drone : public Entity
{
public:
	Drone();
	~Drone();
	virtual void Update(float dt)						override;
	virtual void Render()								override;
	virtual int  GetType() const						override;
	virtual void HandleCollision(const IEntity* pOther) override;
protected:
	Player* m_pPlayer;

};

