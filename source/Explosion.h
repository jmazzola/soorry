#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"


class Explosion : public Entity
{
public:

	Explosion();
	~Explosion();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual SGD::Rectangle GetRect() const override;

	/**********************************************************/
	// Accessors
	float GetDamage() const;
	float GetRadius() const;

	/**********************************************************/
	// Mutators
	void SetDamage(float damage);
	void SetRadius(float radius);
	void SetImage(SGD::HTexture image);

protected:

	/**********************************************************/
	// Data Members
	bool m_bExploded;
	int _m_nImageIndex;
	float _m_fImageSpeed;
	float _m_fNextImage;
	float m_fDamage;
	float m_fRadius;
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
};

