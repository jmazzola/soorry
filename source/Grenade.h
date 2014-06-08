#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Declarations.h"

class Grenade :
	public Entity
{
public:
	Grenade ();
	~Grenade ();

	//Interface
	virtual int GetType() const override;
	virtual void Update(float dt) override;
	virtual void Render() override;
	void ApplyFriction(float dt);

	//Accessors
	float			GetDamage ( void ) const;
	float			GetRadius ( void ) const;
	float			GetRotation(void) const;
	float			GetDetonationTimer(void) const;
	float			GetDetonationLength(void) const;
	float			GetMass ( void ) const;
	float			GetDynamicFrictionCoefficient ( void ) const;
	float			GetStaticFrictionCoefficient ( void ) const;
	float			GetNormalForce ( void ) const;
	float			GetDragForceCoefficient ( void ) const;
	float			GetWeight ( void ) const;
	SGD::Vector		GetDragForce ( void ) const;
	SGD::Vector		GetAcceleration ( void ) const;
	SGD::Vector		GetForce ( void ) const;
	SGD::Vector		GetTotalForces ( void ) const;
	SGD::Vector		GetFrictionForce ( void ) const;

	//Mutators
	void	SetDamage							(float damage);
	void	SetRadius							(float radius);
	void	SetRotation							(float rotation);
	void	SetDetonationTimer					(float timer);
	void	SetDetonationLength					(float length);
	void	SetMass								(float mass);
	void	SetDynamicFrictionCoefficient		(float dfc);
	void	SetStaticFrictionCoefficient		(float sfc);	
	void	SetNormalForce						(float force);		
	void	SetWeight							(float weight);		
	void	SetDragForceCoefficient				(float force);
	void	SetAcceleration						(SGD::Vector accel);	
	void	SetForce							(SGD::Vector force);	
	void	SetTotalForces						(SGD::Vector force);	
	void	SetFrictionForce					(SGD::Vector force);	
	void	SetDragForce						(SGD::Vector force);

private:

	float			m_fDamage;
	float			m_fRadius;
	float			m_fRotation;
	float			m_fDetonationTimer;
	float			m_fDetonationLength;
	float			m_fMass;
	float			m_fDynamicFrictionCoefficient;
	float			m_fStaticFrictionCoefficient;
	float			m_fWeight;
	float			m_fNormalForce;
	float			m_fSpeed;
	float			m_fRho;
	float			m_fDragCoefficient;
	SGD::Vector		m_vAcceleration;
	SGD::Vector		m_vForce;
	SGD::Vector		m_vTotalForces;
	SGD::Vector		m_vFrictionForce;
	SGD::Vector		m_vDragForce;
};

