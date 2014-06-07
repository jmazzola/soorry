#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>
#include <list>
#include <string>

class Particle;
class Entity;

class Emitter
{
	//A bool for when all particles are loaded
	bool allParticlesCreated = false;
	//timer for controling the spwn to run on a per second basis
	float spawnTimer;
public:
	Emitter();
	//The trilogy... OF EVIL!
	virtual ~Emitter();
	Emitter(const Emitter& _cpy);
	Emitter& operator=(const Emitter& _assign);
	//End of trilogy
	bool Update(float dt);
	void Render();
	void load();
	ParticleFlyweight* particleFlyweight;
	bool isLooping;
	SGD::Point position;
	SGD::Point offset = { 0, 0};
	SGD::Size size;
	unsigned int maxParticles;
	float spawnRate;
	int shape;
	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;
	int emitterID;
	SGD::Rectangle square;
	Entity* followEnitiy = nullptr;
};

