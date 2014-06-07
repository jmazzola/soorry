#include "Grenade.h"
#include <math.h>
#include "AnimationManager.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Camera.h"
#include "Sprite.h"

Grenade::Grenade ()
{
}


Grenade::~Grenade ()
{
}

int Grenade::GetType ( void ) const
{
	return ENT_GRENADE;
}

void Grenade::Update ( float dt )
{
	m_fRotation += dt;
	m_fDetonationTimer -= dt;

	if(m_fDetonationTimer < 0)
	{
		// Send the explosion event
		SGD::Event e("GRENADE_EXPLOSION", nullptr, this);
		e.SendEventNow();

		// Explode
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		// Queue the message
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

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
}

void Grenade::Render()
{
	
	// Get the current frame
	int frame = GetSprite()->GetCurrFrame();

	// Find the center of the image
	SGD::Vector center;
	center.x = GetSprite()->GetFrame(frame).GetFrameRect().right - GetSprite()->GetFrame(frame).GetFrameRect().left;
	center.y = GetSprite()->GetFrame(frame).GetFrameRect().bottom - GetSprite()->GetFrame(frame).GetFrameRect().top;
	center.x /= 2;
	center.y /= 2;

	// Render
	AnimationManager::GetInstance()->Render(m_antsAnimation, m_ptPosition.x - Camera::x, m_ptPosition.y - Camera::y, m_fRotation, center);
}

void Grenade::ApplyFriction ( float dt )
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
	if ( abs ( m_vtVelocity.x ) <= 0.1f )
		m_vtVelocity.x = 0.0f;
	// If the Y component of velocity is 0.1(close to 0), set it to 0
	if ( abs ( m_vtVelocity.y ) <= 0.1f )
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

/*********************************************************/
// Accessors

float			Grenade::GetDamage ( void ) const
{
	return m_fDamage;
}

float			Grenade::GetRadius ( void ) const
{
	return m_fRadius;
}

float			Grenade::GetRotation( void ) const
{
	return m_fRotation;
}

float			Grenade::GetDetonationTimer(void) const
{
	return m_fDetonationTimer;
}

float			Grenade::GetDetonationLength(void) const
{
	return m_fDetonationLength;
}

float			Grenade::GetMass ( void ) const
{
	return m_fMass;
}

float			Grenade::GetDynamicFrictionCoefficient ( void ) const
{
	return m_fDynamicFrictionCoefficient;
}

float			Grenade::GetStaticFrictionCoefficient ( void ) const
{
	return m_fStaticFrictionCoefficient;
}

float			Grenade::GetNormalForce ( void ) const
{
	return m_fNormalForce;
}

float			Grenade::GetDragForceCoefficient ( void ) const
{
	return m_fDragCoefficient;
}

float			Grenade::GetWeight ( void ) const
{
	return m_fWeight;
}

SGD::Vector		Grenade::GetDragForce ( void ) const
{
	return m_vDragForce;
}

SGD::Vector		Grenade::GetAcceleration ( void ) const
{
	return m_vAcceleration;
}

SGD::Vector		Grenade::GetForce ( void ) const
{
	return m_vForce;
}

SGD::Vector		Grenade::GetTotalForces ( void ) const
{
	return m_vTotalForces;
}

SGD::Vector		Grenade::GetFrictionForce ( void ) const
{
	return m_vFrictionForce;
}

/*********************************************************/
//Mutators

void			Grenade::SetDamage ( float damage )
{
	m_fDamage = damage;
}

void			Grenade::SetRadius ( float radius )
{
	m_fRadius = radius;
}

void			Grenade::SetRotation( float rotation )
{
	m_fRotation = rotation;
}

void			Grenade::SetDetonationLength(float length)
{
	m_fDetonationLength = length;
}

void			Grenade::SetDetonationTimer(float timer)
{
	m_fDetonationTimer = timer;
}

void			Grenade::SetMass ( float mass )
{
	m_fMass = mass;
}

void			Grenade::SetDynamicFrictionCoefficient ( float dfc )
{
	m_fDynamicFrictionCoefficient = dfc;
}

void			Grenade::SetStaticFrictionCoefficient ( float sfc )
{
	m_fStaticFrictionCoefficient = sfc;
}

void			Grenade::SetNormalForce ( float force )
{
	m_fNormalForce = force;
}

void			Grenade::SetWeight ( float weight )
{
	m_fWeight = weight;
}

void			Grenade::SetDragForceCoefficient ( float force )
{
	m_fDragCoefficient = force;
}

void			Grenade::SetAcceleration ( SGD::Vector accel )
{
	m_vAcceleration = accel;
}

void			Grenade::SetForce ( SGD::Vector force )
{
	m_vForce = force;
}

void			Grenade::SetTotalForces ( SGD::Vector force )
{
	m_vTotalForces = force;
}

void			Grenade::SetFrictionForce ( SGD::Vector force )
{
	m_vFrictionForce = force;
}

void			Grenade::SetDragForce ( SGD::Vector force )
{
	m_vDragForce = force;
}
