#include "Weapon.h"



Weapon::Weapon()
{
	m_fFireTimer = 0.0f;
}


Weapon::~Weapon()
{
}

/*************ACCESSORS******************************/
float Weapon::GetFireRate() const
{
	return m_fFireRate;
}

float Weapon::GetFireRateCap() const
{
	return m_fFireRateCap;
}

int	Weapon::GetCurrAmmo() const
{
	return m_nCurrentAmmo;
}

int	Weapon::GetMaxAmmo() const
{
	return m_nMaxAmmo;
}

int	Weapon::GetMaxAmmoCap() const
{
	return m_nMaxAmmoCap;
}

float Weapon::GetFireTimer() const
{
	return m_fFireTimer;
}
//Projectile* Weapon::GetProjectile() const
//{
//	return m_pProjectile;
//}
Guns Weapon::GetType() const
{
	return m_eType;
}
/*************MUTATORS*******************************/
void Weapon::SetFireRate(float _FireRate)
{
	if (_FireRate < m_fFireRateCap)
		m_fFireRate = m_fFireRateCap;
	else
		m_fFireRate = _FireRate;
}

void Weapon::SetFireRateCap(float cap)
{
	m_fFireRateCap = cap;
}

void Weapon::SetCurrAmmo(int _CurrAmmo)
{
	if (_CurrAmmo > m_nMaxAmmo)
		m_nCurrentAmmo = m_nMaxAmmo;
	else 
		m_nCurrentAmmo = _CurrAmmo;
}
void Weapon::SetMaxAmmo(int _MaxAmmo)
{
	if (_MaxAmmo > m_nMaxAmmoCap)
		m_nMaxAmmo = m_nMaxAmmoCap;
	else
		m_nMaxAmmo = _MaxAmmo;
}

void Weapon::SetMaxAmmoCap(int cap)
{
	m_nMaxAmmoCap = cap;
}

void Weapon::SetFireTimer(float _FireTimer)
{
	m_fFireTimer = _FireTimer;
}

void Weapon::SetType(Guns _Type)
{
	m_eType = _Type;
}