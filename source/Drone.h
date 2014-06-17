#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Handle.h"
class Player;
class EntityManager;


class Drone : public Entity
{
public:
	Drone								()										;
	~Drone								()										;
	virtual void	Update				(float dt)					override	;
	virtual void	Render				()							override	;
	virtual int		GetType				()					const	override	;
	virtual void	HandleCollision		(const IEntity* pOther)		override	;
	void			SetDamage			(int _Damage)							;
	void			SetNumberID			(int _ID)								;
	void			SetHealth			(int _Health)							;
	void			SetAngle			(float _Angle)							;
	void			SetNextShotTimer	(float _Timer)							;
	void			SetFireRate			(float _FireRate)						;
	void			SetBulletSpeed		(float _BulletSpeed)					;
	void			SetRotation			(float _Rotation)						;
	void			SetRange			(float _Range)							;
	void			SetPlayer			(Player* _player)						;
	void			SetEntityManager	(EntityManager* _EntityManager)			;
	void			SetDroneImage		(SGD::HTexture _DroneImage)				;
	int				GetDamage			()					const				;
	int				GetNumberID			()					const				;
	int				GetHealth			()					const				;
	float			GetNextShotTimer	()					const				;
	float			GetFireRate			()					const				;
	float			GetBulletSpeed		()					const				;
	float			GetRotation			()					const				;
	float			GetRange			()					const				;
	Player*			GetPlayer			()					const				;
	EntityManager*	GetEntityManager	()					const				;
	SGD::HTexture	GetDroneImage		()					const				;
	Drone*			Next														;
	Drone*			Previous													;
protected:
	Player*			m_pPlayer													;
	EntityManager*	m_pEntityManager											;
	SGD::HTexture	m_hDroneImage												;
	int				m_nDamage													;
	int				m_nNumberID													;
	int				m_nHealth													;
	float			m_fAngle													;
	float			m_fNextShotTimer											;
	float			m_fFireRate													;
	float			m_fBulletSpeed												;
	float			m_fRotation													;
	float			m_fRange													;
	float			m_fHitTimer = 1												;

};

