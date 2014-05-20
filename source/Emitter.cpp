#include "Emitter.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "Particle.h"
#include <ctime>
Emitter::Emitter()
{
}

Emitter::~Emitter()
{
	if (aliveParticles.size() != 0)
		aliveParticles.clear();
	if (deadParticles.size() != 0)
		deadParticles.clear();
}

void Emitter::load()
{
	srand((unsigned int)time(nullptr));
	if (allParticlesCreated != true)
	{
		switch (shape)
		{
		case 0://square
		{
			SGD::Rectangle(position, size);
		}
			break;
		case 1://circle
		{
<<<<<<< HEAD

=======
		
>>>>>>> Matt'sBranch
		}
			break;
		case 2://line
		{

		}
			break;
		case 3://point
		{

		}
			break;
		}
		for (int i = 0; i < maxParticles; i++)
		{
			//Create a new particle
			Particle* tempParticle = new Particle;
			//set values
			float maxLife = particleFlyweight->maxLifeTime;
			//Take data from particle flyweight and send it to the particle
			tempParticle->Color = particleFlyweight->startColor;
			//Create rates to update particles
			tempParticle->colorRateA = (((float)particleFlyweight->startColor.alpha - (float)particleFlyweight->endColor.alpha) / maxLife);
			tempParticle->colorRateR = (((float)particleFlyweight->startColor.red - (float)particleFlyweight->endColor.red) / maxLife);
			tempParticle->colorRateG = (((float)particleFlyweight->startColor.green - (float)particleFlyweight->endColor.green) / maxLife);
			tempParticle->colorRateB = (((float)particleFlyweight->startColor.blue - (float)particleFlyweight->endColor.blue) / maxLife);
			tempParticle->maxLifeTime = maxLife;
			tempParticle->currLifeTime = 0;
			tempParticle->scale = particleFlyweight->startScale;
			tempParticle->scaleRateX = ((particleFlyweight->startScale.x - particleFlyweight->endScale.x) / maxLife);
			tempParticle->scaleRateY = ((particleFlyweight->startScale.y - particleFlyweight->endScale.y) / maxLife);
			tempParticle->velocity = particleFlyweight->startVelocity;
			tempParticle->velocityRateX = ((particleFlyweight->startVelocity.x + particleFlyweight->endVelocity.x) / maxLife);
			tempParticle->velocityRateY = ((particleFlyweight->startVelocity.y + particleFlyweight->endVelocity.y) / maxLife);
			//Randomize the position within the emitter NOTE: maybe need to add world offset
			tempParticle->position.x = (float)(rand() % (int)size.width)+position.x;
			tempParticle->position.y = (float)(rand() % (int)size.height)+position.y;
			tempParticle->particleFlyweight = *particleFlyweight;
			//add it to dead particles
			deadParticles.push_back(tempParticle);
		}
		allParticlesCreated = true;
	}
}

void Emitter::Update(float dt)
{
	if (aliveParticles.size() == 0 && deadParticles.size() == 0)
		return;
	//Loop for the amount of particles made every second
	if (spawnTimer < 0)
	{
		for (float i = 0; i < spawnRate; i++)
		{
			//create Particle then add it to the alive particles
			//Take data from patricle flyweight and send it to the particle
			if (aliveParticles.size() < maxParticles)
			{
				aliveParticles.push_back(deadParticles.front());
				deadParticles.erase(deadParticles.begin());
			}
		}
		spawnTimer = spawnRate;
	}
	spawnTimer -= dt;
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		//check if the particle is dead
		if (!aliveParticles[i]->Update(dt))
		{
			//move the particle into the dead list
			deadParticles.push_back(aliveParticles[i]);
			aliveParticles.erase(aliveParticles.begin() + i);
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

