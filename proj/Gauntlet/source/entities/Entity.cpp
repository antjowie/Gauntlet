#include "entities\Entity.h"
#include "objects\Explosion.h"

#include "fsms\FSM.h"

#include "Map.h"
#include "objects\Projectile.h"

#include "SpriteManager.h"
#include "Utilities.h"

Entity::Entity(const unsigned id, const Object::Type type, const int health, 
	const unsigned meleePower, const unsigned moveSpeed, 
	const unsigned shootPower, const float shootSpeed, const float shootRate, 
	FSM * fsm, PhysicsEngine & physics, Map & map, 
	const unsigned yAnimIndex, const unsigned width, const unsigned height):
	Object(id,type,true,width,height),
	m_animation(0,4,yAnimIndex,yAnimIndex + 1,0.2f),
	
	m_health(health),
	m_meleePower(meleePower),
	m_moveSpeed(moveSpeed),
	m_shootPower(shootPower),
	m_shootSpeed(shootSpeed),
	m_shootRate(shootRate),
	m_fsm(fsm),
	m_physics(physics),
	m_map(map),
	
	m_meleeTimeline(0),
	m_meleeCooldown(0.25f)
{
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

Entity::~Entity()
{
	delete m_fsm;
	glm::vec2 pos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_id, pos);
	m_map.pushProjectile(new Explosion(pos, Explosion::ExplosionType::Large));
}

void Entity::update(const float elapsedTime)
{
	// Update melee cooldown
	m_meleeTimeline -= elapsedTime;
	if (m_meleeTimeline < 0.f)
		m_meleeTimeline = 0.f;

	// Update blink timeline
	m_blinkTimeline -= elapsedTime;
	if (m_blinkTimeline <= 0.f)
	{
		m_blinkTimeline = 0.f;
		NHTV::SpriteManager::GetInstance()->SetSpriteColour(m_id, glm::vec4(1.f));
	}
	else
		NHTV::SpriteManager::GetInstance()->SetSpriteColour(m_id, glm::vec4(255.f,255,255,1));

	m_fsm->update(elapsedTime);
}

void Entity::updateAfterMove(const float elapsedTime)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	m_fsm->updateAfterMove(elapsedTime);

	m_animation.setMovement(m_moveDirection);
	m_animation.update(elapsedTime);
	
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	sprite->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

void Entity::onCollide(Object * obj)
{
	if(obj->m_type != m_type)
		switch (obj->m_type)
		{
		case Object::Type::Enemy:
		case Object::Type::Hero:
		{
			Entity * entity = static_cast<Entity*>(obj);
			loseHealth(entity->getMeleeDamage(),true);
			break;
		}
			
		case Object::Type::Flyable:	
		{

			Projectile *projectile = static_cast<Projectile*>(obj);
			loseHealth(projectile->m_damage,true);
			
			break;
		}
		}
}

void Entity::loseHealth(const unsigned amount, bool blink)
{
	m_health -= amount;
	if(amount != 0 && blink)
		m_blinkTimeline = 0.1f;
}

void Entity::gainHealth(const unsigned amount)
{
	m_health += amount;
}

int Entity::getHealth() const
{
	return m_health;
}

float Entity::getSpeed() const
{
	return 32.f + m_moveSpeed * 16.f;
}

unsigned Entity::getMeleeDamage()
{
	if (m_meleeTimeline == 0.f)
	{
		m_meleeTimeline = m_meleeCooldown;
		return m_meleePower;
	}
	return 0;
}

unsigned Entity::getShootPower() const
{
	return m_shootPower;
}

float Entity::getShootSpeed() const
{
	return m_shootSpeed;
}

float Entity::getShootRate() const
{
	return m_shootRate;
}

void Entity::setLookDirection(const glm::vec2 & direction)
{
	m_moveDirection = direction;
}

void Entity::setInvisibility(const bool isInvisible)
{
	m_isInvisible = isInvisible;
}

bool Entity::getInvisibility() const
{
	return m_isInvisible;
}