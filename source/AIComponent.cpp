#include "AIComponent.h"

#include "WorldManager.h"
#include "GameplayState.h"
#include "Enemy.h"
#include "Camera.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include <queue>
using namespace std;

#define BOXCAST_INTERVAL 16.0f
#define BOXCAST_ITERATIONS 100

AIComponent::AIComponent()
{
	m_fTimeToPathfind = 0.0f;

	// Get world sizes
	WorldManager* pWorld = WorldManager::GetInstance();
	m_nWorldWidth = pWorld->GetWorldWidth();
	m_nWorldHeight = pWorld->GetWorldHeight();

	// Create node chart
	m_nNodeChart = new int*[m_nWorldWidth];
	for (int x = 0; x < m_nWorldWidth; x++)
		m_nNodeChart[x] = new int[m_nWorldHeight];

	// Get the entity manager
	m_pEntityManager = GameplayState::GetInstance()->GetEntityManager();
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
	// Set camera (FOR DEBUG PURPOSES ONLY!)
	/*Camera::x = (int)m_pAgent->GetPosition().x - 384;
	Camera::y = (int)m_pAgent->GetPosition().y - 284;*/

	WorldManager* pWorld = WorldManager::GetInstance();
	int tileWidth = pWorld->GetTileWidth();
	int tileHeight = pWorld->GetTileHeight();

	// Update timers
	m_fTimeToPathfind -= dt;

	// JIT fix for this stupid, fucking bug
	if ((int)m_pAgent->GetPosition().x < -1000 || (int)m_pAgent->GetPosition().y < -1000)
		m_pAgent->SetPosition({ 0, 0 });

	// Check for direct route (TEMPORARILY DISABLED!)
	SGD::Vector toPlayer = m_pPlayer->GetPosition() - m_pAgent->GetPosition();
	toPlayer.Normalize();
	if (false && BoxCast(toPlayer, m_pPlayer->GetPosition()))
	{
		m_ptMoveTarget = m_pPlayer->GetPosition();
	}

	// No direct route, need to pathfind
	else
	{
		// Check if we create a new path
		if (m_fTimeToPathfind <= 0.0f)
		{
			// Pathfind to player
			m_ptFindTarget = m_pPlayer->GetPosition();

			// Start node
			Node start;
			start.x = (int)(m_pAgent->GetPosition().x + 16) / tileWidth;
			start.y = (int)(m_pAgent->GetPosition().y + 16) / tileHeight;

			// End node
			Node end;
			end.x = (int)(m_ptFindTarget.x + 16) / tileWidth;
			end.y = (int)(m_ptFindTarget.y + 16) / tileHeight;

			Pathfind(start, end);

			// Reset pathing timer
			m_fTimeToPathfind = 2.0f + (rand() % 300) / 100.0f;
		}

		if (m_pAgent->GetPosition().x == 0.2f || m_pAgent->GetPosition().y == 0.2f)
			m_pAgent->SetPosition({ 0, 0 });

		// Find move target
		int snapX, snapY;
		//float snapFX, snapFY;
		snapX = (int)(m_pAgent->GetPosition().x + tileWidth / 2) / tileWidth;
		snapY = (int)(m_pAgent->GetPosition().y + tileWidth / 2) / tileHeight;

		// Determine direction
		int smallestValue = m_nNodeChart[snapX][snapY];
		int goX = snapX;
		int goY = snapY;
		if (snapX < m_nWorldWidth - 1 && m_nNodeChart[snapX + 1][snapY] < smallestValue && m_nNodeChart[snapX + 1][snapY] > 0)
		{
			smallestValue = m_nNodeChart[snapX + 1][snapY];
			goX = snapX + 1;
			goY = snapY;
		}
		if (snapX > 0 && m_nNodeChart[snapX - 1][snapY] < smallestValue && m_nNodeChart[snapX - 1][snapY] > 0)
		{
			smallestValue = m_nNodeChart[snapX - 1][snapY];
			goX = snapX - 1;
			goY = snapY;
		}
		if (snapY < m_nWorldHeight - 1 && m_nNodeChart[snapX][snapY + 1] < smallestValue && m_nNodeChart[snapX][snapY + 1] > 0)
		{
			smallestValue = m_nNodeChart[snapX][snapY + 1];
			goX = snapX;
			goY = snapY + 1;
		}
		if (snapY > 0 && m_nNodeChart[snapX][snapY - 1] < smallestValue && m_nNodeChart[snapX][snapY - 1] > 0)
		{
			smallestValue = m_nNodeChart[snapX][snapY - 1];
			goX = snapX;
			goY = snapY - 1;
		}

		// Needs a new path
		if (smallestValue == m_nNodeChart[snapX][snapY])
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

			// Reset pathing timer
			m_fTimeToPathfind = 1.0f + (rand() % 200) / 100.0f;
		}

		// Determine where to go
		m_ptMoveTarget = SGD::Point((float)(goX * tileWidth), (float)(goY * tileHeight));
	}

	// FOR DEBUG PURPOSES ONLY!
	//m_ptMoveTarget = m_pPlayer->GetPosition();

	// Create move vector
	SGD::Vector toTarget = m_ptMoveTarget - m_pAgent->GetPosition();
	toTarget.Normalize();
	toTarget *= dynamic_cast<Enemy*>(m_pAgent)->GetSpeed() * dt;
	SGD::Point oldPosition = m_pAgent->GetPosition();
	SGD::Point newPosition = oldPosition;

#if 1

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

	if (m_pEntityManager->CheckCollision(m_pAgent->GetRect(), 0))
	{
		Game* pGame = Game::GetInstance();

		// If we're godmode, don't do anything
		if (!pGame->IsGod())
		{
			float damage = 8.0f * dt;
			SGD::Event e("TAKE_DAMAGE", (void*)&damage);
			e.SendEventNow();
		}

		newPosition = oldPosition;
	}

#endif

	// Calculate velocity for enemies rotation
	SGD::Vector velocity = newPosition - oldPosition;
	if (velocity != SGD::Vector(0, 0))
		m_pAgent->SetVelocity(velocity);

	// Move
	m_pAgent->SetPosition(newPosition);
}

void AIComponent::Render()
{
	// NOTE: This function is only called for certain debug practices

	// Get camera position in terms of tiles
	int camTileX = Camera::x / 32;
	int camTileY = Camera::y / 32;

	// Get stop point for rendering
	int stopX = camTileX + (int)ceil((800.0f / 32)) + 1;
	int stopY = camTileY + (int)ceil((600.0f / 32)) + 1;

	// Loop through the viewport
	for (int x = camTileX; x < stopX; x++)
	{
		for (int y = camTileY; y < stopY; y++)
		{
			// Don't render out-of-bounds index
			if (x < 0 || y < 0 || x >= m_nWorldWidth || y >= m_nWorldHeight)
				continue;

			SGD::GraphicsManager::GetInstance()->DrawString(std::to_string(m_nNodeChart[x][y]).c_str(), { x * 32.0f - Camera::x, y * 32.0f - Camera::y });
		}
	}
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

		/*if (node.x < 1 || node.y < 1 || node.x >= m_nWorldWidth - 1 || node.y >= m_nWorldHeight - 1)
			continue;*/

		if (node.x > 0 && m_nNodeChart[node.x - 1][node.y] == 0)
		{
			nodes.push(Node(node.x - 1, node.y));
			m_nNodeChart[node.x - 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.x < m_nWorldWidth - 1 && m_nNodeChart[node.x + 1][node.y] == 0)
		{
			nodes.push(Node(node.x + 1, node.y));
			m_nNodeChart[node.x + 1][node.y] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.y < m_nWorldHeight - 1 && m_nNodeChart[node.x][node.y + 1] == 0)
		{
			nodes.push(Node(node.x, node.y + 1));
			m_nNodeChart[node.x][node.y + 1] = m_nNodeChart[node.x][node.y] + 1;
		}

		if (node.y > 0 && m_nNodeChart[node.x][node.y - 1] == 0)
		{
			nodes.push(Node(node.x, node.y - 1));
			m_nNodeChart[node.x][node.y - 1] = m_nNodeChart[node.x][node.y] + 1;
		}
	}

	return false;
}

bool AIComponent::BoxCast(SGD::Vector _direction, SGD::Point _destination) const
{
	WorldManager* pWorld = WorldManager::GetInstance();
	SGD::Rectangle rect = m_pAgent->GetRect();

	for (int i = 0; i < BOXCAST_ITERATIONS; i++)
	{
		// Move the box
		rect.left += _direction.x * BOXCAST_INTERVAL;
		rect.right += _direction.x * BOXCAST_INTERVAL;
		rect.top += _direction.x * BOXCAST_INTERVAL;
		rect.bottom += _direction.x * BOXCAST_INTERVAL;
		rect.left += _direction.y * BOXCAST_INTERVAL;
		rect.right += _direction.y * BOXCAST_INTERVAL;
		rect.top += _direction.y * BOXCAST_INTERVAL;
		rect.bottom += _direction.y * BOXCAST_INTERVAL;

		// Check for collision against world
		if (pWorld->CheckCollision(rect))
			return false;

		// Check if boxcast is successful
		if (_destination.IsWithinRectangle(rect))
			return true;
	}

	return false;
}