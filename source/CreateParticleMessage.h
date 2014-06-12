#pragma once
#include <string>
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Message.h"
class Entity;
class CreateParticleMessage : public SGD::Message
{
public:
	CreateParticleMessage(std::string _EmitterID,int _X, int _Y);
	CreateParticleMessage(std::string _EmitterID, Entity* _Entity,int _XOffset,int _YOffset);
	CreateParticleMessage(std::string emitterID, Entity* entity, SGD::Vector vector, int XOffset, int YOffset);
	CreateParticleMessage(std::string emitterID, SGD::Point position, int XOffset, int YOffset);

	~CreateParticleMessage();

	//Accesors
	std::string GetEmitterID() const;
	int GetX() const;
	int GetY() const;
	int GetXOffset() const;
	int GetYOffset() const;
	SGD::Point GetPosition() const;
	SGD::Vector GetVector() const;
	Entity* GetParticleEntity() const;
	
	//Mutatos
	void SetX(int _X);
	void SetY(int _Y);
	void SetXOffset(int _XOffset);
	void SetYOffset(int _YOffset);
	void SetPosition(SGD::Point);
	void SetVector(SGD::Vector _Vector);
	void SetEmitterID(std::string _EmitterID);
	void SetParticleEntity(Entity* _Entity);

protected:
	int m_nX;
	int m_nY;
	int m_nXOffset;
	int m_nYOffset;
	SGD::Point m_pPosition;
	SGD::Vector m_vVector;
	Entity* m_pEntity;
	std::string m_sEmitterID;
};

