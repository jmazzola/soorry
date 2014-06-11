#pragma once
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "Emitter.h"
#include "Player.h"
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
	SGD::Color			Color;
	SGD::Size			scale;
	SGD::Size			size;
	float				currLifeTime;
	float				maxLifeTime;
	float				rotation;
	float				rotationRate;
	bool				velOverride;
	SGD::Vector			velocityOS;
	SGD::Vector			velocityOE;
};

