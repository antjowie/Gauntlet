#include "fsms\FSM.h"
#include "SpriteManager.h"
#include "objects\Explosion.h"

FSM::~FSM()
{
}

void FSM::setEntityHandle(Entity * entity)
{
	m_entity = entity;
}

void FSM::updateAfterMove(const float elapsedTime)
{
}