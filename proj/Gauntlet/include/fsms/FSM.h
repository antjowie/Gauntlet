#pragma once
#include"commands\Command.h"

#include "entities\Entity.h"
#include "PhysicsEngine.h"
#include "Map.h"

class FSM
{
public:

	virtual ~FSM();

	void setEntityHandle(Entity *entity);

	virtual void update(const float elapsedTime) = 0;
	virtual void updateAfterMove(const float elapsedTime);
	
protected:

	Entity *m_entity;
};