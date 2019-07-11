#include "objects\Door.h"

#include "SpriteManager.h"

Door::Door(unsigned id, Map &map) :
	AdjacentTile(id, map,Object::Type::Gate, true)
{
	float u1, v1, u2, v2;
	m_animation.setXAnim(0, 1);
	m_animation.setYAnim(1, 2);

	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

void Door::setDead()
{
	Object::setDead();
	glm::vec2 pos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_id, pos);
	std::vector<std::vector<Map::Data*>> data = m_map.getSurroundingTiles(pos.x, pos.y);
	// Remove edges, we only need axises directions
	data[0][0] = nullptr;
	data[0][2] = nullptr;
	data[2][0] = nullptr;
	data[2][2] = nullptr;

	for (size_t y = 0; y < 3; y++)
		for (size_t x = 0; x < 3; x++)
		{
			Map::Data *elem = data[y][x];

			if (!elem)
				continue;

			static_cast<AdjacentObject*>(elem->tile)->setCorrectFrame();
			
			if (elem->object && (elem->object->m_type == Object::Gate || elem->object->m_type == Object::Wall))
			{
				if (elem->object->m_type == Object::Gate && !elem->object->getDead())
					elem->object->setDead();
				else
					static_cast<AdjacentObject*>(elem->object)->setCorrectFrame();
			}
		}
}
