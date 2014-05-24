#include "AIComponent.h"

#include "WorldManager.h"
#include "Enemy.h"

#include <queue>
using namespace std;


AIComponent::AIComponent()
{
	m_fTimeToPathfind = 0.0f;

	// Get world sizes
	WorldManager* pWorld = WorldManager::GetInstance();
	m_nWorldWidth = pWorld->GetWorldWidth();
	m_nWorldHeight = pWorld->GetWorldHeight();

	// Create node chart
	m_nNodeChart = new int*[m_nWorldWidth];
	for (unsigned int x = 0; x < m_nWorldWidth; x++)
		m_nNodeChart[x] = new int[m_nWorldHeight];
}


AIComponent::~AIComponent()
{
	for (int x = 0; x < m_nWorldWidth; x++)
		delete[] m_nNodeChart[x];
	delete[] m_nNodeChart;
}


/**********************************************************/
// Interface

void AIComponent::Update(float dt)
{
	WorldManager* pWorld = WorldManager::GetInstance();
	int tileWidth = pWorld->GetTileWidth();
	int tileHeight = pWorld->GetTileHeight();

	// Update timers
	m_fTimeToPathfind -= dt;

	// Pathfind
	if (m_fTimeToPathfind <= 0.0f)
	{
		// Pathfind to player
		m_ptFindTarget = m_pPlayer->GetPosition();

		// Start node
		Node start;
		start.x = (int)(m_pAgent->GetPosition().x) / tileWidth;
		start.y = (int)(m_pAgent->GetPosition().y) / tileHeight;

		// End node
		Node end;
		end.x = (int)(m_ptFindTarget.x) / tileWidth;
		end.y = (int)(m_ptFindTarget.y) / tileHeight;

		Pathfind(start, end);
		m_fTimeToPathfind = 1.0f;
	}

	// Find move target
	int snapX, snapY;
	snapX = (int)(m_pAgent->GetPosition().x) / tileWidth;
	snapY = (int)(m_pAgent->GetPosition().y) / tileHeight;

	// Determine direction
	int smallestValue = INT_MAX;
	if (snapX < m_nWorldWidth - 1 && m_nNodeChart[snapX + 1][snapY] < smallestValue && m_nNodeChart[snapX + 1][snapY] > 0)
	{
		smallestValue = m_nNodeChart[snapX + 1][snapY];
		m_ptMoveTarget = m_pAgent->GetPosition() + SGD::Vector(32, 0);
	}
	if (snapX > 0 && m_nNodeChart[snapX - 1][snapY] < smallestValue && m_nNodeChart[snapX - 1][snapY] > 0)
	{
		smallestValue = m_nNodeChart[snapX - 1][snapY];
		m_ptMoveTarget = m_pAgent->GetPosition() + SGD::Vector(-32, 0);
	}
	if (snapY < m_nWorldHeight - 1 && m_nNodeChart[snapX][snapY + 1] < smallestValue && m_nNodeChart[snapX][snapY + 1] > 0)
	{
		smallestValue = m_nNodeChart[snapX][snapY + 1];
		m_ptMoveTarget = m_pAgent->GetPosition() + SGD::Vector(0, 32);
	}
	if (snapY > 0 && m_nNodeChart[snapX][snapY - 1] < smallestValue && m_nNodeChart[snapX][snapY - 1] > 0)
	{
		smallestValue = m_nNodeChart[snapX][snapY - 1];
		m_ptMoveTarget = m_pAgent->GetPosition() + SGD::Vector(0, -32);
	}

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

bool AIComponent::Pathfind(Node start, Node end)
{
	WorldManager* pWorld = WorldManager::GetInstance();

	// Reset node chart
	for (int x = 0; x < m_nWorldWidth; x++)
	{
		for (int y = 0; y < m_nWorldHeight; y++)
		{
			if (pWorld->IsSolidAtPosition(x, y))
				m_nNodeChart[x][y] = -1;
			else
				m_nNodeChart[x][y] = 0;
		}
	}

	queue<Node> nodes;

	nodes.push(end);
	m_nNodeChart[end.x][end.y] = 1;

	int highestNode = 1;

	while (!nodes.empty())
	{
		Node node;
		node = nodes.front();
		nodes.pop();

		if (node.x == start.x && node.y == start.y)
			return true;

		if (node.x < 1 || node.y < 1 || node.x >= m_nWorldWidth - 1 || node.y >= m_nWorldHeight - 1)
			continue;

		if (m_nNodeChart[node.x - 1][node.y] == 0)
		{
			nodes.push(Node(node.x - 1, node.y));
			m_nNodeChart[node.x - 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (m_nNodeChart[node.x + 1][node.y] == 0)
		{
			nodes.push(Node(node.x + 1, node.y));
			m_nNodeChart[node.x + 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (m_nNodeChart[node.x][node.y + 1] == 0)
		{
			nodes.push(Node(node.x, node.y + 1));
			m_nNodeChart[node.x][node.y + 1] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (m_nNodeChart[node.x][node.y - 1] == 0)
		{
			nodes.push(Node(node.x, node.y - 1));
			m_nNodeChart[node.x][node.y - 1] = m_nNodeChart[node.x][node.y] + 1;
		}
	}

	return false;
}