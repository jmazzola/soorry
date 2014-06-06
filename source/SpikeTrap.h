#pragma once
#include "Tower.h"
class SpikeTrap :
	public Tower
{
public:
	SpikeTrap ();
	~SpikeTrap ();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual int GetType() const override;
	bool GetActive( void ) const;

	/**********************************************************/
	// Accessors
	float GetAttackTimer( void ) const;
	float GetAttackCD( void ) const;
	float GetHoldTimer( void ) const;
	float GetHoldLength( void ) const;
	float GetDamage(void) const;

	/**********************************************************/
	// Mutators
	void SetAttackTimer( float time );
	void SetAttackCD( float length );
	void SetHoldTimer( float time );
	void SetHoldLength( float length );
	void SetDamage( float damage );
private:
	
	/**********************************************************/
	// Data members
	float m_fAttackTimer;
	float m_fAttackCD;
	float m_fHoldTimer;
	float m_fHoldLength;
	float m_fDamage;

};

