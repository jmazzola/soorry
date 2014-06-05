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
	if (currLifeTime > maxLifeTime)
		return false;
	//change based on the rates
	Color.alpha		-=	(char)((float)colorRateA*dt);
	Color.red		-=	(char)((float)colorRateR*dt);
	Color.green		-=	(char)((float)colorRateG*dt);
	Color.blue		-=	(char)((float)colorRateB*dt);
	velocity.x		-=	velocityRateX*dt;
	velocity.y		-=	velocityRateY*dt;
	scale.width		-=	scaleRateX*dt;
	scale.height	-=	scaleRateY*dt;
	position.x		+=	velocity.x*dt;
	position.y		+=	velocity.y*dt;
	rotation		-=	rotationRate*dt;
	//return true to signal that the particle is alive
	currLifeTime	+=	dt;
	return true;
}

void Particle::Render()
{
	//Offset position based on scale
	SGD::GraphicsManager::GetInstance()->DrawTexture(particleFlyweight->image, { position.x - (particleFlyweight->imageSize.width / 2)*scale.width, position.y - (particleFlyweight->imageSize.height / 2)*scale.height }, rotation, { particleFlyweight->imageSize.width / 2, particleFlyweight->imageSize.height / 2 }, Color, scale);
}


