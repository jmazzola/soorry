#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "Emitter.h"
class Particle
{
	
public:
	Particle();
	~Particle();
	SGD::HTexture		image;
	bool				Update(float dt);
	void				Render();
	ParticleFlyweight*	particleFlyweight;
	SGD::Point			position;
	SGD::Vector			velocity;
	float				velocityRateX;
	float				velocityRateY;
	SGD::Color			Color;
	float				colorRateA;
	float				colorRateR;
	float				colorRateG;
	float				colorRateB;
	SGD::Vector			scale;
	float				scaleRateX;
	float				scaleRateY;
	SGD::Size			size;
	float				rotation;
	float				currLifeTime;
	float				maxLifeTime;
};

