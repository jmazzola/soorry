#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

class MapleSyrupBullet : public Entity
{
public:

	MapleSyrupBullet();
	~MapleSyrupBullet();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	/**********************************************************/
	// Accessors
	float GetSlowTime() const;

	/**********************************************************/
	// Mutators
	void SetSlowTime(float slowTime);
	void SetImage(SGD::HTexture image);

protected:

	float m_fSlowTime;
	float m_fLifetime;
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
};

