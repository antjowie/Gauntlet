#include "objects\Key.h"

#include "SpriteManager.h"

Key::Key(const unsigned id) :
	Object(id, Object::Type::Key, true),
	m_animation(5, 6, 7, 8, 0.f)
{
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

void Key::onCollide(Object * obj)
{
	if (obj->m_type == Object::Hero)
		setDead();
}
