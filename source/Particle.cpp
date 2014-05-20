#include "Particle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
Particle::Particle()
{

}


Particle::~Particle()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(image);
}

//Returns false if dead returns true if alive
bool Particle::Update(float dt)
{
	//if statement to determine if the particle is dead
	if (currLifeTime > maxLifeTime)
		return false;
	//change based on the rates
	Color.alpha		-=	(char)colorRateA;
	Color.red		-=	(char)colorRateR;
	Color.green		-=	(char)colorRateG;
	Color.blue		-=	(char)colorRateB;
	if (velocity.x < particleFlyweight->endVelocity.x)
		velocity.x	-=	velocityRateX;
	if (velocity.y < particleFlyweight->endVelocity.y)
	velocity.y		-=	velocityRateY;
	scale.x			-=	scaleRateX;
	scale.y			-=	scaleRateY;
	position.x		+=	velocity.x;
	position.y		+=	velocity.y;
	currLifeTime	+=	dt;
	//return true to signal that the particle is alive
	return true;
}

void Particle::Render()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(particleFlyweight->image, position);
}


