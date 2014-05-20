#include "Emitter.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "Particle.h"
#include <ctime>
Emitter::Emitter()
{
}

Emitter::~Emitter()
{
	for (unsigned int i = aliveParticles.size(); i > 0; i--)
	{
		delete aliveParticles[i-1];
	}
	if (aliveParticles.size() != 0)
		aliveParticles.clear();
	for (unsigned int i = deadParticles.size(); i > 0; i--)
	{
		delete deadParticles[i-1];
	}
	if (deadParticles.size() != 0)
		deadParticles.clear();
	delete particleFlyweight;
}

void Emitter::load()
{
	srand((unsigned int)time(nullptr));
	if (allParticlesCreated != true)
	{
		
		for (int i = 0; i < maxParticles; i++)
		{
			//Create a new particle
			Particle* tempParticle = new Particle;
			//set values
			float maxLife = particleFlyweight->maxLifeTime;
			//Take data from particle flyweight and send it to the particle
			tempParticle->Color = particleFlyweight->startColor;
			//Create rates to update particles
			tempParticle->maxLifeTime = rand() % (int)particleFlyweight->maxLifeTime + particleFlyweight->minLifeTime;
			tempParticle->currLifeTime = 0;
			tempParticle->colorRateA = (((float)particleFlyweight->startColor.alpha - (float)particleFlyweight->endColor.alpha) / tempParticle->maxLifeTime);
			tempParticle->colorRateR = (((float)particleFlyweight->startColor.red - (float)particleFlyweight->endColor.red) / tempParticle->maxLifeTime);
			tempParticle->colorRateG = (((float)particleFlyweight->startColor.green - (float)particleFlyweight->endColor.green) / tempParticle->maxLifeTime);
			tempParticle->colorRateB = (((float)particleFlyweight->startColor.blue - (float)particleFlyweight->endColor.blue) / tempParticle->maxLifeTime);
			tempParticle->scale = particleFlyweight->startScale;
			tempParticle->scaleRateX = ((particleFlyweight->startScale.width - particleFlyweight->endScale.width) / tempParticle->maxLifeTime);
			tempParticle->scaleRateY = ((particleFlyweight->startScale.height - particleFlyweight->endScale.height) / tempParticle->maxLifeTime);
			tempParticle->velocity = particleFlyweight->startVelocity;
			tempParticle->velocityRateX = ((particleFlyweight->startVelocity.x - particleFlyweight->endVelocity.x) / tempParticle->maxLifeTime);
			tempParticle->velocityRateY = ((particleFlyweight->startVelocity.y - particleFlyweight->endVelocity.y) / tempParticle->maxLifeTime);
			tempParticle->rotation = particleFlyweight->startRotation;
			tempParticle->rotationRate = ((particleFlyweight->startRotation - particleFlyweight->endRotation) / tempParticle->maxLifeTime);
			//Randomize the position within the emitter NOTE: maybe need to add world offset
			switch (shape)
			{
			case 0://square
			{
				tempParticle->position.x = (float)(rand() % (int)size.width) + position.x;
				tempParticle->position.y = (float)(rand() % (int)size.height) + position.y;
			}
				break;
			case 1://circle
			{
				tempParticle->position.x = (float)(rand() % (int)size.width) + position.x;
				tempParticle->position.y = (float)(rand() % (int)size.width) + position.y;
			}
				break;
			case 2://line
			{
				tempParticle->position.x = (float)(rand() % (int)size.width) + position.x;
				tempParticle->position.y = (float)(rand() % (int)size.height) + position.y;
			}
				break;
			case 3://point
			{
				tempParticle->position.x = (float)(rand() % (int)size.width) + position.x;
				tempParticle->position.y = (float)(rand() % (int)size.height) + position.y;
			}
				break;
			}
	
			tempParticle->particleFlyweight = particleFlyweight;
			//add it to dead particles
			deadParticles.push_back(tempParticle);
			spawnRate = deadParticles.size() / ((particleFlyweight->maxLifeTime + particleFlyweight->minLifeTime) / 2);
		}
		allParticlesCreated = true;
	}
}

void Emitter::Update(float dt)
{
	if (isLooping)
	{
		if (aliveParticles.size() == 0 && deadParticles.size() == 0)
			return;
		for (float i = 0; i < spawnRate*dt; i++)
		{
			//create Particle then add it to the alive particles
			//Take data from patricle flyweight and send it to the particle
			if (aliveParticles.size() < (unsigned int)maxParticles)
			{
				Particle* tempParticle = deadParticles.front();
				tempParticle->currLifeTime = 0;
				tempParticle->velocity = particleFlyweight->startVelocity;
				tempParticle->position.x = (float)(rand() % (int)size.width) + position.x;
				tempParticle->position.y = (float)(rand() % (int)size.height) + position.y;
				tempParticle->Color = particleFlyweight->startColor;
				tempParticle->scale = particleFlyweight->startScale;
				tempParticle->rotation = particleFlyweight->startRotation;
				aliveParticles.push_back(tempParticle);
				deadParticles.erase(deadParticles.begin());
			}
		}
	}
	else
	{
		for (float i = 0; i < spawnRate*dt; i++)
		{
			if (deadParticles.size() == 0)
				break;
			aliveParticles.push_back(deadParticles.front());
			deadParticles.erase(deadParticles.begin());
		}
	}
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		//check if the particle is dead
		bool update = aliveParticles[i]->Update(dt);
		if (!update && isLooping)
		{
			//move the particle into the dead list
			deadParticles.push_back(aliveParticles[i]);
			aliveParticles.erase(aliveParticles.begin() + i);
			i--;
		}
		else if (!update &&!isLooping)
		{
			delete aliveParticles[i];
			aliveParticles.erase(aliveParticles.begin()+i);
			i--;
		}
	}
}
void Emitter::Render()
{
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		aliveParticles[i]->Render();
	}
}

