#pragma once
#include "Tower.h"
class LavaTrap :
	public Tower
{
public:
	LavaTrap ();
	~LavaTrap ();

	/**********************************************************/
	// Interface Methods
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual int  GetType() const override;

	/**********************************************************/
	// Accessors
	float GetDamage(void) const;
	float GetImageSwitchRate( void ) const;
	float GetImageSwitchTimer( void ) const;
	bool  GetRenderTop( void ) const;

	/**********************************************************/
	// Mutators
	void SetDamage( float damage );
	void SetImageSwitchRate( float rate );
	void SetImageSwitchTimer( float time );
	void SetRenderTop( bool choice );

private:

	float m_fImageSwitchRate;
	float m_fImageSwitchTimer;
	float m_fDamage;
	float m_fDt;
	bool m_bRenderTop;
};

