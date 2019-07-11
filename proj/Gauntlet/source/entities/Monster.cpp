#include "entities\Monster.h"

#include "entities\Player.h"

#include "Map.h"

#include "SpriteManager.h"

Monster::Monster(const unsigned id, const MonsterType monsterType, 
	const unsigned level, const int health, const unsigned meleePower, 
	const unsigned moveSpeed, 
	const unsigned shootPower, const float shootSpeed, const float shootRate,
	FSM * fsm, PhysicsEngine &physics, Map &map, const unsigned score,
	const unsigned width, const unsigned height):

	Entity(id, Object::Enemy, health,meleePower,moveSpeed,shootPower,shootSpeed,shootRate, fsm, physics, map, 18-monsterType,width,height),
	m_level(level),
	m_score(score)
{
}

void Monster::update(const float elapsedTime)
{
	Entity::update(elapsedTime);
	if (getHealth() < 0)
		m_level--;
	if (m_level == 0)
		setDead();
}

void Monster::setDead()
{
	Entity::setDead();
	glm::vec2 playerPos;
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(m_map.getPlayerID(), playerPos);
	static_cast<Player*>(m_map.posToCell(playerPos)->object)->getInventory()->addScore(m_score);
}
