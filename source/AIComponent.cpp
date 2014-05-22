#include "AIComponent.h"


AIComponent::AIComponent()
{
}


AIComponent::~AIComponent()
{
}


/**********************************************************/
// Interface

void AIComponent::Update(float dt)
{

}

/**********************************************************/
// Accessors

Entity* AIComponent::GetAgent() const
{
	return m_pAgent;
}

/**********************************************************/
// Mutators

void AIComponent::SetAgent(Entity* _agent)
{
	m_pAgent = _agent;
}