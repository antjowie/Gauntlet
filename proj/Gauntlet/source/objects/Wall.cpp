#include "objects\Wall.h"

#include "SpriteManager.h"

#include "objects/Explosion.h"

Wall::Wall(unsigned id, bool isBreakable, Map &map):
	AdjacentTile(id,map,Object::Wall,true),
	m_isBreakable(isBreakable)
{
	setBreakable(isBreakable);
}

void Wall::onCollide(Object * obj)
{
	if (!m_isBreakable || obj->m_type != Object::Type::Flyable)
		return;

	setDead();
	glm::vec2 pos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_id, pos);

	m_map.pushProjectile(new Explosion(pos, Explosion::Large));

	std::vector<std::vector<Map::Data*>> data = m_map.getSurroundingTiles(pos.x, pos.y);

	for(size_t y = 0; y < 3; y++)
		for (size_t x = 0; x < 3; x++)
		{
			Map::Data *elem = data[y][x];
			if (!elem)
				continue;
			
			static_cast<AdjacentObject*>(elem->tile)->setCorrectFrame();
			
			if (elem && elem->object && (elem->object->m_type == Object::Gate || elem->object->m_type == Object::Wall))
				static_cast<AdjacentObject*>(elem->object)->setCorrectFrame();
		}
}

void Wall::setBreakable(const bool isBreakable)
{
	m_isBreakable = isBreakable;
	if (isBreakable)
	{
		m_animation.setYAnim(3, 4);
		m_animation.setXAnim(1, 2);
	}
	else
	{
		m_animation.setYAnim(4, 5);
		m_animation.setXAnim(1, 2);
	}

	float u1, u2, v1, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));

}

bool Wall::getBreakable() const
{
	return m_isBreakable;
}
