#include "MachineGunBullet.h"

#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"


MachineGunBullet::MachineGunBullet()
{
	m_fLifetime = 1.5f;
}


MachineGunBullet::~MachineGunBullet()
{
}

/**********************************************************/
// Interface Methods

void MachineGunBullet::Update(float dt)
{
	m_fLifetime -= dt;
	if (m_fLifetime <= 0.0f)
	{
		DestroyEntityMessage* msg = new DestroyEntityMessage(this);
		msg->QueueMessage();
	}

	Entity::Update(dt);
}

void MachineGunBullet::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	pGraphics->DrawTexture(m_hImage, SGD::Point((float)(m_ptPosition.x - 3 - Camera::x), (float)(m_ptPosition.y - 3 - Camera::y)));

	// FOR DEBUG PURPOSES ONLY!
	//pGraphics->DrawRectangle(GetRect(), {});
}

int MachineGunBullet::GetType() const
{
	return ENT_MACHINE_GUN_BULLET;
}

SGD::Rectangle MachineGunBullet::GetRect() const
{
	SGD::Rectangle rect;

	rect.left = m_ptPosition.x - 3;
	rect.top = m_ptPosition.y - 3;
	rect.right = m_ptPosition.x + 3;
	rect.bottom = m_ptPosition.y + 3;

	return rect;
}

void MachineGunBullet::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_ZOMBIE_BEAVER || pOther->GetType() == ENT_ZOMBIE_FAST ||
		pOther->GetType() == ENT_ZOMBIE_SLOW)
	{
		// Destroy the proj
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

/**********************************************************/
// Accessors

int MachineGunBullet::GetDamage() const
{
	return m_nDamage;
}

/**********************************************************/
// Mutators

void MachineGunBullet::SetDamage(int _damage)
{
	m_nDamage = _damage;
}

void MachineGunBullet::SetImage(SGD::HTexture _image)
{
	m_hImage = _image;
}