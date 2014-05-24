#include "AIComponent.h"

#include "WorldManager.h"
#include "Enemy.h"

#include <queue>
using namespace std;


AIComponent::AIComponent()
{
	m_fTimeToPathfind = 0.0f;

	// Create node chart
	WorldManager* pWorld = WorldManager::GetInstance();
	m_nNodeChart = new int*[pWorld->GetWorldWidth()];
	for (unsigned int x = 0; x < pWorld->GetWorldWidth(); x++)
		m_nNodeChart[x] = new int[pWorld->GetWorldHeight()];
}


AIComponent::~AIComponent()
{
}


/**********************************************************/
// Interface

void AIComponent::Update(float dt)
{
	WorldManager* pWorld = WorldManager::GetInstance();

	// Update timers
	m_fTimeToPathfind -= dt;

	// Pathfind
	if (m_fTimeToPathfind <= 0.0f)
	{
		// Pathfind to player
		m_ptFindTarget = m_pPlayer->GetPosition();

		// Start node
		Position start;
		start.x = (int)(m_pAgent->GetPosition().x + pWorld->GetTileWidth() / 2) / pWorld->GetTileWidth();
		start.y = (int)(m_pAgent->GetPosition().y + pWorld->GetTileHeight() / 2) / pWorld->GetTileHeight();

		// End node
		Position end;
		end.x = (int)(m_ptFindTarget.x + pWorld->GetTileWidth() / 2) / pWorld->GetTileWidth();
		end.y = (int)(m_ptFindTarget.y + pWorld->GetTileHeight() / 2) / pWorld->GetTileHeight();

		Pathfind(start, end);
	}

	// Go to player (testing)
	m_ptMoveTarget = m_pPlayer->GetPosition();

	// Create move vector
	SGD::Vector toTarget = m_ptMoveTarget - m_pAgent->GetPosition();
	toTarget.Normalize();
	toTarget *= dynamic_cast<Enemy*>(m_pAgent)->GetSpeed() * dt;
	SGD::Point oldPosition = m_pAgent->GetPosition();
	SGD::Point newPosition = oldPosition;

	// Check x
	newPosition.x += toTarget.x;
	m_pAgent->SetPosition(newPosition);
	if (pWorld->CheckCollision(m_pAgent))
	{
		newPosition.x = oldPosition.x;
		m_pAgent->SetPosition(newPosition);
	}

	// Check y
	newPosition.y += toTarget.y;
	m_pAgent->SetPosition(newPosition);
	if (pWorld->CheckCollision(m_pAgent))
	{
		newPosition.y = oldPosition.y;
		m_pAgent->SetPosition(newPosition);
	}

	// Move
	m_pAgent->SetPosition(newPosition);
}

/**********************************************************/
// Accessors

Entity* AIComponent::GetAgent() const
{
	return m_pAgent;
}

Entity* AIComponent::GetPlayer() const
{
	return m_pPlayer;
}

/**********************************************************/
// Mutators

void AIComponent::SetAgent(Entity* _agent)
{
	m_pAgent = _agent;
}

void AIComponent::SetPlayer(Entity* _player)
{
	m_pPlayer = _player;
}

/**********************************************************/
// Helper Functions

bool AIComponent::Pathfind(Position start, Position end)
{
	WorldManager* pWorld = WorldManager::GetInstance();

	// Reset node chart
	for (int x = 0; x < pWorld->GetWorldWidth(); x++)
	{
		for (int y = 0; y < pWorld->GetWorldHeight(); y++)
		{
			if (pWorld->IsSolidAtPosition(x, y))
				m_nNodeChart[x][y] = -1;
			else
				m_nNodeChart[x][y] = 0;
		}
	}

	queue<Position> nodes;

	nodes.push(end);
	m_nNodeChart[end.x][end.y] = 1;

	int highestNode = 1;

	while (!nodes.empty())
	{
		Position node;
		node = nodes.front();
		nodes.pop();

		if (node.x == end.x && node.y == end.y)
			return true;

		
	}
}