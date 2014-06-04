/***************************************************************
|	File:		IGameState.h
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	IGameState class declares the interface that all
|				child states must implement
***************************************************************/

#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#define TRANSITION_TIME 1.0f


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

	// Accessor
	bool IsTransitioning() const { return m_bIsTransitioning; }

	// Mutator
	void SetTransition(bool b) { m_bIsTransitioning = b; }

	// Transition time
	float m_fTransitionTime = TRANSITION_TIME;

private:

	// Is transitioning
	bool m_bIsTransitioning;
};

#endif //IGAMESTATE_H
