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
	int					colorRateA;
	int					colorRateR;
	int					colorRateG;
	int					colorRateB;
	SGD::Size			scale;
	float				scaleRateX;
	float				scaleRateY;
	SGD::Size			size;
	float				currLifeTime;
	float				maxLifeTime;
	float				rotation;
	float				rotationRate;
};

