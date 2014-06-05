//***********************************************************************
//	File:		EntityManager.h
//	Author:		Doug Monroe
//	Course:		SGD 1403
//	Purpose:	EntityManager class stores & maintains all game entities
//***********************************************************************

#pragma once

class IEntity;			// uses IEntity*
#include <vector>		// uses std::vector

#include "../SGD Wrappers/SGD_Geometry.h"


//***********************************************************************
// EntityManager class
//	- stores references to game entities
//	- updates & renders all game entities
class EntityManager
{
public:
	//*******************************************************************
	// Default constructor & destructor
	EntityManager( void )	= default;
	~EntityManager( void )	= default;
	

	//*******************************************************************
	// Entity Storage:
	void	AddEntity	( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity );
	void	RemoveAll	( unsigned int bucket );
	void	RemoveAll	( void );
	

	//*******************************************************************
	// Entity Upkeep:
	void	UpdateAll	( float elapsedTime );
	void	RenderAll	( void );

	void	CheckCollisions( unsigned int bucket1, unsigned int bucket2 );
	IEntity* CheckCollision(SGD::Rectangle rect, int bucket = -1);
	IEntity* CheckCollision(SGD::Point point, float radius, int bucket = -1);

	const std::vector<IEntity*> GetBucket(int index) const;

private:
	//*******************************************************************
	// Not a singleton, but still don't want the Trilogy-of-Evil
	EntityManager( const EntityManager& )				= delete;
	EntityManager& operator= ( const EntityManager& )	= delete;

	
	//*******************************************************************
	// Typedefs will simplify the templates
	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;
	
	//*******************************************************************
	// members:
	EntityTable	m_tEntities;			// vector-of-vector-of-IEntity* (2D table)
	bool		m_bIterating = false;	// read/write lock
};

