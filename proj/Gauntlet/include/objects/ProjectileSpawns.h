#pragma once
#include "Projectile.h"

//using ProjectileSpawn = Projectile * (*)(const glm::vec2 direction, Projectile::ProjectileType type,
//	const unsigned damage, const float speed, Map &map, PhysicsEngine & physics, const bool damageHero);

inline Projectile * spawnProjectile(const glm::vec2 direction, const Projectile::ProjectileType projectileType, 
	const unsigned damage, const float speed, Map &map, PhysicsEngine & physics, const bool damageHero)
{
	return new Projectile(direction, projectileType, damage, speed, map, physics, damageHero);
}