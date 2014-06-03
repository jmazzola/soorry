#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

class Tower : public Entity
{
public:

	Tower();
	~Tower();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;

	/**********************************************************/
	// Mutators
	void SetBaseImage(SGD::HTexture baseImage);
	void SetGunImage(SGD::HTexture gunImage);

protected:

	/**********************************************************/
	// Data members
	float m_fRotation;
	SGD::HTexture m_hBaseImage;
	SGD::HTexture m_hGunImage;
};

