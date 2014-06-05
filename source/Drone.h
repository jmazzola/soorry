#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Handle.h"
class Player;
class EntityManager;


class Drone : public Entity
{
public:
	Drone();
	~Drone();
	virtual void	Update(float dt)						override;
	virtual void	Render()								override;
	virtual int		GetType() const							override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
	void			SetDamage(int _Damage);
	void			SetNumberID(int _ID);
	void			SetNextShotTimer(float _Timer);
	void			SetFireRate(float _FireRate);
	void			SetBulletSpeed(float _BulletSpeed);
	void			SetRotation(float _Rotation);
	void			SetRange(float _Range);
	void			SetPlayer(Player* _player);
	void			SetEntityManager(EntityManager* _EntityManager);
	void			SetDroneImage(SGD::HTexture _DroneImage);
	int				GetDamage();
	int				GetNumberID();
	float			GetNextShotTimer();
	float			GetFireRate();
	float			GetBulletSpeed();
	float			GetRotation();
	float			GetRange();
	Player*			GetPlayer();
	EntityManager*	GetEntityManager();
	SGD::HTexture	GetDroneImage();

protected:
	Player*			m_pPlayer;
	EntityManager*	m_pEntityManager;
	SGD::HTexture	m_hDroneImage;
	int				m_nDamage;
	int				m_nNumberID;
	float			m_fNextShotTimer;
	float			m_fFireRate;
	float			m_fBulletSpeed;
	float			m_fRotation;
	float			m_fRange;

};

