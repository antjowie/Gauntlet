#include "entities\Player.h"
#include "objects\Door.h"
#include <iostream>

Player::Player(const unsigned id, const HeroType heroType, const int health, 
	const unsigned meleePower, const unsigned moveSpeed, 
	const unsigned magic, const unsigned armor, 
	const unsigned shootPower, const float shootSpeed, const float shootRate,
	FSM * fsm, PhysicsEngine & physicsEngine, Map & map, 
	const unsigned width, const unsigned height):
	Entity(id,Hero,health,meleePower, moveSpeed,shootPower,shootSpeed,shootRate,fsm, physicsEngine, map,11 - (int)heroType,width,height),
	m_inventory(new Inventory()),
	m_dotTimeline(0)
{
}

void Player::onCollide(Object * obj)
{
	Entity::onCollide(obj);
	switch (obj->m_type)
	{
	case Object::Type::Key:
		m_inventory->addKey();
		break;

	case Object::Type::Potion:
		m_inventory->addPotion();
		obj->setDead();
		break;

	case Object::Type::Gate:
		if (m_inventory->popKey())
			obj->setDead();
		break;

	case Object::Type::Food:
		gainHealth(100);
		break;

	case Object::Type::Treasure:
		m_inventory->addScore(300);
		obj->setDead();
	}
}

void Player::update(const float elapsedTime)
{
	Entity::update(elapsedTime);

	m_dotTimeline -= elapsedTime;
	if (m_dotTimeline < 0.f)
	{
		loseHealth(1,false);
		m_dotTimeline = 1.f;
	}
}

Inventory * Player::getInventory()
{
	return m_inventory;
}
