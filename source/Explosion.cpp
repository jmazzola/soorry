#include "Explosion.h"

#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "Camera.h"


Explosion::Explosion()
{
	_m_nImageIndex = 0;
	_m_fImageSpeed = 0.05f;
	_m_fNextImage = _m_fImageSpeed;
	m_bExploded = false;
}


Explosion::~Explosion()
{
}


/**********************************************************/
// Interface Methods

void Explosion::Update(float dt)
{
	if (!m_bExploded)
	{
		SGD::Event e("EXPLOSION", nullptr, this);
		e.SendEventNow();

		SGD::AudioManager::GetInstance()->PlayAudio(m_hExplosion);

		m_bExploded = true;
	}

	_m_fNextImage -= dt;

	if (_m_fNextImage <= 0.0f)
	{
		_m_fNextImage = _m_fImageSpeed;
		_m_nImageIndex++;

		if (_m_nImageIndex > 15)
		{
			DestroyEntityMessage* msg = new DestroyEntityMessage(this);
			msg->QueueMessage();
		}
	}
}

void Explosion::Render()
{
	SGD::Rectangle rect;
	rect.left = (float)((_m_nImageIndex % 4) * 64);
	rect.top = (float)((_m_nImageIndex / 4) * 64);
	rect.right = rect.left + 64;
	rect.bottom = rect.top + 64;

	SGD::GraphicsManager::GetInstance()->DrawTextureSectionSimple(m_hImage, m_ptPosition - SGD::Vector((float)Camera::x + 32, (float)Camera::y + 32), rect);
}

SGD::Rectangle Explosion::GetRect() const
{
	return SGD::Rectangle();
}

/**********************************************************/
// Accessors

float Explosion::GetDamage() const
{
	return m_fDamage;
}

float Explosion::GetRadius() const
{
	return m_fRadius;
}

/**********************************************************/
// Mutators

void Explosion::SetDamage(float _damage)
{
	m_fDamage = _damage;
}

void Explosion::SetRadius(float _radius)
{
	m_fRadius = _radius;
}

void Explosion::SetImage(SGD::HTexture _image)
{
	m_hImage = _image;
}

void Explosion::SetExplosionSound(SGD::HAudio _explosionSound)
{
	m_hExplosion = _explosionSound;
}
