#pragma once
#include "Command.h"

#include "glm\vec2.hpp"
#include "objects\Projectile.h"

// This class updates player position and monster position, it is usefull because
// these values are needed for almost every monster command in Gauntlet
class MonsterCommand : public Command
{
public:
	// This excecute function updates the protected variables
	virtual void excecute(Entity *entity, const float elapsedTime) override;

protected:

	glm::vec2 m_playerPos;
	glm::vec2 m_monsterPos;
	glm::vec2 m_distance;
};

class MonsterWalk : public MonsterCommand
{
public:
	MonsterWalk();
	virtual void excecute(Entity *entity, const float elapsedTime) override final;

private:

	glm::vec2 m_oldPos;

	glm::vec2 m_targetTile;
	glm::vec2 m_previousTile;
	
	glm::vec2 m_direction;

	float m_moveProgress;
};

class MonsterShoot: public Command
{
public:

	MonsterShoot(ProjectileLauncher * launcher);
	virtual ~MonsterShoot();

	virtual void excecute(Entity *entity, const float elapsedTime) override final;

private:

	ProjectileLauncher *m_launcher;
};