#include "objects\AdjacentObject.h"

#include "SpriteManager.h"

AdjacentObject::AdjacentObject(const unsigned id, Map & map, const Object::Type type,const bool isSolid):
	Object(id,type,isSolid),
	m_animation(0,1,0,1,0.f),
	m_map(map)
{
}

void AdjacentObject::getSurroundingTiles(const Object::Type type, bool objectArray[3][3])
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	glm::vec2 pos;
	sprite->GetSpritePosition(m_id, pos);

	std::vector<std::vector<Map::Data*>> data = m_map.getSurroundingTiles(pos.x,pos.y);

	//for (std::vector<std::vector<Map::Data*>>::const_iterator row = data.begin(); row != data.end(); row++)
	//	for (std::vector<Map::Data*>::const_iterator elem = row->begin(); elem != row->end(); elem++)
	for (size_t y = 0; y < 3; y++)
		for (size_t x = 0; x < 3; x++)
		{
			Map::Data * elem = data[y][x];
			if (elem == nullptr) // If it's out of bounds
			{
				objectArray[y][x] = false;
				continue;
			}
			if (elem->tile && !elem->tile->getDead() && elem->tile->m_type == type) // If it is tile
				objectArray[y][x] = true;
			else if (elem->object && !elem->object->getDead()  && elem->object->m_type == type) // If it is object
				objectArray[y][x] = true;
			else // If it isn't the criteria
				objectArray[y][x] = false;
		}
}

AdjacentTile::AdjacentTile(const unsigned id, Map & map, const Object::Type type, const bool isSolid):
	AdjacentObject(id, map, type, isSolid)
{
}

void AdjacentTile::setCorrectFrame()
{
	bool flags[3][3];
	getSurroundingTiles(m_type,flags);

	//value = value
	//	| (int)std::pow(2,((int)flags[0][1] * 0))
	//	| (int)std::pow(2,((int)flags[1][2] * 1))
	//	| (int)std::pow(2,((int)flags[2][1] * 2))
	//	| (int)std::pow(2,((int)flags[1][0] * 3));
	
	//value = value
	//	| (flags[0][1] ? 0b0001: 0)	// 1
	//	| (flags[1][2] ? 0b0010: 0)	// 2
	//	| (flags[2][1] ? 0b0100: 0)	// 4
	//	| (flags[1][0] ? 0b1000: 0);// 8

	// The only reason why I use a char is because 
	// it has 2 bytes of storage, I think. This should safe us
	// a nice 2 bytes cuz integers take 4 bytes, I think.
	// Also, that really doesn't affect our program's performance
	char value = 0x00; 
	value = value
		| (flags[0][1] ? 1 : 0)	
		| (flags[1][2] ? 2 : 0)	
		| (flags[2][1] ? 4 : 0)	
		| (flags[1][0] ? 8 : 0);

	m_animation.setXAnim((unsigned)value, (unsigned)value + 1);
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id,glm::vec2(u1, v1), glm::vec2(u2, v2));
}

AdjacentFloor::AdjacentFloor(const unsigned id, Map & map, const Object::Type type, const bool isSolid):
	AdjacentObject(id, map, type, isSolid)
{
}

void AdjacentFloor::setCorrectFrame()
{
	bool flags[3][3];
	getSurroundingTiles(Wall, flags);

	// The only reason why I use a char is because 
	// it has 2 bytes of storage, I think. This should safe us
	// a nice 2 bytes cuz integers take 4 bytes, I think.
	char value = 0x00;
	value = value
		| (flags[1][0] ? 1 : 0)
		| (flags[2][0] ? 2 : 0)
		| (flags[2][1] ? 4 : 0);
	
	m_animation.setXAnim((unsigned)value, (unsigned)value + 1);
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}