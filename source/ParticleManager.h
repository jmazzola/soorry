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
	void unload();
	static ParticleManager* GetInstance();
	void activate(std::string,int,int);
	void activate ( std::string _emitterID , SGD::Point, int _x , int _y , bool vector = false , SGD::Vector direction = SGD::Vector { 0.0f , 0.0f } );
	ParticleManager();
	~ParticleManager();
	bool loadEmitter(std::string fileName);
};

