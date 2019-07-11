#pragma once
#include "Object.h"
#include "glm/vec2.hpp"
#include "Animation.h"

class Map;
class PhysicsEngine;
class Entity;

class Projectile : public Object
{
public:

	enum ProjectileType;
	Projectile(const glm::vec2 direction, ProjectileType type, const unsigned damage, const float speed, Map &map, PhysicsEngine &physics, const bool damageHero);
	virtual ~Projectile();

	virtual void update(const float elapsedTime) override;
	virtual void onCollide(Object *obj) override final;

	enum ProjectileType
	{
		LobberBall,
		PurpleFireball,
		Arrow,
		Fireball,
		Sword,
		Axe,
		None
	};

	const unsigned m_damage;
	const bool m_damageHero;

private:

	Map &m_map;
	PhysicsEngine & m_physics;
	Animation m_animation;

	const glm::vec2 m_direction;
	const float m_speed;
};

using ProjectileSpawn = Projectile * (*)(const glm::vec2 direction, Projectile::ProjectileType type,
	const unsigned damage, const float speed, Map &map, PhysicsEngine & physics, const bool damageHero);

class ProjectileLauncher
{
public:
	
	ProjectileLauncher(const Projectile::ProjectileType & projectileType, ProjectileSpawn projectileSpawnFunction, Map &map, PhysicsEngine &physics, bool damageHero);

	void setEntity(Entity *entity);
	void setDirection(const glm::vec2 &direction);

	void update(const float elapsedTime);
	Projectile* shoot();

private:

	Map &m_map;
	PhysicsEngine & m_physics;

	ProjectileSpawn m_projectileSpawn;
	Projectile::ProjectileType m_projectileType;
	glm::vec2 m_direction;

	Entity * m_entity;
	float m_cooldown;

	const bool m_damageHero;
};