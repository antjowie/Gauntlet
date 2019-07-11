#pragma once

class Object;
class Map;
class PhysicsEngine;

class ObjectLibrary
{
public:

	Object *getObject(int id);

	void setMonsterLevel(const unsigned level);
	void setHeroClass(int type);
	
	void initialize(Map *map, PhysicsEngine * physicsEngine);

private:

	Map *m_map;
	PhysicsEngine *m_physicsEngine;

	int m_heroType;
	unsigned m_monsterLevel;
};