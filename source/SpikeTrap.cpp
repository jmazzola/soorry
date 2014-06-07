#include "SpikeTrap.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

SpikeTrap::SpikeTrap ()
{
	m_fDamage = 5.5f;
	m_fAttackCD = 1.0f;
	m_fAttackTimer = 0.0f;
	m_fHoldLength = 0.2f;
	m_fHoldTimer = 0.0f;
}


SpikeTrap::~SpikeTrap ()
{
}

void  SpikeTrap::Update ( float dt )
{
	m_fAttackTimer -= dt;
	m_fHoldTimer -= dt;

	if ( m_fAttackTimer <= 0.0f )
	{
		m_fAttackTimer = m_fAttackCD;
		m_fHoldTimer = m_fHoldLength;
	}
}

void SpikeTrap::Render ()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance ();

	// Render base image
	pGraphics->DrawTexture ( m_hBaseImage , SGD::Point ( m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y ) );

	if ( m_fHoldTimer > 0 )
	{

		// Render spikes
		pGraphics->DrawTexture ( m_hGunImage , SGD::Point ( m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y ) );

	}

	Tower::Render ();
}

void SpikeTrap::PostRender ()
{
	
}

int   SpikeTrap::GetType () const
{
	return ENT_TRAP_SPIKE;
}

// GetActive
// Returns true if the spikes are up and should deal damage
bool  SpikeTrap::GetActive ( void ) const
{
	if ( m_fHoldTimer > 0 )
		return true;
	else
		return false;
}

/**********************************************************/
// Accessors

float SpikeTrap::GetAttackTimer ( void ) const
{
	return m_fAttackTimer;
}

float SpikeTrap::GetAttackCD ( void ) const
{
	return m_fAttackCD;
}

float SpikeTrap::GetHoldTimer ( void ) const
{
	return m_fHoldTimer;
}

float SpikeTrap::GetHoldLength ( void ) const
{
	return m_fHoldLength;
}

float SpikeTrap::GetDamage ( void ) const
{
	return m_fDamage;
}

/**********************************************************/
// Mutators

void  SpikeTrap::SetAttackTimer ( float time )
{
	m_fAttackTimer = time;
}

void  SpikeTrap::SetAttackCD ( float length )
{
	m_fAttackCD = length;
}

void  SpikeTrap::SetHoldTimer ( float time )
{
	m_fHoldTimer = time;
}

void  SpikeTrap::SetHoldLength ( float length )
{
	m_fHoldLength = length;
}

void SpikeTrap::SetDamage ( float damage )
{
	m_fDamage = damage;
}
