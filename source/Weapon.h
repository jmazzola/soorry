#pragma once
class Projectile;
enum Guns {TYPE_ASSAULT_RIFLE,TYPE_SHOTGUN,TYPE_ROCKET_LAUNCHER};
class Weapon
{
public:
	Weapon();
	~Weapon();
	/*************ACCESSORS******************************/
	float GetFireRate() const;
	int GetCurrAmmo() const;
	int GetMaxAmmo() const;
	//Projectile* GetProjectile() const;
	Guns GetType() const;
	/*************MUTATORS*******************************/
	void SetFireRate(float _FireRate);
	void SetCurrAmmo(int _CurrAmmo);
	void SetMaxAmmo(int _MaxAmmo);
	//void SetProjectile(Projectile* _Projectile);
	void SetType(Guns _Type);
	 
protected:
	/************DATA_MEMBERS****************************/
	float m_fFireRate;
	int m_nCurrentAmmo;
	int m_nMaxAmmo;
	//Projectile* m_pProjectile;
	Guns m_eType;
};

