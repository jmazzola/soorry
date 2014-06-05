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
	float GetFireTimer() const;
	//Projectile* GetProjectile() const;
	Guns GetType() const;
	/*************MUTATORS*******************************/
	void SetFireRate(float _FireRate);
	void SetFireTimer(float _FireTimer);
	void SetCurrAmmo(int _CurrAmmo);
	void SetMaxAmmo(int _MaxAmmo);
	//void SetProjectile(Projectile* _Projectile);
	void SetType(Guns _Type);
	 
protected:
	/************DATA_MEMBERS****************************/
	float m_fFireRate;
	float m_fFireTimer;
	int m_nCurrentAmmo;
	int m_nMaxAmmo;
	//Projectile* m_pProjectile;
	Guns m_eType;
};

