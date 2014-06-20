#include "Weapon.h"


Weapon::Weapon()
{
	m_fFireTimer = 0.0f;
}


Weapon::~Weapon()
{
}

/*************ACCESSORS******************************/
float		Weapon::GetFireRate() const
{
	return m_fFireRate;
}
int			Weapon::GetCurrAmmo() const
{
	return m_nCurrentAmmo;
}
int			Weapon::GetMaxAmmo() const
{
	return m_nMaxAmmo;
}
float		Weapon::GetFireTimer() const
{
	return m_fFireTimer;
}
//Projectile* Weapon::GetProjectile() const
//{
//	return m_pProjectile;
//}
Guns		Weapon::GetType() const
{
	return m_eType;
}
/*************MUTATORS*******************************/
void Weapon::SetFireRate(float _FireRate)
{
	m_fFireRate = _FireRate;
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
	m_nMaxAmmo = _MaxAmmo;
}
void Weapon::SetFireTimer(float _FireTimer)
{
	m_fFireTimer = _FireTimer;
}
//void Weapon::SetProjectile(Projectile* _Projectile)
//{
//	m_pProjectile = _Projectile;
//}
void Weapon::SetType(Guns _Type)
{
	m_eType = _Type;
}