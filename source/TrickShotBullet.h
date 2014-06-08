#pragma once
#include "Projectile.h"
#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Listener.h"

class TrickShotBullet :
	public Projectile, public SGD::Listener
{
public:
	TrickShotBullet ();
	~TrickShotBullet ();

	//Interface
	virtual int GetType() const override;
	virtual void Render() override;
	virtual void Update(float dt) override;
	virtual void HandleCollision(const IEntity* pOther) override;
	virtual void HandleEvent(const SGD::Event * pEvent);
	void Bounce(SGD::Rectangle correct);
	void ApplyFriction(float dt);
	void UpdateHitList();
	void ClearHitList();

	//Accessors
	float			GetRotation(void) const;
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
	std::vector<IEntity *> GetHitList(void) const;
	bool			AmIOnTheHitList(const IEntity* self) const;

	//Mutators
	void	SetRotation							(float rotation);
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
	void	AddMyselfToTheHitList				(IEntity* self);


private:

	bool			m_bResolving;
	float			m_fRotation;
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
	SGD::Rectangle	m_rLastHit;

	std::vector<IEntity *> m_vtHitList;
};

