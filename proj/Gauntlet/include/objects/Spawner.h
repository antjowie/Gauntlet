#pragma once
#include "entities\Monster.h"
#include "Animation.h"

class Map;
class PhysicsEngine;

using SpawnCallback = Monster * (*)(const unsigned, Map &, PhysicsEngine &, const unsigned);

class Spawner : public Object
{
public:
	
	Spawner(const unsigned id, const unsigned level, Monster::MonsterType spawnerType, Map &map, PhysicsEngine &physics, SpawnCallback spawnCallback);
	
	virtual void update(const float elapsedTime) override final;
	virtual void onCollide(Object *obj) override final;

	const Monster::MonsterType m_spawnerType;

private:

	Map &m_map;
	PhysicsEngine &m_physics;
	SpawnCallback m_spawnCallback;
	Animation m_animation;

	float m_spawnTimer;
	int m_health;
};