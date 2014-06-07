#include "LavaTrap.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"

LavaTrap::LavaTrap ()
{
	m_bRenderTop = false;
	m_fImageSwitchRate = 0.5f;
	m_fImageSwitchTimer = 0.5f;
	m_fDamage = 1.0f;
}


LavaTrap::~LavaTrap ()
{

}

void  LavaTrap::Update(float dt)
{
	m_fImageSwitchTimer -= dt;
	if(m_fImageSwitchTimer < 0)
	{
		m_bRenderTop = !m_bRenderTop;
		m_fImageSwitchTimer = m_fImageSwitchRate;
	}
}

void  LavaTrap::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance ();

	// Render base image or the lava overflowing
	if(m_bRenderTop == false)
		pGraphics->DrawTexture ( m_hBaseImage , SGD::Point ( m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y ) );
	else
		pGraphics->DrawTexture ( m_hGunImage , SGD::Point ( m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y ) );


	Tower::Render ();
}

void  LavaTrap::PostRender()
{
	
}

int   LavaTrap::GetType() const
{
	return ENT_TRAP_LAVA;
}

float LavaTrap::GetDamage(void) const
{
	return m_fDamage;
}

float LavaTrap::GetImageSwitchRate( void ) const
{
	return m_fImageSwitchRate;
}

float LavaTrap::GetImageSwitchTimer( void ) const
{
	return m_fImageSwitchTimer;
}

bool LavaTrap::GetRenderTop( void ) const
{
	return m_bRenderTop;
}

void  LavaTrap::SetDamage( float damage )
{
	m_fDamage = damage;
}

void  LavaTrap::SetImageSwitchRate( float rate )
{
	m_fImageSwitchRate = rate;
}

void  LavaTrap::SetImageSwitchTimer( float time )
{
	m_fImageSwitchTimer = time;
}

void LavaTrap::SetRenderTop( bool choice )
{
	m_bRenderTop = choice;
}

