#pragma once

//#include "Node.h"

#include "../SGD Wrappers/SGD_Geometry.h"

/**********************************************************/
// Forward Declarations
class Entity;
struct Node;

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
	int m_nWorldWidth;
	int m_nWorldHeight;
	int** m_nNodeChart;
	float m_fTimeToPathfind;
	Entity* m_pAgent;
	Entity* m_pPlayer;
	SGD::Point m_ptMoveTarget;
	SGD::Point m_ptFindTarget;

private:

	/**********************************************************/
	// Helper Functions
	bool Pathfind(Node start, Node end);
};


struct Node
{
	Node()
	{
		x = 0;
		y = 0;
	}

	Node(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x, y;
};
