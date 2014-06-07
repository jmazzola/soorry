#include "ParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include "Particle.h"
#include "ParticleFlyweight.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Entity.h"
#include <map>
//TODO: emitter position will be set by message, with an overload accepting an entity*
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
		if (activeEmitters[i]->Update(dt) == false)
		{
			delete activeEmitters[i];
			activeEmitters.erase(activeEmitters.begin() + i);
		}
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

}

bool ParticleManager::loadEmitter(std::string fileName)
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
		//get spread bool
		data->Attribute("spread", &tempInt);
		if (tempInt != 0) tempBool = true;
		else tempBool = false;
		tempFlyweight->isSpread = tempBool;
		//Read XML for flyweight rotation
		data = data->NextSiblingElement("rotate");
		data->Attribute("start", &tempDouble);
		tempFlyweight->startRotation = (float)tempDouble;
		data->Attribute("end", &tempDouble);
		tempFlyweight->endRotation = (float)tempDouble;
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
		tempStr = "resource/images/particles/" + tempStr;
		tempFlyweight->image = SGD::GraphicsManager::GetInstance()->LoadTexture(tempStr.c_str());
		//Adding flyweight to vector
		particleFlyweights.push_back(tempFlyweight);
		//creating an iterator to emplace an emitter into the map
		auto iter = loadedEmitters.end();
		tempEmitter->particleFlyweight = tempFlyweight;
		particleFlyweights.push_back(tempFlyweight);
		std::pair<std::string, Emitter*> emitterKeyPair;
		emitterKeyPair.first = tempFlyweight->particleID;
		emitterKeyPair.second = tempEmitter;
		loadedEmitters.insert(iter, emitterKeyPair);

		return true;
	}
	else
		return false;
}

void ParticleManager::unload()
{
	for (auto iter = loadedEmitters.begin(); iter != loadedEmitters.end(); ++iter)
	{
		delete loadedEmitters.at(iter->first);
	}
	activeEmitters.clear();
	loadedEmitters.clear();
}
void ParticleManager::activate(std::string _emitterID,int _x, int _y)
{
	//NOTE: should be creating new memory?
	Emitter* tempEmitter;
	tempEmitter = loadedEmitters[_emitterID];
	tempEmitter->position = SGD::Point( (float)_x, (float)_y );
	tempEmitter->load();
	tempEmitter->emitterID = activeEmitters.size();
	activeEmitters.push_back(tempEmitter);
}

void ParticleManager::activate(std::string _emitterID, Entity* _entity, int _x, int _y)
{
	Emitter* tempEmitter;
 	tempEmitter = loadedEmitters[_emitterID];
	tempEmitter->offset = SGD::Point((float)_x, (float)_y);
	tempEmitter->followEnitiy = _entity;
	tempEmitter->load();
	tempEmitter->emitterID = activeEmitters.size();
	activeEmitters.push_back(tempEmitter);
}

