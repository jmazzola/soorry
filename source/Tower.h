#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

#include <vector>
using namespace std;

class Enemy;
class EntityManager;

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
	// Accessors
	bool IsSelected() const;

	/**********************************************************/
	// Mutators
	void SetSelected(bool selected);
	void SetBaseImage(SGD::HTexture baseImage);
	void SetGunImage(SGD::HTexture gunImage);

protected:

	/**********************************************************/
	// Data members
	bool m_bSelected;
	float m_fRotation;
	EntityManager* m_pEntityManager;
	SGD::HTexture m_hBaseImage;
	SGD::HTexture m_hGunImage;
};

