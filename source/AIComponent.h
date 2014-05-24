#pragma once

//#include "Node.h"

#include "../SGD Wrappers/SGD_Geometry.h"

/**********************************************************/
// Forward Declarations
class Entity;
struct Position;

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
	Entity* GetPlayer() const;

	/**********************************************************/
	// Mutators
	void SetAgent(Entity* agent);
	void SetPlayer(Entity* player);

protected:

	/**********************************************************/
	// Data Members
	int** m_nNodeChart;
	float m_fTimeToPathfind;
	Entity* m_pAgent;
	Entity* m_pPlayer;
	SGD::Point m_ptMoveTarget;
	SGD::Point m_ptFindTarget;

private:

	/**********************************************************/
	// Helper Functions
	bool Pathfind(Position start, Position end);
};


struct Position
{
	int x, y;
};
