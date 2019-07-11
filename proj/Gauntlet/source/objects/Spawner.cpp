#include "objects/Spawner.h"
#include "objects/AdjacentObject.h"
#include "Map.h"

#include "objects\Projectile.h"
#include "entities\Entity.h"

#include "SpriteManager.h"

#include <cassert>

Spawner::Spawner(const unsigned id, const unsigned level, Monster::MonsterType type, Map &map, PhysicsEngine &physics, SpawnCallback spawnCallback) :
	Object(id, Object::Type::Spawner, true),
	m_spawnerType(type),
	m_map(map),
	m_physics(physics),
	m_spawnCallback(spawnCallback),
	m_animation(10,11,0,1,0.f),
	m_spawnTimer(0),
	m_health(10)
{
	NHTV::SpriteManager * sprite = NHTV::SpriteManager::GetInstance();

	// Map object id to monster id
	unsigned monsterIndex = type == Monster::MonsterType::Ghost ? 1:0;
	monsterIndex += 5;

	float uMin, vMin, uMax, vMax;
	m_animation.setYAnim(monsterIndex, monsterIndex + 1);
	m_animation.getCurrentFrame(uMin, vMin, uMax, vMax);
	sprite->SetSpriteUVCoordinates(id, glm::vec2{ uMin,vMin }, glm::vec2{ uMax,vMax });
}

void Spawner::update(const float elapsedTime)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();

	if (m_health < 0.f)
		setDead();

	// Update texture frame
	m_animation.update(elapsedTime);
	//m_animation.setAnimation(0);

	//float u1, u2, v1, v2;
	//m_animation.getCurrentAnimation(u1, v1, u2, v2);
	//sprite->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));

	// Update spawn and check if can spawn
	m_spawnTimer += elapsedTime;
	if (m_spawnTimer > 1.f)
	{
		m_spawnTimer = 0;

		glm::vec2 pos;
		sprite->GetSpritePosition(m_id, pos);

		std::vector<std::vector<Map::Data*>> data = m_map.getSurroundingTiles(pos.x,pos.y);

		// Map the data to a normal vector so we can pick a random element out of it
		std::vector<Map::Data*> randomizer;
		for(std::vector<std::vector<Map::Data*>>::const_iterator y = data.begin(); y != data.end(); y++)
			for (std::vector<Map::Data*>::const_iterator obj = y->begin(); obj != y->end(); obj++)
			{
				Object * object= (*obj)->object;
				if ((*obj)->tile->m_isSolid)
					continue;
				if(object != nullptr)
					continue;
				if (object == this)
					continue;
				randomizer.push_back(*obj);				
			}

		std::random_shuffle(randomizer.begin(), randomizer.end());

		for (size_t i = 0; i < randomizer.size(); i++)
		{
			Map::Data *mapData = randomizer[i];
			sprite->GetSpritePosition(mapData->tile->m_id,pos);
		
			// Check if there is no object on that spot
			bool collided = false;
			std::vector<std::vector<Map::Data*>> surroundingObjects = m_map.getSurroundingTiles(pos.x,pos.y);
			
			// Check collision for every surrounding object on potential new spawn position
			for (std::vector<std::vector<Map::Data*>>::iterator y = surroundingObjects.begin(); y != surroundingObjects.end() && !collided; y++)
				for(std::vector<Map::Data*>::iterator x = y->begin(); x != y->end() && !collided; x++)
				{
					if (!(*x)->object)
						continue;
						
					glm::vec2 obj2Pos;
					NHTV::SpriteManager::GetInstance()->GetSpritePosition((*x)->object->m_id, obj2Pos);
					glm::vec2 delta = obj2Pos-pos;
					
					if (std::abs(delta.x) < 32.f && std::abs(delta.y) < 32.f)
						collided = true;
				}
		
			if (collided)
				continue;
		
			// If this is called, there were no collisions, so we can create the object
			Object * obj = m_spawnCallback(sprite->AddSprite("images/spriteSheet.png", glm::vec2(32.f), glm::vec2(.5f), glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec4(1.f)), m_map, m_physics, 1);
			sprite->MoveSprite(obj->m_id, glm::value_ptr(pos));
		
			Object * location = m_map.posToCell(pos)->object;
			assert(!location && "Spawner tries to overwrite existing object");
			m_map.posToCell(pos)->object = obj;
			return;
		}
	}
}

void Spawner::onCollide(Object * obj)
{
	switch (obj->m_type)
	{
	case Object::Type::Flyable:
		m_health -= static_cast<Projectile*>(obj)->m_damage;
		break;

	case Object::Type::Hero:
		m_health -= static_cast<Entity*>(obj)->getMeleeDamage();
		break;
	}
}