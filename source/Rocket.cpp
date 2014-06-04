#include "Rocket.h"
#include "AnimationManager.h"

Rocket::Rocket()
{
	SetSprite(AnimationManager::GetInstance()->GetSprite("rocket"));
	m_antsAnimation.m_fTimeOnFrame = 0;
	m_antsAnimation.m_nCurrFrame = 0;
	m_antsAnimation.m_nCurrAnimation = "rocket";
}


Rocket::~Rocket()
{
	/*delete m_pSprite;*/
}

 int Rocket::GetType() const 
{
	return ENT_BULLET_ROCKET;
}
