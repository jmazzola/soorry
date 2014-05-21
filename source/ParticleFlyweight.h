#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include <string>
class ParticleFlyweight
{
	
public:
	ParticleFlyweight();
	~ParticleFlyweight();
	//DataType         //Name
	SGD::HTexture	image;
	SGD::Vector		startVelocity;
	SGD::Vector		endVelocity;
	SGD::Color		startColor;
	SGD::Color		endColor;
	SGD::Size		startScale;
	SGD::Size		endScale;
	float			startRotation;
	float			endRotation;
	float			minLifeTime;
	float			maxLifeTime;
	std::string		particleID;
	SGD::Vector		direction;
	SGD::Size		imageSize;

};

