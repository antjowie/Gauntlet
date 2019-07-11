#include "objects\Floor.h"

#include "SpriteManager.h"

Floor::Floor(const unsigned id, const bool isTrap, Map &map) :
	AdjacentFloor(id, map,Object::Floor, isTrap),
	m_isTrap(isTrap)
{
	setTrap(isTrap);
}

void Floor::setTrap(bool isTrap)
{
	m_isTrap = isTrap;
	if (isTrap)
	{
		m_animation.setYAnim(2, 3);
		m_animation.setXAnim(0, 1);
	}
	else
	{
		m_animation.setYAnim(2, 3);
		m_animation.setXAnim(0, 1);
	}

	float u1, u2, v1, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

bool Floor::getTrap() const
{
	return m_isTrap;
}
