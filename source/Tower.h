#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Handle.h"

class Tower : public Entity
{
public:

	Tower();
	~Tower();

	/**********************************************************/
	// Mutators

private:

	SGD::HTexture m_hBaseImage;
	SGD::HTexture m_hGunImage;
};

