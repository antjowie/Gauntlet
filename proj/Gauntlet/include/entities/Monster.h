#pragma once
#include "Entity.h"

class Monster : public Entity
{
public:

	enum MonsterType;
	Monster(const unsigned id, const MonsterType monsterType, const unsigned level, const int health,
		const unsigned meleePower, const unsigned moveSpeed,
		const unsigned shootPower, const float shootSpeed, const float shootRate,
		FSM *fsm, PhysicsEngine &physics, Map &map, const unsigned score = 100,
		const unsigned width = 28.f, const unsigned height = 28.f);

	virtual void update(const float elapsedTime) override final;
	virtual void setDead();

	enum MonsterType
	{
		Ghost, 
		Demon,
		Grunt,
		Sorcerer,
		Darkness,
		Lobber,
		Thief
	};

private:

	unsigned m_level;

	const unsigned m_score;
};