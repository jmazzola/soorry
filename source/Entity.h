/***************************************************************
|	File:		Entity.h
|	Author:		
|	Course:		
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#ifndef ENTITY_H
#define ENTITY_H


#include "IEntity.h"
#include "AnimationManager.h"

/**********************************************************/
// Forward Declarations
class Sprite;


/**************************************************************/
// Entity class
//	- parent class of all game entities, stores the shared data members
//	- velocity-based movement
class Entity : public IEntity
{
public:
	/**********************************************************/
	// MUST have a virtual destructor to allow upcasted pointers
	// to trigger children destructors at deallocation
	Entity( void )			= default;	// default constructor
	virtual ~Entity( void )	= default;	// VIRTUAL destructor

	
	/**********************************************************/
	// Entity Types:
	enum EntityType 
	{ 
		ENT_BASE,
			ENT_PLAYER,
			ENT_ENEMY,
				ENT_ZOMBIE_FAST,
				ENT_ZOMBIE_SLOW,
				ENT_ZOMBIE_BEAVER,
			ENT_PROJECTILE,
				ENT_BULLET_ROCKET,
				ENT_BULLET_ASSAULT,
				ENT_BULLET_SHOTGUN,
				ENT_BULLET_TRICKSHOT,
			ENT_PICKUP,
				ENT_PICKUP_AMMO,
				ENT_PICKUP_HEALTHPACK,
				ENT_PICKUP_SUPER,
				ENT_PICKUP_WALL,
				ENT_PICKUP_WINDOW,
			ENT_TRAP,
				ENT_TRAP_BEARTRAP,
				ENT_TRAP_MINE,
				ENT_TRAP_SPIKE,
				ENT_TRAP_LAVA,
			ENT_GRENADE,
			ENT_TOWER,
				ENT_TOWER_MACHINE_GUN,
				ENT_TOWER_MAPLE_SYRUP,
				ENT_TOWER_HOCKEY_STICK,
				ENT_TOWER_LASER,
			ENT_MACHINE_GUN_BULLET,
			ENT_MAPLE_SYRUP_BULLET,
			ENT_DRONE,
	};

	
	/**********************************************************/
	// Interface Methods
	virtual void Update	(float dt) override;
	virtual void Render () override;
	virtual void PostRender() override;
	virtual SGD::Rectangle GetRect() const override;
	virtual int GetType() const override;
	virtual void HandleCollision (const IEntity* pOther) override;
	

	// Children classes CANNOT override a 'final' method.
	virtual void AddRef() final;
	virtual void Release() final;

	
	/**********************************************************/
	// Accessors
	Sprite* GetSprite() const;
	SGD::Point GetPosition() const;
	SGD::Vector GetVelocity() const;
	float GetTimeOfFrame() const { return m_antsAnimation.m_fTimeOnFrame; }
	int GeCurrFrame() const { return m_antsAnimation.m_nCurrFrame; }
	std::string GetCurrAnimation() const{ return m_antsAnimation.m_nCurrAnimation; }
	/**********************************************************/
	// Mutators
	void SetSprite(Sprite* sprite);
	void SetPosition(SGD::Point position);
	void SetVelocity(SGD::Vector velocity);
	void SetTimeOfFrame(float time) { m_antsAnimation.m_fTimeOnFrame = time; }
	void SetCurrFrame(int numframe) { m_antsAnimation.m_nCurrFrame = numframe; }
	void SetCurrAnimation(std::string name) { m_antsAnimation.m_nCurrAnimation = name; }

protected:

	/**********************************************************/
	// Members:
	Sprite* m_pSprite;
	SGD::Point m_ptPosition;
	SGD::Vector m_vtVelocity;
	AnimationTimestamp m_antsAnimation;

private:

	/**********************************************************/
	// reference count
	unsigned int		m_unRefCount	= 1;
};

#endif //ENTITY_H
