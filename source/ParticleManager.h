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
	std::vector<std::string> IDs;
	int numOfEmitters = 0;
public:
	void Update(float dt);
	void Render();
	void load();
	void unload();
	static ParticleManager* GetInstance();
	void activate(std::string,int,int);
	void activate(std::string _emitterID, Entity* _entity, int _x, int _y);
	ParticleManager();
	~ParticleManager();
	bool loadEmitters(std::string fileName);
	Emitter* createEmitter(std::string emitterID,std::string filename);
};

