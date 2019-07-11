#include "objects\Projectile.h"
#include "objects\Explosion.h"
#include "entities\Entity.h"

#include "Map.h"
#include "SpriteManager.h"
#include "PhysicsEngine.h"

Projectile::Projectile(const glm::vec2 direction, ProjectileType type, const unsigned damage, const float speed, Map &map,PhysicsEngine &physics, bool damageHero) :
	Object(NHTV::SpriteManager::GetInstance()->AddSprite("images/spriteSheet.png",
		glm::vec2(32.f), glm::vec2(0.5f), glm::vec4(0, 0, 1, 1), glm::vec4(1.f)), Object::Flyable, true,26,26),
	m_speed(speed),
	m_damage(damage),
	m_direction(direction),
	m_animation(24, 32, 2 + (int)type, 3 + (int)type, 0.1f),
	m_map(map),
	m_physics(physics),
	m_damageHero(damageHero)
{
	unsigned offset;
	if (direction == glm::vec2(0.f))
		assert(false && "This should not happen");
	else if (direction.y > 0 && direction.x == 0)
		offset = 0;
	else if (direction.y > 0 && direction.x > 0)
		offset = 1;
	else if (direction.y == 0 && direction.x > 0)
		offset = 2;
	else if (direction.y < 0 && direction.x > 0)
		offset = 3;
	else if (direction.y < 0 && direction.x == 0)
		offset = 4;
	else if (direction.y < 0 && direction.x < 0)
		offset = 5;
	else if (direction.y == 0 && direction.x < 0)
		offset = 6;
	else if (direction.y > 0 && direction.x < 0)
		offset = 7;

	// Set initial sprite
	float u1, v1, u2, v2;
	m_animation.setCurrentFrame(offset);
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id,glm::vec2(u1,v1),glm::vec2(u2,v2));
}

Projectile::~Projectile()
{
	glm::vec2 pos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_id, pos);
	m_map.pushProjectile(new Explosion(pos, Explosion::Small));
}

void Projectile::update(const float elapsedTime)
{
	// Update movement
	glm::vec2 pos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_id, pos);
	pos += m_direction * elapsedTime * m_speed;
	m_physics.requestMove(this, pos.x,pos.y,false,false);
}

void Projectile::onCollide(Object *obj)
{
	setDead();
}

//--------------------------------------------------

ProjectileLauncher::ProjectileLauncher(const Projectile::ProjectileType & projectileType, ProjectileSpawn projectileSpawnFunction, Map & map, PhysicsEngine & physics, bool damageHero):
	m_projectileType(projectileType),
	m_projectileSpawn(projectileSpawnFunction),
	m_map(map),
	m_physics(physics),
	m_damageHero(damageHero),

	m_entity(nullptr),
	m_direction(1.f)
{
}

void ProjectileLauncher::setEntity(Entity * entity)
{
	m_entity = entity;
}

void ProjectileLauncher::setDirection(const glm::vec2 &direction)
{
	if (direction == glm::vec2(0))
		return;

	m_direction = direction;

	if (m_direction.x != 0) m_direction.x /= std::abs(m_direction.x);
	if (m_direction.y != 0) m_direction.y /= std::abs(m_direction.y);
}

void ProjectileLauncher::update(const float elapsedTime)
{
	m_cooldown -= elapsedTime;
	if (m_cooldown < 0.f) m_cooldown = 0.f;
}

Projectile* ProjectileLauncher::shoot()
{
	if (m_cooldown != 0.f)
		return nullptr;

	m_cooldown = m_entity->getShootRate();
	Projectile * projectile = m_projectileSpawn(m_direction,m_projectileType,m_entity->getShootPower(),m_entity->getShootSpeed(),m_map,m_physics,m_damageHero);
	
	// Calculate initial position of projectile
	glm::vec2 origPos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_entity->m_id, origPos);
	glm::vec2 projectilePos(origPos + m_direction * 32.f);
	Object * object = m_map.getSurroundingTiles(projectilePos.x, projectilePos.y)[1][1]->object;
	NHTV::SpriteManager::GetInstance()->MoveSprite(projectile->m_id, glm::value_ptr(projectilePos));
	
	m_map.pushProjectile(projectile);
	
	return projectile;
}