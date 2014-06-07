#include "MapleSyrupBullet.h"

#include "Camera.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"


MapleSyrupBullet::MapleSyrupBullet()
{
	m_fLifetime = 1.5f;
}


MapleSyrupBullet::~MapleSyrupBullet()
{
}

/**********************************************************/
// Interface Methods

void MapleSyrupBullet::Update(float dt)
{
	m_fLifetime -= dt;
	if (m_fLifetime <= 0.0f)
	{
		DestroyEntityMessage* msg = new DestroyEntityMessage(this);
		msg->QueueMessage();
	}

	Entity::Update(dt);
}

void MapleSyrupBullet::Render()
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	pGraphics->DrawTexture(m_hImage, SGD::Point((float)(m_ptPosition.x - 3 - Camera::x), (float)(m_ptPosition.y - 3 - Camera::y)));

	// FOR DEBUG PURPOSES ONLY!
	//pGraphics->DrawRectangle(GetRect(), {});
}

int MapleSyrupBullet::GetType() const
{
	return ENT_MAPLE_SYRUP_BULLET;
}

SGD::Rectangle MapleSyrupBullet::GetRect() const
{
	SGD::Rectangle rect;

	rect.left = m_ptPosition.x - 7;
	rect.top = m_ptPosition.y - 7;
	rect.right = m_ptPosition.x + 7;
	rect.bottom = m_ptPosition.y + 7;

	return rect;
}

void MapleSyrupBullet::HandleCollision(const IEntity* pOther)
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

float MapleSyrupBullet::GetSlowTime() const
{
	return m_fSlowTime;
}

/**********************************************************/
// Mutators

void MapleSyrupBullet::SetSlowTime(float _slowTime)
{
	m_fSlowTime = _slowTime;
}

void MapleSyrupBullet::SetImage(SGD::HTexture _image)
{
	m_hImage = _image;
}