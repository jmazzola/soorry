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
void Emitter::load()
{
	if (allParticlesCreated != true)
	{
		for (int i = 0; i < maxParticles; i++)
		{
			//Create a new particle
			Particle* tempParticle = new Particle;
			//set values
			float maxLife = particleFlyweight.maxLifeTime;
			//Take data from particle flyweight and send it to the particle
			tempParticle->Color = particleFlyweight.startColor;
			//Create rates to update particles
			tempParticle->colorRateA = (((float)particleFlyweight.startColor.alpha - (float)particleFlyweight.endColor.alpha) / maxLife);
			tempParticle->colorRateR = (((float)particleFlyweight.startColor.red - (float)particleFlyweight.endColor.red) / maxLife);
			tempParticle->colorRateG = (((float)particleFlyweight.startColor.green - (float)particleFlyweight.endColor.green) / maxLife);
			tempParticle->colorRateB = (((float)particleFlyweight.startColor.blue - (float)particleFlyweight.endColor.blue) / maxLife);
			tempParticle->maxLifeTime = maxLife;
			tempParticle->currLifeTime = 0;
			tempParticle->scale = particleFlyweight.startScale;
			tempParticle->scaleRateX = ((particleFlyweight.startScale.x - particleFlyweight.endScale.x) / maxLife);
			tempParticle->scaleRateY = ((particleFlyweight.startScale.y - particleFlyweight.endScale.y) / maxLife);
			tempParticle->velocity = particleFlyweight.startVelocity;
			tempParticle->velocityRateX = ((particleFlyweight.startVelocity.x - particleFlyweight.endVelocity.x) / maxLife);
			tempParticle->velocityRateY = ((particleFlyweight.startVelocity.y - particleFlyweight.endVelocity.y) / maxLife);
			tempParticle->particleFlyweight = particleFlyweight;
			//add it to dead particles
			deadParticles.push_back(tempParticle);
		}
		allParticlesCreated = true;
	}
}

void Emitter::Update(float dt)
{

	//Loop for the amount of particles made every second
	for (float i = 0; i < spawnRate; i++)
	{
		//create Particle then add it to the alive particles
		//Take data from patricle flyweight and send it to the particle
		Particle* tempParticle = new Particle;
		if (aliveParticles.size() < 500)
		{
			aliveParticles.push_back(deadParticles.front());
			deadParticles.erase(deadParticles.begin());
		}
	}
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		aliveParticles[i]->Update(dt);
	}
}
void Emitter::Render()
{
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		aliveParticles[i]->Render();
	}
}