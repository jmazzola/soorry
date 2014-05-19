#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>
#include <list>
#include <string>

class Particle;

class Emitter
{
	//A bool for when all particles are loaded
	bool allParticlesCreated = false;
public:
	Emitter();
	~Emitter();
	void Update(float dt);
	void Render();
	void load();
	ParticleFlyweight particleFlyweight;
	bool isLooping;
	SGD::Point position;
	SGD::Size size;
	int maxParticles;
	float spawnRate;
	int shape;
	std::vector<Particle* > aliveParticles;
	std::vector<Particle* > deadParticles;
	std::string emitterID;
};

