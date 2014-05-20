#pragma once
#include <string>
#include <vector>
#include "ParticleFlyweight.h"
#include "Emitter.h"
#include <map>
class ParticleManager
{
	std::vector<ParticleFlyweight*> particleFlyweights;
	std::vector<Emitter*> activeEmitters;
	std::map<std::string, Emitter*> loadedEmitters;
	int numOfEmitters = 0;
public:
	void Update(float dt);
	void Render();
	void load();
	static ParticleManager* GetInstance();
	ParticleManager();
	~ParticleManager();
	bool loadEmitters(std::string fileName,std::string);
	Emitter* createEmitter(std::string emitterID,std::string filename);
};

