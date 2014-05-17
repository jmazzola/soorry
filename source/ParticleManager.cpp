#include "ParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
}

void ParticleManager::loadEmitters(std::string fileName)
{
	TiXmlDocument doc(fileName.c_str());
	if (doc.LoadFile())
	{

	}
}
Emitter* createEmitter(std::string emitterID)
{
	return nullptr;
}