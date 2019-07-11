#include "objects/Treasure.h"

#include "SpriteManager.h"

Treasure::Treasure(const unsigned id) :
	Object(id, Object::Type::Treasure, true),
	m_animation(7,8,6,7,0.f),
	m_animIndex(-1),

	m_elapsed(0)
{
	switchFrame();
}

void Treasure::update(const float elapsedTime)
{
	m_elapsed += elapsedTime;
	if (m_elapsed > 1.f)
	{
		m_elapsed = 0;
		switchFrame();
	}
}

void Treasure::switchFrame()
{

	int oldIndex = m_animIndex;
	m_animIndex = std::rand() % 3;

	if (m_animIndex == oldIndex)
	{
		m_animIndex += 1;
		m_animIndex %= 3;
	}

	m_animation.setXAnim(7 + m_animIndex, 8 + m_animIndex);
	
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}
