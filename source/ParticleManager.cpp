#include "ParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include "Particle.h"
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{

}


ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager s_Instance;
	return &s_Instance;
}

void ParticleManager::Update(float dt)
{
	for (unsigned int i = 0; i < activeEmitters.size(); i++)
	{
		activeEmitters[i]->Update(dt);
	}
}

void ParticleManager::Render()
{
	for (unsigned int i = 0; i < activeEmitters.size(); i++)
	{
		activeEmitters[i]->Render();
	}
}
void ParticleManager::load()
{
	for (unsigned int i = 0; i < activeEmitters.size(); i++)
	{
		//NOTE for later: should switch to map so its not just the active emitters
		activeEmitters[i]->load();
	}
}

bool ParticleManager::loadEmitters(std::string fileName, std::string EmitterID)
{
	TiXmlDocument doc(fileName.c_str());
	if (doc.LoadFile())
	{
		//Temporary variables for transfering data
		double width, height, tempDouble, x, y, a, r, g, b;
		int tempInt;
		bool tempBool;
		std::string tempStr; 
		Emitter* tempEmitter = new Emitter;
		ParticleFlyweight* tempFlyweight = new ParticleFlyweight;
		TiXmlElement* root, *emitter, *flyweight, *data;
		root = doc.FirstChildElement("ParticleManager");
		if (!root)
			return false;
		//Read XML for particle Emitter
		emitter = root->FirstChildElement("emitter");
		data = emitter->FirstChildElement("isLooping");
		data->Attribute("bool", &tempInt);
		if (tempInt != 0) tempBool = true;
		else tempBool = false;
		tempEmitter->isLooping = tempBool;
		//Read XML for Emitter Position
		data = data->NextSiblingElement("position");
		data->Attribute("x", &x);
		data->Attribute("y", &y);
		tempEmitter->position = SGD::Point((float)x, (float)y);
		//Read XML for Emitter size
		data = data->NextSiblingElement("size");
		data->Attribute("width", &width);
		data->Attribute("height", &height);
		tempEmitter->size = SGD::Size((float)width, (float)height);
		//Read XML for max number of particles
		data = data->NextSiblingElement("particles");
		data->Attribute("max", &tempInt);
		tempEmitter->maxParticles = tempInt;
		//Read XML for Emitter Shape
		data = data->NextSiblingElement("shape");
		data->Attribute("number", &tempInt);
		tempEmitter->shape = tempInt;
		//Start reading in flyweight data
		flyweight = root->FirstChildElement("flyweight");
		//Read XML for Flyweight velocity
		//get start velocity
		data = flyweight->FirstChildElement("velocity");
		data->Attribute("startx", &x);
		data->Attribute("starty", &y);
		tempFlyweight->startVelocity = SGD::Vector((float)x, (float)y);
		//get end velocity
		data->Attribute("endx", &x);
		data->Attribute("endy", &y);
		tempFlyweight->endVelocity = SGD::Vector((float)x, (float)y);
		//Read XML for flyweight rotation
		data = data->NextSiblingElement("rotate");
		data->Attribute("start", &tempDouble);
		tempFlyweight->startRotation = (float)tempDouble;
		data->Attribute("end", &tempDouble);
		tempFlyweight->endRotation = (float)tempDouble;
		//Read XML for Flyweight direction
		data = data->NextSiblingElement("direction");
		data->Attribute("x", &x);
		data->Attribute("y", &y);
		tempFlyweight->direction = SGD::Vector((float)x, (float)y);
		//Read XML for Flyweight color
		data = data->NextSiblingElement("color");
		data->Attribute("startA",&a);
		data->Attribute("startR",&r);
		data->Attribute("startG",&g);
		data->Attribute("startB",&b);
		tempFlyweight->startColor = SGD::Color((unsigned char)a, (unsigned char)r, (unsigned char)g, (unsigned char)b);
		data->Attribute("endA", &a);
		data->Attribute("endR", &r);
		data->Attribute("endG", &g);
		data->Attribute("endB", &b);
		tempFlyweight->endColor = SGD::Color((unsigned char)a, (unsigned char)r, (unsigned char)g, (unsigned char)b);
		//Read XML Flyweight Scale
		data = data->NextSiblingElement("scale");
		data->Attribute("startx", &x);
		data->Attribute("starty", &y);
		tempFlyweight->startScale = SGD::Size((float)x, (float)y);
		data->Attribute("endx", &x);
		data->Attribute("endy", &y);
		tempFlyweight->endScale = SGD::Size((float)x, (float)y);
		//Read XML Flyweight image size
		data = data->NextSiblingElement("size");
		data->Attribute("width", &tempDouble);
		tempFlyweight->imageSize.width = (float)tempDouble;
		data->Attribute("height", &tempDouble);
		tempFlyweight->imageSize.height = (float)tempDouble;
		//Read XML Flyweight lifetiem
		data = data->NextSiblingElement("lifetime");
		data->Attribute("max", &tempDouble);
		tempFlyweight->maxLifeTime = (float)tempDouble;
		data->Attribute("min", &tempDouble);
		tempFlyweight->minLifeTime = (float)tempDouble;
		//Read XML for the Flyweight Particle ID
		data = data->NextSiblingElement("particleID");
		tempStr = data->GetText();
		tempFlyweight->particleID = tempStr;
		//Read XML for the Flyweight Image filename
		data = data->NextSiblingElement("image");
		tempStr = data->Attribute("name");
		tempFlyweight->image = SGD::GraphicsManager::GetInstance()->LoadTexture(tempStr.c_str());
		//Adding flyweight to vector
		particleFlyweights.push_back(tempFlyweight);
		//creating an iterator to emplace an emitter into the map
		auto iter = loadedEmitters.end();
		tempEmitter->particleFlyweight = tempFlyweight;
		particleFlyweights.push_back(tempFlyweight);
		std::pair<std::string, Emitter*> emitterKeyPair;
		emitterKeyPair.first = EmitterID;
		emitterKeyPair.second = tempEmitter;
		loadedEmitters.insert(iter, emitterKeyPair);
		//NOTE: pushing to active emitters for testing
		activeEmitters.push_back(tempEmitter);
		return true;
	}
	else
		return false;
}

Emitter* ParticleManager::createEmitter(std::string emitterID, std::string filename)
{
	if (loadEmitters(filename,emitterID))
	{
		return loadedEmitters[emitterID];
	}
	return nullptr;
}

void ParticleManager::unload()
{
	for (unsigned int i = activeEmitters.size(); i > 0; i--)
	{
		delete activeEmitters[i-1];
	}
	activeEmitters.clear();
	loadedEmitters.clear();
}
