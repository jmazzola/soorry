#include "Particle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
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
	if (currLifeTime <= 0)
		return false;
	float percent = currLifeTime / maxLifeTime;
	//change based on the rates
	Color.alpha		-=	(char)((percent*(particleFlyweight->endColor.alpha - particleFlyweight->startColor.alpha))	*dt)		;
	Color.red		-=	(char)((percent*(particleFlyweight->endColor.red - particleFlyweight->startColor.red)	 )	*dt)		;
	Color.green		-=	(char)((percent*(particleFlyweight->endColor.green - particleFlyweight->startColor.green))	*dt)		;
	Color.blue		-=	(char)((percent*(particleFlyweight->endColor.green - particleFlyweight->startColor.green))	*dt)		;
	if					(particleFlyweight->isSpread == false)
	{
		velocity.x -=	(percent*(particleFlyweight->endVelocity.x - particleFlyweight->startVelocity.x))	*dt					;
		velocity.y -=	(percent*(particleFlyweight->endVelocity.y - particleFlyweight->startVelocity.y))	*dt					;
	}
	scale.width		-=	(percent*(particleFlyweight->startScale.width - particleFlyweight->endScale.width))	*dt					;
	scale.height	-=	(percent*(particleFlyweight->startScale.height - particleFlyweight->endScale.height))*dt				;
	position.x		+=	velocity.x*dt;
	position.y		+=	velocity.y*dt;
	rotation		-=	rotationRate*dt;
	//return true to signal that the particle is alive
	currLifeTime	-=	dt;
	return true;
}

void Particle::Render()
{
	//Offset position based on scale
	SGD::GraphicsManager::GetInstance()->DrawTexture(particleFlyweight->image, { position.x - (particleFlyweight->imageSize.width / 2)*scale.width, position.y - (particleFlyweight->imageSize.height / 2)*scale.height }, rotation, { particleFlyweight->imageSize.width / 2, particleFlyweight->imageSize.height / 2 }, Color, scale);
}


