#include "TrickShotBullet.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnimationManager.h"
#include "WorldManager.h"
#include "Sprite.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"
#include <vector>

TrickShotBullet::TrickShotBullet () : SGD::Listener(this)
{
	RegisterForEvent("IM_HIT");
	SetSprite(AnimationManager::GetInstance()->GetSprite("trickshot"));
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "trickshot";
	SetMass(1);
	SetStaticFrictionCoefficient(0.7f);
	SetDynamicFrictionCoefficient(135.0f);
	m_bResolvingX = false;
	m_bResolvingY = false;
}


TrickShotBullet::~TrickShotBullet ()
{
	ClearHitList();
	UnregisterFromEvent("IM_HIT");
}

int TrickShotBullet::GetType ( void ) const
{
	return ENT_BULLET_TRICKSHOT;
}

void TrickShotBullet::Update ( float dt )
{
	m_fRotation += dt * 3;
	m_fLifeTime -= dt;
	if(m_fRotation > 2 * SGD::PI)
		m_fRotation = 0.0f;

	// Compute object speed, that is the norm of the velocity
	m_fSpeed = m_vtVelocity.ComputeLength ();
	// Compute the aerodynamic drag force
	m_vDragForce = m_vtVelocity * (-.5f * m_fSpeed * m_fDragCoefficient * 64);
	// Apply Friction
	ApplyFriction ( dt );
	// Compute the acceleration using Newton's 2nd Law
	m_vAcceleration = m_vTotalForces / m_fMass;
	// Compute the velocity
	m_vtVelocity += m_vAcceleration * dt;

	Entity::Update ( dt );

	std::vector<SGD::Rectangle> hits = WorldManager::GetInstance()->CheckTrickShot(GetRect());
	
	
	Bounce(hits);

	UpdateHitList();

	if ( m_vForce != SGD::Vector { 0.0f , 0.0f } )
	{
		m_fLifeTime = 1.5f;
	}
	if ( m_fLifeTime <= 0.0f)
	{
		DestroyEntityMessage* pmsg = new DestroyEntityMessage(this);
		pmsg->QueueMessage();
	}
}

void TrickShotBullet::Bounce (std::vector<SGD::Rectangle> hit)
{
	// Multiple collisions
	if ( hit.size() > 1)
	{
		int LxHits = 0;
		int RxHits = 0;
		int TyHits = 0;
		int ByHits = 0;
		SGD::Rectangle me = GetRect ();
		// Find out how many intersecting rectangles of which your sides are the closest to
		// the center of
		for ( unsigned int i = 0; i < hit.size(); i++ )
		{
			SGD::Rectangle section = GetRect ().ComputeIntersection ( hit[i] );

			if(section.left != me.left)
				LxHits++;
			if(section.right != me.right)
				RxHits++;
			if(section.top != me.top)
				TyHits++;
			if(section.bottom != me.bottom)
				ByHits++;
		}
		
		
		if(LxHits != RxHits && !m_bResolvingX)
		{
			m_vtVelocity.x = -m_vtVelocity.x * 0.9f;
			m_vForce.x = -m_vForce.x * 0.9f;
			m_bResolvingX = true;
		}
		else if(LxHits == RxHits)
			m_bResolvingX = false;

		if(TyHits != ByHits && !m_bResolvingY)
		{
			m_vtVelocity.y = -m_vtVelocity.y * 0.9f;
			m_vForce.y = -m_vForce.y * 0.9f;
			m_bResolvingY = true;
		}
		else if(TyHits == ByHits)
			m_bResolvingY = false;
		
		m_rLastHit = hit;
	}
	else if(hit.size() == 1)
	{
		SGD::Rectangle me = GetRect ();
		SGD::Rectangle section = GetRect ().ComputeIntersection ( hit[0] );
		// Hit on the left
		if ( !m_bResolvingX && section.left == me.left && section.top == me.top && section.bottom == me.bottom )
		{
			m_vtVelocity.x = -m_vtVelocity.x * 0.9f;
			m_vForce.x = -m_vForce.x * 0.9f;
			m_bResolvingX = true;
		}
		// Hit on the right
		else if ( !m_bResolvingX && section.right == me.right && section.top == me.top && section.bottom == me.bottom )
		{
			m_vtVelocity.x = -m_vtVelocity.x * 0.9f;
			m_vForce.x = -m_vForce.x * 0.9f;
			m_bResolvingX = true;
		}
		// Hit on the top
		else if ( !m_bResolvingY && section.top == me.top && section.right == me.right && section.left == me.left )
		{
			m_vtVelocity.y = -m_vtVelocity.y * 0.9f;
			m_vForce.y = -m_vForce.y * 0.9f;
			m_bResolvingY = true;
		}
		// Hit on the bottom
		else if ( !m_bResolvingY && section.bottom == me.bottom && section.right == me.right && section.left == me.left )
		{
			m_vtVelocity.y = -m_vtVelocity.y * 0.9f;
			m_vForce.y = -m_vForce.y * 0.9f;
			m_bResolvingY = true;
		}
		// Hit on corner
		else if( !m_bResolvingX && !m_bResolvingY)
		{
			// Going Up
			if(m_vtVelocity.y < 0 )
			{
				if ( section.top == me.top )
				{
					m_vtVelocity.y = -m_vtVelocity.y * 0.9f;
					m_vForce.y = -m_vForce.y * 0.9f;
					m_bResolvingY = true;
				}
			}
			// Going Down
			else if(m_vtVelocity.y > 0)
			{
				if(section.bottom == me.bottom)
				{
					m_vtVelocity.y = -m_vtVelocity.y * 0.9f;
					m_vForce.y = -m_vForce.y * 0.9f;
					m_bResolvingY = true;
				}
			}

			// Going Left
			if(m_vtVelocity.x < 0)
			{
				if ( section.left == me.left )
				{
					m_vtVelocity.x = -m_vtVelocity.x * 0.9f;
					m_vForce.x = -m_vForce.x * 0.9f;
					m_bResolvingY = true;
				}
			}
			// Going Right
			else if(m_vtVelocity.x > 0)
			{
				if ( section.right == me.right )
				{
					m_vtVelocity.x = -m_vtVelocity.x * 0.9f;
					m_vForce.x = -m_vForce.x * 0.9f;
					m_bResolvingY = true;
				}			
			}			
		}
		
		m_rLastHit = hit;
	}
	else if (hit.size() == 0)
	{
		m_bResolvingX = false;
		m_bResolvingY = false;
	}
}

void TrickShotBullet::Render ()
{

	// Get the current frame
	int frame = GetSprite ()->GetCurrFrame ();

	// Find the center of the image
	SGD::Vector center;
	center.x = GetSprite ()->GetFrame ( frame ).GetFrameRect ().right - GetSprite ()->GetFrame ( frame ).GetFrameRect ().left;
	center.y = GetSprite ()->GetFrame ( frame ).GetFrameRect ().bottom - GetSprite ()->GetFrame ( frame ).GetFrameRect ().top;
	center.x /= 2;
	center.y /= 2;

	// Render
	AnimationManager::GetInstance ()->Render ( m_antsAnimation , m_ptPosition.x - Camera::x , m_ptPosition.y - Camera::y , m_fRotation , center );
}

void TrickShotBullet::HandleEvent(const SGD::Event * pEvent)
{
	if(pEvent->GetEventID() == "IM_HIT")
	{
		IEntity* pEntity = reinterpret_cast<IEntity*>(pEvent->GetSender());
		if(pEntity->GetRect().IsIntersecting(GetRect()))
		AddMyselfToTheHitList(pEntity);
	}
}

void TrickShotBullet::ApplyFriction ( float dt )
{
	float m_fStaticFriction;					// Static Friction
	SGD::Vector m_vKineticFriction;				// Kinetic Friction
	SGD::Vector m_vTangetForce;					// Tangential Component of the applied force
	SGD::Vector m_vTangent;						// Vector in the direction of the tangential component of the applied force


	// Compute the tangential component of the applied force
	m_vTangetForce = m_vForce;
	// Compute the tangent vector
	m_vTangent = m_vTangetForce.ComputeNormalized ();

	if ( m_vtVelocity.ComputeLength () == 0.0f )
	{
		// We have static state
		// Compute static friction, see note for formula
		m_fStaticFriction = m_fStaticFrictionCoefficient;
		// Set velocity to (0,0)
		m_vtVelocity = SGD::Vector ( 0 , 0 );
		// Compute the total force applid to the ball
		m_vTotalForces = SGD::Vector ( 0 , 0 );
	}

	if ( m_vtVelocity.ComputeLength () > 0.0f )
	{
		// Compute Kinetic friction
		m_vKineticFriction = m_vtVelocity.ComputeNormalized () * (-m_fDynamicFrictionCoefficient);
		// Set the Friction Force equal to the previously computed kinetic friction
		m_vFrictionForce = m_vKineticFriction;
		// Compute the total, including friction and drag force on the object
		m_vTotalForces = m_vForce + m_vFrictionForce + m_vDragForce;
	}

	if ( m_vtVelocity.ComputeLength () == 0.0f && m_vTangetForce.ComputeLength () >= m_fStaticFriction )
	{
		float m_fSlidingFactor;			// Sliding factor to make the transition from static to dynamic
		// Compute the sliding factor (sliding speed) that is the ration of the tangent force
		m_fSlidingFactor = m_vTangetForce.ComputeLength () / m_fStaticFriction;
		// Compute an initial velocity to break the transition from static friction to dynamic friction
		// that is velocity = sliding factor x direction of action (= direction of tangent force, computed as "tangent")
		m_vtVelocity = m_vTangent * m_fSlidingFactor;
		// Compute Kinetic(dynamic) friction
		m_vKineticFriction = m_vtVelocity.ComputeNormalized () * (-m_fDynamicFrictionCoefficient);
		// Set friction force equal to dynamic friction computed above
		m_vFrictionForce = m_vKineticFriction;
		// Compute total forces involved
		m_vTotalForces = m_vForce + m_vFrictionForce + m_vDragForce;
	}

	// Clamp velocity by its components to zero

	// If the X component of velocity is 0.1(close to 0), set it to 0
	if ( abs ( m_vtVelocity.x ) <= 0.5f )
		m_vtVelocity.x = 0.0f;
	// If the Y component of velocity is 0.1(close to 0), set it to 0
	if ( abs ( m_vtVelocity.y ) <= 0.5f )
		m_vtVelocity.y = 0.0f;

	// Decrease the force with a damping factor
	if ( abs ( m_vForce.x ) > 0.1f )
		m_vForce.x = m_vForce.x * 0.99f;
	else
		m_vForce.x = 0.0f;

	if ( abs ( m_vForce.y ) > 0.1f )
		m_vForce.y = m_vForce.y * 0.99f;
	else
		m_vForce.y = 0.0f;

}

void			TrickShotBullet::HandleCollision(const IEntity* pOther)
{
	if ( pOther->GetType () == ENT_ZOMBIE_BEAVER || pOther->GetType () == ENT_ZOMBIE_FAST ||
		pOther->GetType () == ENT_ZOMBIE_SLOW )
	{
		if(AmIOnTheHitList(pOther) == false)
		{
			m_vForce *= 0.9f;
			m_vtVelocity *= 0.9f;
		}
	}

}

bool TrickShotBullet::AmIOnTheHitList(const IEntity* pOther) const
{
	for ( unsigned int i = 0; i < m_vtHitList.size(); i++ )
		{
			if ( pOther == m_vtHitList[ i ] )
			{
				return true;
			}

		}
	return false;
}

void TrickShotBullet::UpdateHitList(void)
{
	if ( m_vtHitList.size () > 0 )
	{
		for ( int i = (int)m_vtHitList.size () - 1; i >= 0; i-- )
		{
			if ( m_vtHitList[ i ]->GetRect ().IsIntersecting ( GetRect () ) == false )
			{
				m_vtHitList[i]->Release();
				m_vtHitList.erase ( m_vtHitList.begin () + i );
			}
		}
	}
}

void TrickShotBullet::AddMyselfToTheHitList(IEntity* self)
{
	
		self->AddRef ();
		m_vtHitList.push_back ( self );
}

void TrickShotBullet::ClearHitList( void )
{
	for ( unsigned int i = 0; i < m_vtHitList.size(); i++ )
	{
		m_vtHitList[i]->Release();
	}
	m_vtHitList.clear();
}

/*********************************************************/
// Accessors

float			TrickShotBullet::GetRotation( void ) const
{
	return m_fRotation;
}

float			TrickShotBullet::GetMass ( void ) const
{
	return m_fMass;
}

float			TrickShotBullet::GetDynamicFrictionCoefficient ( void ) const
{
	return m_fDynamicFrictionCoefficient;
}

float			TrickShotBullet::GetStaticFrictionCoefficient ( void ) const
{
	return m_fStaticFrictionCoefficient;
}

float			TrickShotBullet::GetNormalForce ( void ) const
{
	return m_fNormalForce;
}

float			TrickShotBullet::GetDragForceCoefficient ( void ) const
{
	return m_fDragCoefficient;
}

float			TrickShotBullet::GetWeight ( void ) const
{
	return m_fWeight;
}

SGD::Vector		TrickShotBullet::GetDragForce ( void ) const
{
	return m_vDragForce;
}

SGD::Vector		TrickShotBullet::GetAcceleration ( void ) const
{
	return m_vAcceleration;
}

SGD::Vector		TrickShotBullet::GetForce ( void ) const
{
	return m_vForce;
}

SGD::Vector		TrickShotBullet::GetTotalForces ( void ) const
{
	return m_vTotalForces;
}

SGD::Vector		TrickShotBullet::GetFrictionForce ( void ) const
{
	return m_vFrictionForce;
}

/*********************************************************/
//Mutators

void			TrickShotBullet::SetRotation( float rotation )
{
	m_fRotation = rotation;
}

void			TrickShotBullet::SetMass ( float mass )
{
	m_fMass = mass;
}

void			TrickShotBullet::SetDynamicFrictionCoefficient ( float dfc )
{
	m_fDynamicFrictionCoefficient = dfc;
}

void			TrickShotBullet::SetStaticFrictionCoefficient ( float sfc )
{
	m_fStaticFrictionCoefficient = sfc;
}

void			TrickShotBullet::SetNormalForce ( float force )
{
	m_fNormalForce = force;
}

void			TrickShotBullet::SetWeight ( float weight )
{
	m_fWeight = weight;
}

void			TrickShotBullet::SetDragForceCoefficient ( float force )
{
	m_fDragCoefficient = force;
}

void			TrickShotBullet::SetAcceleration ( SGD::Vector accel )
{
	m_vAcceleration = accel;
}

void			TrickShotBullet::SetForce ( SGD::Vector force )
{
	m_vForce = force;
}

void			TrickShotBullet::SetTotalForces ( SGD::Vector force )
{
	m_vTotalForces = force;
}

void			TrickShotBullet::SetFrictionForce ( SGD::Vector force )
{
	m_vFrictionForce = force;
}

void			TrickShotBullet::SetDragForce ( SGD::Vector force )
{
	m_vDragForce = force;
}
