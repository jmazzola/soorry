#pragma once
#include <string>
#include <vector>
#include "ParticleFlyweight.h"
#include "Emitter.h"
#include <map>
class ParticleManager
{
	std::vector<ParticleFlyweight*> particleFlyweights;
	std::vector<Emitter> activeEmitters;
	std::vector<std::string> fileNames;
	std::map<std::string, Emitter*> loadedEmitters;
	int numOfEmitters = 0;
public:
	ParticleManager();
	~ParticleManager();
	bool loadEmitters(std::string fileName);
	Emitter* createEmitter(std::string emitterID,std::string filename);
};

