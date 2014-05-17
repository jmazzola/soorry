#include "ParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include "Particle.h"
#include "ParticleFlyweight.h"
ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
}

bool ParticleManager::loadEmitters(std::string fileName)
{
	TiXmlDocument doc(fileName.c_str());
	if (doc.LoadFile())
	{
		//Temporary variables for transfering data
		double width, height, tempDouble, x, y, a, r, g, b;
		int tempInt;
		std::string tempStr; 
		Particle tempParticle;
		Emitter tempEmitter;
		ParticleFlyweight tempFlyweight;
		TiXmlElement* root, *emitter, *flyweight, *data;
		root = doc.FirstChildElement("ParticleManager");
		if (!root)
			return false;
		//Read XML for particle Emitter
		emitter = root->FirstChildElement("emitter");
		data = emitter->FirstChildElement("isLooping");
		data->Attribute("bool", &tempInt);
		tempEmitter.isLooping = (bool)tempInt;
		//Read XML for Emitter Position
		data = data->NextSiblingElement("position");
		data->Attribute("x", &x);
		data->Attribute("y", &y);
		tempEmitter.position = SGD::Point((float)x, (float)y);
		//Read XML for Emitter size
		data = data->NextSiblingElement("size");
		data->Attribute("width", &width);
		data->Attribute("height", &height);
		tempEmitter.size = SGD::Size((float)width, (float)height);
		//Read XML for max number of particles
		data = data->NextSiblingElement("particles");
		data->Attribute("max", &tempInt);
		tempEmitter.maxParticles = tempInt;
		//Read XML for Particle Spawn Rate
		data = data->NextSiblingElement("spawn");
		data->Attribute("rate", &tempDouble);
		tempEmitter.spawnRate = (float)tempDouble;
		//Read XML for Emitter Shape
		data = data->NextSiblingElement("shape");
		data->Attribute("number", &tempInt);
		//Start reading in flyweight data
		flyweight = root->FirstChildElement("flyweight");
		//Read XML for Flyweight velocity
		//get start velocity
		data = flyweight->FirstChildElement("velocity");
		data->Attribute("startx", &x);
		data->Attribute("starty", &y);
		tempFlyweight.startVelocity = SGD::Vector((float)x, (float)y);
		//get end velocity
		data->Attribute("endx", &x);
		data->Attribute("endy", &y);
		tempFlyweight.endVelocity = SGD::Vector((float)x, (float)y);
		//Read XML for Flyweight direction
		data = data->NextSiblingElement("direction");
		data->Attribute("x", &x);
		data->Attribute("y", &y);
		tempFlyweight.direction = SGD::Vector((float)x, (float)y);
		//Read XML for Flyweight color
		data = data->NextSiblingElement("color");
		data->Attribute("startA",&a);
		data->Attribute("startR",&r);
		data->Attribute("startG",&g);
		data->Attribute("startB",&b);
		tempFlyweight.startColor = SGD::Color((unsigned char)a, (unsigned char)r, (unsigned char)g, (unsigned char)b);
		data->Attribute("endA", &a);
		data->Attribute("endR", &r);
		data->Attribute("endG", &g);
		data->Attribute("endB", &b);
		tempFlyweight.endColor = SGD::Color((unsigned char)a, (unsigned char)r, (unsigned char)g, (unsigned char)b);
		//Read XML Flyweight Scale
		data = data->NextSiblingElement("scale");
		data->Attribute("startx", &x);
		data->Attribute("starty", &y);
		tempFlyweight.startScale = SGD::Vector((float)x, (float)y);
		data->Attribute("endx", &x);
		data->Attribute("endy", &y);
		tempFlyweight.endScale = SGD::Vector((float)x, (float)y);
		//Read XML Flyweight lifetiem
		data = data->NextSiblingElement("lifetime");
		data->Attribute("max", &tempDouble);
		tempFlyweight.maxLifeTime = (float)tempDouble;
		data->Attribute("min", &tempDouble);
		tempFlyweight.minLifeTime = (float)tempDouble;
		//Read XML for the Flyweight Particle ID
		data = data->NextSiblingElement("particleID");
		tempStr = data->GetText();
		tempFlyweight.particleID = tempStr;
		//Read XML for the Flyweight Image filename
		data = data->NextSiblingElement("image");
		tempStr = data->Attribute("name");
		tempFlyweight.fileName = tempStr;
		return true;
	}
	else
		return false;
}
Emitter* createEmitter(std::string emitterID)
{
	return nullptr;
}