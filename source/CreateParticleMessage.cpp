#include "CreateParticleMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreateParticleMessage::CreateParticleMessage(std::string emitterID, int x, int y) : Message(MessageID::MSG_CREATE_STATIC_PARTICLE)
{
	m_sEmitterID = emitterID;
	m_nX = x;
	m_nY = y;
}

CreateParticleMessage::CreateParticleMessage(std::string emitterID, Entity* entity, int XOffset, int YOffset) : Message(MessageID::MSG_CREATE_DYNAMIC_PARTICLE)
{
	m_sEmitterID = emitterID;
	m_pEntity = entity;
	m_nXOffset = XOffset;
	m_nYOffset = YOffset;
}

CreateParticleMessage::CreateParticleMessage(std::string emitterID, Entity* entity,SGD::Vector vector, int XOffset, int YOffset) : Message(MessageID::MSG_CREATE_VECTOR_PARTICLE)
{
	//Here you go Ryan
	m_vVector = vector;
	m_sEmitterID = emitterID;
	m_pEntity = entity;
	m_nXOffset = XOffset;
	m_nYOffset = YOffset;
}


CreateParticleMessage::~CreateParticleMessage()
{

}


std::string CreateParticleMessage::GetEmitterID()const
{
	return m_sEmitterID;
}
int CreateParticleMessage::GetX()const
{
	return m_nX;
}
int CreateParticleMessage::GetY()const
{
	return m_nY;
}
int CreateParticleMessage::GetXOffset()const
{
	return m_nXOffset;
}
int CreateParticleMessage::GetYOffset()const
{
	return m_nYOffset;
}
SGD::Vector CreateParticleMessage::GetVector() const
{
	return m_vVector;
}
Entity* CreateParticleMessage::GetParticleEntity()const
{
	return m_pEntity;
}

void CreateParticleMessage::SetX(int _X)
{
	m_nX = _X;
}
void CreateParticleMessage::SetY(int _Y)
{
	m_nY = _Y;
}
void CreateParticleMessage::SetXOffset(int _XOffset)
{
	m_nXOffset = _XOffset;
}
void CreateParticleMessage::SetYOffset(int _YOffset)
{
	m_nXOffset = _YOffset;
}
void CreateParticleMessage::SetVector(SGD::Vector _Vector)
{
	m_vVector = _Vector;
}
void CreateParticleMessage::SetEmitterID(std::string _EmitterID)
{
	m_sEmitterID = _EmitterID;
}
void CreateParticleMessage::SetParticleEntity(Entity* _Entity)
{
	m_pEntity = _Entity;
}