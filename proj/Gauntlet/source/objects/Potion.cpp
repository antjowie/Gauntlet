#include "objects\Potion.h"

#include "SpriteManager.h"

Potion::Potion(const unsigned id, const PotionType PotionType) :
	Object(id, Object::Type::Potion, true),
	m_animation(PotionType, PotionType + 1, 5, 6, 0.f)
{
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}
