#include "objects/AdjacentObject.h"

#include "PhysicsEngine.h"

#include "SpriteManager.h"

#include <cassert>

PhysicsEngine::PhysicsEngine(Map & map):
	m_map(map)
{
}

void PhysicsEngine::requestMove(Object * const obj, const float x, const float y, const bool cellBased, const bool validateMovement, const bool correctMovement)
{
	m_objects.push_back(Movement(obj, x, y,cellBased, validateMovement, correctMovement));
}

void PhysicsEngine::pushListener(Listener * listener)
{
	m_listeners.push_back(listener);
}

void PhysicsEngine::cellBasedCollision(Movement & obj)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();

	// If object exists on that place
	// Get cell object wants to move to
	glm::vec2 fromPos;
	glm::vec2 toPos;

	// Calculate direction vector to get cell that object wants to move to

	Object *obj2 = m_map.posToCell(glm::vec2(obj.x, obj.y))->object;
	if (obj2)
	{
		obj.obj->onCollide(obj2);
		obj2->onCollide(obj.obj);

		// Push collision to listeners
		for (std::vector<Listener*>::iterator elem = m_listeners.begin(); elem != m_listeners.end(); elem++)
			(*elem)->onEvent(obj.obj, obj2);
		
		if (!obj.obj->getDead() && !obj.validateMovement)
		{
			sprite->MoveSprite(obj.obj->m_id, glm::value_ptr(glm::vec2(obj.x, obj.y)));
		}
	}
	else
		sprite->MoveSprite(obj.obj->m_id, glm::value_ptr(glm::vec2(obj.x, obj.y)));
}

void PhysicsEngine::AABBCollision(Movement & obj)
{
	// Check if object isn't trying to overwrite an existing location
	Object *object = m_map.posToCell(glm::vec2(obj.x, obj.y))->object;
	if (object && object != obj.obj)
	{
		obj.obj->onCollide(object);
		object->onCollide(obj.obj);
	}
	else
	{
		std::vector<std::vector<Map::Data*>> data = m_map.getSurroundingTiles(obj.x,obj.y);
		glm::vec2 origPos;
		NHTV::SpriteManager::GetInstance()->GetSpritePosition(obj.obj->m_id, origPos);

		std::vector<Object*> objects;
		for (std::vector<std::vector<Map::Data*>>::iterator y = data.begin(); y != data.end(); y++)
			for (std::vector<Map::Data*>::iterator x = y->begin(); x != y->end(); x++)
				if((*x)->object)
					objects.push_back((*x)->object);
		
		// Get the closest objects to calculate first
		std::sort(objects.begin(), objects.end(), [&obj](Object* obj1, Object* obj2)
		{
			glm::vec2 pos1;
			glm::vec2 pos2;
			NHTV::SpriteManager::GetInstance()->GetSpritePosition(obj1->m_id, pos1);
			NHTV::SpriteManager::GetInstance()->GetSpritePosition(obj2->m_id, pos2);

			const float mag1 = std::powf(obj.x - pos1.x, 2) + std::powf(obj.y - pos1.y, 2);
			const float mag2 = std::powf(obj.x - pos2.x, 2) + std::powf(obj.y - pos2.y, 2);
			// The smallest magnitude will be placed up front
			return mag1 < mag2;
		});

		for(std::vector<Object*>::iterator x = objects.begin(); x != objects.end(); x++)
		{
			Object *obj2 = (*x);;
			if (!obj2 || obj2 == obj.obj)
				continue;
		
			// Get positions
			glm::vec2 pos1;
			glm::vec2 pos2;
			pos1 = glm::vec2(obj.x, obj.y);
			NHTV::SpriteManager::GetInstance()->GetSpritePosition(obj2->m_id, pos2);

			// Convert to lower left
			pos1.x -= obj.obj->m_width *0.5f;
			pos1.y -= obj.obj->m_height*0.5f;
			pos2.x -= obj2->m_width *0.5f;
			pos2.y -= obj2->m_height*0.5f;

			// Do AABB
			if (pos1.x < pos2.x + obj2->m_width &&
				pos1.x + obj.obj->m_width > pos2.x &&
				pos1.y < pos2.y + obj2->m_height &&
				pos1.y + obj.obj->m_height > pos2.y)
			{	
				obj.obj->onCollide(obj2);
				obj2->onCollide(obj.obj);

				// Push collision to listeners
				for (std::vector<Listener*>::iterator elem = m_listeners.begin(); elem != m_listeners.end(); elem++)
					(*elem)->onEvent(obj.obj, obj2);
		
				if (!obj.validateMovement)
					continue;
		
				if (obj.correctMovement)
				{
					// Calculate least penetration axis
					const float x = pos2.x - pos1.x;
					const float y = pos2.y - pos1.y;

					// Penetrated x axis
					if (std::abs(x) > std::abs(y))
					{
						// If x is negative, object is left side of x
						const float width = (obj.obj->m_width + obj2->m_width) * 0.5f;
						obj.x = pos2.x + (x > 0 ? -width * 0.5f: width * 1.5f);
					}
					else if (std::abs(x) < std::abs(y))
					{
						const float height = (obj.obj->m_height+ obj2->m_height) * 0.5f;
						obj.y = pos2.y + (y > 0 ? -height* 0.5f: height* 1.5f);
					}
					else
					{
						obj.x = origPos.x;
						obj.y = origPos.y;
					}
				}
				else
				{
					obj.x = origPos.x;
					obj.y = origPos.y;
				}
			}
		}

		NHTV::SpriteManager::GetInstance()->MoveSprite(obj.obj->m_id, glm::value_ptr(glm::vec2(obj.x, obj.y)));
	}
}

PhysicsEngine::Movement::Movement(Object * obj, const float x, const float y, bool cellBased, bool validateMovement, bool correctMovement):
	obj(obj),
	x(x),
	y(y),
	validateMovement(validateMovement),
	cellBased(cellBased),
	correctMovement(correctMovement)
{
}

void PhysicsEngine::moveObjects()
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	for (std::vector<Movement>::iterator obj = m_objects.begin(); obj != m_objects.end(); obj++)
	{
		// Remove from map if needed
		glm::vec2 origPos;
		sprite->GetSpritePosition(obj->obj->m_id, origPos);

		// Check if it has an position on the map
		// Projectiles for example don't occupy a space on the map
		bool isOnMap = false;
		if (m_map.posToCell(origPos)->object && m_map.posToCell(origPos)->object->m_id == obj->obj->m_id)
		{
			isOnMap = true;
			m_map.posToCell(origPos)->object = nullptr;
		}

		if (obj->cellBased)
			cellBasedCollision(*obj);
		else
			AABBCollision(*obj);
	
		// Place back in map if needed
		if (isOnMap)
		{
			sprite->GetSpritePosition(obj->obj->m_id, origPos);
			assert(!m_map.posToCell(origPos)->object&& "Physics Engine overwrote existing sprite");
			m_map.posToCell(origPos)->object = obj->obj;
		}

	}
	m_objects.clear();
}