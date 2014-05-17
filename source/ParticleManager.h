#pragma once
#include <string>
#include <vector>
#include "ParticleFlyweight.h"
#include "Emitter.h"
#include <map>
class ParticleManager
{
	std::vector<ParticleFlyweight> particleFlyweights;
	std::vector<Emitter> activeEmitters;
	std::map<std::string, Emitter> loadedEmitters;
public:
	ParticleManager();
	~ParticleManager();
	void loadEmitters(std::string filName);
	Emitter* CreateEmitter(std::string emitterID);
};

