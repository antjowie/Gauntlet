#include "objects/Object.h"

#include "SpriteManager.h"

void Object::update(const float elapsedTime)
{
}

void Object::updateAfterMove(const float elapsedTime)
{
}

void Object::onCollide(Object *obj)
{
}

void Object::setDead()
{
	m_isDead = true;
}

bool Object::getDead() const
{
	return m_isDead;
}

Object::Object(const unsigned id, const Type type, const bool isSolid, const unsigned width, const unsigned height):
	m_id(id),
	m_type(type),
	m_isSolid(isSolid),
	m_isDead(false),
	m_shouldDraw(true),
	m_width(width),
	m_height(height)
{
	//NHTV::SpriteManager::GetInstance()->SetSpriteScale(m_id, glm::vec2(width, height));
}

Object::~Object()
{
	NHTV::SpriteManager::GetInstance()->RemoveSprite(m_id);
}
