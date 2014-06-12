#include "Particle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "Player.h"
Particle::Particle()
{
	velocityOS = SGD::Vector { 0.0f , 0.0f };
	velocityOE = SGD::Vector {0.0f, 0.0f};
	velOverride = false;
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
	float percent = (currLifeTime / maxLifeTime)*dt;
	//change based on the rates
	Color.alpha		+=	(unsigned char)(((long)((percent*(particleFlyweight->endColor.alpha - particleFlyweight->startColor.alpha)))	)*dt)		;
	Color.red		+=	(unsigned char)(((long)((percent*(particleFlyweight->endColor.red - particleFlyweight->startColor.red)	))	)*dt)		;
	Color.green		+=	(unsigned char)(((long)((percent*(particleFlyweight->endColor.green - particleFlyweight->startColor.green)))	)*dt)		;
	Color.blue		+=	(unsigned char)(((long)((percent*(particleFlyweight->endColor.blue - particleFlyweight->startColor.blue)))	)*dt)		;
	if					(particleFlyweight->isSpread == false)
	{
		if ( !velOverride )
		{
			velocity.x -= (percent*(particleFlyweight->endVelocity.x - particleFlyweight->startVelocity.x))	*dt;
			velocity.y -= (percent*(particleFlyweight->endVelocity.y - particleFlyweight->startVelocity.y))	*dt;
		}
		else
		{
			velocity.x -= (percent*(velocityOE.x - velocityOS.x))	*dt;
			velocity.y -= (percent*(velocityOE.y - velocityOS.y))	*dt;
		}
	}
	else
	{
			velocity.x -= (percent*(velocityOE.x - velocityOS.x))	*dt;
			velocity.y -= (percent*(velocityOE.y - velocityOS.y))	*dt;
	}
	scale.width		-=	(percent*(particleFlyweight->startScale.width - particleFlyweight->endScale.width))	*dt					;
	scale.height	-=	(percent*(particleFlyweight->startScale.height - particleFlyweight->endScale.height))*dt				;
	position.x		+= velocity.x*dt;
	position.y		+= velocity.y*dt;
	rotation		-=	rotationRate*dt;
	//return true to signal that the particle is alive
	currLifeTime	-=	dt;
	return true;
}

void Particle::Render()
{
	//Offset position based on scale
	if (particleFlyweight->entity != nullptr)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(particleFlyweight->image,
		{ (position.x - (particleFlyweight->imageSize.width / 2)*scale.width) - Camera::x,
		(position.y - (particleFlyweight->imageSize.height / 2)*scale.height) - Camera::y},
		rotation, { particleFlyweight->imageSize.width / 2, particleFlyweight->imageSize.height / 2 }, Color, scale);
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(particleFlyweight->image,
		{ (position.x - (particleFlyweight->imageSize.width / 2)*scale.width),
		(position.y - (particleFlyweight->imageSize.height / 2)*scale.height) }, rotation, { particleFlyweight->imageSize.width / 2, particleFlyweight->imageSize.height / 2 }, Color, scale);
	}
}


