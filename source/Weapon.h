#pragma once
class Projectile;

enum Guns {TYPE_ASSAULT_RIFLE,TYPE_SHOTGUN,TYPE_ROCKET_LAUNCHER, TYPE_TRICKSHOT};
class Weapon
{
public:
	Weapon();
	~Weapon();
	/*************ACCESSORS******************************/
	float GetFireRate() const;
	float GetFireRateCap() const;

	int GetCurrAmmo() const;
	int GetMaxAmmo() const;
	int GetMaxAmmoCap() const;

	float GetFireTimer() const;
	//Projectile* GetProjectile() const;
	Guns GetType() const;
	/*************MUTATORS*******************************/
	void SetFireRate(float _FireRate);
	void SetFireRateCap(float cap);
	void SetFireTimer(float _FireTimer);
	void SetCurrAmmo(int _CurrAmmo);
	void SetMaxAmmo(int _MaxAmmo);
	void SetMaxAmmoCap(int cap);
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

	/* Maximum UPGRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADEs*/


	float m_fFireRateCap;	// Maximum Firerate
	int m_nMaxAmmoCap;	// Max Ammo Cap
};

