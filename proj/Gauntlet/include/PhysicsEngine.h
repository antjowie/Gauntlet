#pragma once
#include <vector>
#include "Map.h"

class Object;
class Map;

class Listener
{
public:
	virtual void onEvent(Object *obj1, Object * obj2) = 0;
};

// The physics engine checks whether movement is valid or not. It has very tight 
// coupling with the map class and thus is a friend of map
class PhysicsEngine
{
public:

	// We need the map to check collision with.
	// By using the map, we can save on quite a few operations
	PhysicsEngine(Map &map);

	// CellBased false uses AABB collision checks. It is a more expensive collision check and allows movement
	// disregarinding the cells. It can move freely and doesn't take cells into consideration, only surrounding objects
	// CellBased true is cheaper and is used by monsters, it doesn't do AABB collision checks.
	// It does a cell based collision check. If the cell is occupied, it will count as a collision and the
	// object won't move.
	// validateMovement checks if the movement should even be validated or if collisions should only happen
	// correct movement decides if the object should snap whenever it touches something or if its false, that it should just not move
	void requestMove(Object * const obj, const float x, const float y, const bool cellBased, const bool validateMovement = true, bool correctMovement = true);

	void moveObjects();

	void pushListener(Listener * listener);

private:

	struct Movement;

	void cellBasedCollision(Movement & obj);
	void AABBCollision(Movement & obj);

	struct Movement
	{
		Movement(Object *obj, const float x, const float y, bool cellBased, bool validateMovement, bool correctMovement);

		Object *obj;
		float x, y;
		
		const bool validateMovement;
		const bool correctMovement;
		const bool cellBased;
	};

	std::vector<Movement> m_objects;
	std::vector<Listener*>m_listeners;

	Map &m_map;
};