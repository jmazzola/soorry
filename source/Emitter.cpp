#include "Emitter.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Particle.h"
#include <ctime>
#include <math.h>
#include "Entity.h"
#include "Camera.h"

Emitter::Emitter()
{
}

Emitter::~Emitter()
{
	for (unsigned int i = aliveParticles.size(); i > 0; i--)
	{
		delete aliveParticles[i - 1];
	}
	if (aliveParticles.size() != 0)
		aliveParticles.clear();
	for (unsigned int i = deadParticles.size(); i > 0; i--)
	{
		delete deadParticles[i - 1];
	}
	if (deadParticles.size() != 0)
		deadParticles.clear();
	if (particleFlyweight != nullptr)
	{
		delete particleFlyweight;
		particleFlyweight = nullptr;
	}
}

Emitter::Emitter(const Emitter& _cpy)
{
	for (unsigned int i = aliveParticles.size(); i > 0; i--)
	{
		delete aliveParticles[i - 1];
	}
	if (aliveParticles.size() != 0)
		aliveParticles.clear();
	for (unsigned int i = 0; i < _cpy.aliveParticles.size(); i++)
	{
		aliveParticles.push_back(new Particle);
	}
	for (unsigned int i = deadParticles.size(); i > 0; i--)
	{
		delete deadParticles[i - 1];
	}
	if (deadParticles.size() != 0)
		deadParticles.clear();
	for (unsigned int i = 0; i < _cpy.deadParticles.size(); i++)
	{
		deadParticles.push_back(new Particle);
	}
	if (particleFlyweight != nullptr)
	{
		delete particleFlyweight;
		particleFlyweight = nullptr;
	}
	particleFlyweight = new ParticleFlyweight;
	followEnitiy = _cpy.followEnitiy;
	particleFlyweight = _cpy.particleFlyweight;
	isLooping = _cpy.isLooping;
	position = _cpy.position;
	offset = _cpy.offset;
	size = _cpy.size;
	maxParticles = _cpy.maxParticles;
	spawnRate = _cpy.spawnRate;
	shape = _cpy.shape;
	emitterID = _cpy.emitterID;
	square = _cpy.square;
}

Emitter& Emitter::operator=(const Emitter& _assign)
{
	//check for self assignment
	if (this != &_assign)
	{
		for (unsigned int i = aliveParticles.size(); i > 0; i--)
		{
			delete aliveParticles[i - 1];
		}
		if (aliveParticles.size() != 0)
			aliveParticles.clear();
		for (unsigned int i = 0; i < _assign.aliveParticles.size(); i++)
		{
			aliveParticles.push_back(new Particle);
		}
		for (unsigned int i = deadParticles.size(); i > 0; i--)
		{
			delete deadParticles[i - 1];
		}
		if (deadParticles.size() != 0)
			deadParticles.clear();
		for (unsigned int i = 0; i < _assign.deadParticles.size(); i++)
		{
			deadParticles.push_back(new Particle);
		}
		if (particleFlyweight != nullptr)
		{
			delete particleFlyweight;
			particleFlyweight = nullptr;
		}
		particleFlyweight = new ParticleFlyweight;
		followEnitiy = _assign.followEnitiy;
		particleFlyweight = _assign.particleFlyweight;
		isLooping = _assign.isLooping;
		position = _assign.position;
		offset = _assign.offset;
		size = _assign.size;
		maxParticles = _assign.maxParticles;
		spawnRate = _assign.spawnRate;
		shape = _assign.shape;
		emitterID = _assign.emitterID;
		square = _assign.square;
	}
	return *this;
}

void Emitter::load()
{
	
	srand((unsigned int)time(nullptr));
	if (allParticlesCreated != true)
	{
		if (followEnitiy != nullptr)
		{
			position = followEnitiy->GetPosition();
			position.x -= Camera::x;
			position.y -= Camera::y;
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
			tempParticle->maxLifeTime = rand() % (int)particleFlyweight->maxLifeTime + particleFlyweight->minLifeTime;
			tempParticle->currLifeTime = tempParticle->maxLifeTime;
			tempParticle->scale = particleFlyweight->startScale;
			if (!particleFlyweight->isSpread)
			{
				tempParticle->velocity = particleFlyweight->startVelocity;
			}
			else
			{
				tempParticle->velocity.x = rand() % (int)(particleFlyweight->startVelocity.x * 2) + particleFlyweight->endVelocity.x;
				tempParticle->velocity.y = rand() % (int)(particleFlyweight->startVelocity.y * 2) + particleFlyweight->endVelocity.y;
			}
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
				//MAFF very powerful
				//NOTE:width == radius height is not used
				float radius = (float)(rand() % (int)size.width / 2);
				float x = size.width / 2;
				float y = size.width / 2;
				//Point for the center of the emitter
				SGD::Point center = SGD::Point(x, y);
				//randomize the angle for the circle
				float randAngle = (float)(rand() % 360);
				randAngle = (randAngle / 180)*SGD::PI;
				//put in the angle
				tempParticle->position.x = center.x + (radius*cosf(randAngle) + position.x);
				tempParticle->position.y = center.y + (radius*sinf(randAngle) + position.y);
			}
				break;
			case 2://line
			{
				//draw line from position to width and height
				SGD::Point start = position;
				SGD::Point end = { size.width, size.height };
				float slope = (start.y - end.y) / (start.x - end.x);
				float x, y;
				if (start.x != 0)
					x = rand() % (int)start.x + end.x;
				if (end.x != 0)
					x = rand() % (int)end.x + start.x;
				y = end.y + (slope*(x - end.x));
				tempParticle->position = { x, y };
			}
				break;
			case 3://point
			{
				tempParticle->position = position;
			}
				break;
			}

			tempParticle->particleFlyweight = particleFlyweight;
			tempParticle->currLifeTime = tempParticle->maxLifeTime;
			//add it to dead particles
			deadParticles.push_back(tempParticle);
			spawnRate = deadParticles.size() / ((particleFlyweight->maxLifeTime + particleFlyweight->minLifeTime) / 2);
		}
 		allParticlesCreated = true;
	}
}

bool Emitter::Update(float dt)
{
	if (followEnitiy != nullptr)
	{
		position = followEnitiy->GetPosition();
		position.x -= Camera::x;
		position.y -= Camera::y;
	}
	
	if (isLooping)
	{
		//NOTE: may cause bugs not sure
		if (aliveParticles.size() == 0 && deadParticles.size() == 0)
			return false;
		for (float i = 0; i < spawnRate*dt; i++)
		{
			//create Particle then add it to the alive particles
			//Take data from patricle flyweight and send it to the particle
			if (aliveParticles.size() < (unsigned int)maxParticles)
			{
				Particle* tempParticle = deadParticles.front();
				tempParticle->currLifeTime = tempParticle->maxLifeTime;
				if (!particleFlyweight->isSpread)
				{
					tempParticle->velocity = particleFlyweight->startVelocity;
				}
				else
				{
					tempParticle->velocity.x = rand() % (int)(particleFlyweight->startVelocity.x * 2) + particleFlyweight->endVelocity.x;
					tempParticle->velocity.y = rand() % (int)(particleFlyweight->startVelocity.y * 2) + particleFlyweight->endVelocity.y;
				}
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
					//NOTE:width == radius height is not used
					float radius = (float)(rand() % (int)size.width / 2);
					//Point for the center of the emitter
					SGD::Point center = SGD::Point(position.x + size.width / 2, position.y + size.width / 2);
					//randomize the angle for the circle
					float randAngle = (float)(rand() % 360);
					//convert to radians
					randAngle = (randAngle / 180)*SGD::PI;
					//put in the angle
					tempParticle->position.x = center.x + (radius*cosf(randAngle));
					tempParticle->position.y = center.y + (radius*sinf(randAngle));
				}
					break;
				case 2://line
				{
					//draw line from position to width and height
					SGD::Point start = position;
					SGD::Point end = { size.width, size.height };
					float slope = (start.y - end.y) / (start.x - end.x);
					float x, y;
					if (start.x != 0)
						x = rand() % (int)start.x + end.x;
					if (end.x != 0)
						x = rand() % (int)end.x + start.x;
					y = end.y + (slope*(x - end.x));
					tempParticle->position = { x, y };
				}
					break;
				case 3://point
				{
					tempParticle->position = position;
				}
					break;
				}

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
			{
				break;
			}
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
			deadParticles.push_back(aliveParticles[i]);
			aliveParticles.erase(aliveParticles.begin() + i);
			i--;
		}
	}
	if (deadParticles.size() == maxParticles)
	{
		allParticlesCreated = false;
		return false;
	}
	return true;
}

void Emitter::Render()
{
	for (unsigned int i = 0; i < aliveParticles.size(); i++)
	{
		aliveParticles[i]->Render();
	}
}

