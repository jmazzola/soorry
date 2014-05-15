/***************************************************************
|	File:		IGameState.h
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	IGameState class declares the interface that all
|				child states must implement
***************************************************************/

#ifndef IGAMESTATE_H
#define IGAMESTATE_H


/**************************************************************/
// IGameState class
//	- abstract base class!
//	- declares interface for game state
class IGameState
{
public:
	/**********************************************************/
	// Virtual Destructor
	virtual ~IGameState( void )	= default;


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter( void )	= 0;	// load resources
	virtual void	Exit ( void )	= 0;	// unload resources

	virtual bool	Input( void )	= 0;	// handle user input
	virtual void	Update( float elapsedTime )	= 0;	// update entites
	virtual void	Render( void )	= 0;	// render entities / menu
};

#endif //IGAMESTATE_H
