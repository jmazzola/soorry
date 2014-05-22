#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"

/**********************************************************/
// Forward Declarations
class Entity;

class AIComponent
{
public:

	AIComponent();
	~AIComponent();

	/**********************************************************/
	// AI Behaviors
	enum BEHAVIOR { };

	/**********************************************************/
	// Interface
	void Update(float dt);

	/**********************************************************/
	// Accessors
	Entity* GetAgent() const;

	/**********************************************************/
	// Mutators
	void SetAgent(Entity* agent);

protected:

	/**********************************************************/
	// Data Members
	Entity* m_pAgent;
	SGD::Point m_ptMoveTarget;
	SGD::Point m_ptFindTarget;
};

