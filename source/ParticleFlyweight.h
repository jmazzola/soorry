#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include <string>
class ParticleFlyweight
{
	
public:
	ParticleFlyweight();
	~ParticleFlyweight();
	//DataType         //Name
	SGD::Vector		startVelocity;
	SGD::Vector		endVelocity;
	SGD::Color		startColor;
	SGD::Color		endColor;
	SGD::Vector		startScale;
	SGD::Vector		endScale;
	float			startRotation;
	float			endRotation;
	float			minLifeTime;
	float			maxLifeTime;
	std::string		particleID;
	std::string		fileName;
	SGD::Vector		direction;


};

