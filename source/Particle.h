#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "Emitter.h"
class Particle
{
	
public:
	Particle();
	~Particle();
	ParticleFlyweight particleFlyweight;
	SGD::Point position;
	SGD::Vector velocity;
	SGD::Color startColor;
	SGD::Color endColor;
	SGD::Size scale;
	SGD::Size size;
	float rotation;
	float currLifeTime;
	float maxLifeTime;
};

