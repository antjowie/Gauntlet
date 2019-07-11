#include "objects\Exit.h"

#include "Map.h"
#include "TiledConverter.h"

#include "SpriteManager.h"

#include <cassert>

Exit::Exit(unsigned id, unsigned level, Map &map):
	Object(id,Object::Type::Exit,true),
	m_animation(3,6,6,7,0.f),
	m_map(map),
	m_level(level)
{
	switch (level)
	{
	case 1:
		m_animation.setXAnim(4, 5);
		break;

	case 4:
		m_animation.setXAnim(5, 6);
		break;

	case 8:
		m_animation.setXAnim(6, 7);
		break;

	default:
		assert(false && "Invalid level index in exit tile");
	}

	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

void Exit::onCollide(Object * obj)
{
	if (obj->m_type == Object::Hero)
	{
		TiledConverter * converter = m_map.getTiledConverterHandle();
		converter->increaseCurrentLevel(m_level);
		
		std::vector<std::vector<unsigned>> map;
		converter->importTiled(map);

		m_map.loadMap(map);
	}
}
