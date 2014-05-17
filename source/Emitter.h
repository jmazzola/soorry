#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>
#include <list>
#include <string>

class Particle;

class Emitter
{

	
public:
	Emitter();
	~Emitter();
	void Update(float dt);
	void Render();

	ParticleFlyweight particleFlyweight;
	bool isLooping;
	SGD::Point position;
	SGD::Size size;
	int maxParticles;
	float spawnRate;
	int shape;
	//std::vector<Particle* > particles;
	//Particle[] particles;
	std::list<Particle* > aliveParticles;
	std::list<Particle* > deadParticles;
	std::string emitterID;
};

