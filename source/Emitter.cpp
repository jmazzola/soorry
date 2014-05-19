#include "Emitter.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "Particle.h"
Emitter::Emitter()
{
	//Create all the particle needed for this emitter
	
}

Emitter::~Emitter()
{

}

void Emitter::Update(float dt)
{
	if (allParticlesCreated != true)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			Particle* tempParticle = new Particle;
			deadParticles.push_back(tempParticle);
		}
		allParticlesCreated = true;
	}
	//Loop for the amount of particles made every second
	for (float i = 0; i < spawnRate; i++)
	{
		//create Particle then add it to the alive particles
		//Take data from patricle flyweight and send it to the particle
		/*Particle* tempParticle = new Particle;
		aliveParticles.push_back(deadParticles.front());
		deadParticles.erase(deadParticles.begin());*/

		//particleFlyweight.startColor
		//aliveParticles.push_back()
	}
}
void Emitter::Render()
{
	
}