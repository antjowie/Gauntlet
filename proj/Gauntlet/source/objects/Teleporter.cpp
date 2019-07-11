#include "objects/Teleporter.h"
#include "SpriteManager.h"

Teleporter::Teleporter(const unsigned id) :
	Object(id, Object::Type::Teleporter, true),
	m_animation(0,4,6,7,0.25f)
{
}

void Teleporter::update(const float elapsedTime)
{
	m_animation.update(elapsedTime);

	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}