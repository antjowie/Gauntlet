#include "fsms\MoveFSM.h"
#include "SpriteManager.h"

MoveFSM::MoveFSM(Command * move):
	m_coMove(move)
{
}

MoveFSM::~MoveFSM()
{
	delete m_coMove;
}

void MoveFSM::update(const float elapsedTime)
{
	m_coMove->excecute(m_entity, elapsedTime);
	//NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	//glm::vec2 playerPos;
	//glm::vec2 pos;
	//
	//sprite->GetSpritePosition(m_map.getPlayerID(), playerPos);
	//sprite->GetSpritePosition(m_entity.m_id,pos);
	//
	//glm::vec2 movement = playerPos - pos;
	//movement.x /= std::abs(movement.x);
	//movement.y /= std::abs(movement.y);
	//
	//movement *= m_entity.getSpeed() * 16.f * elapsedTime;
	//m_physics.requestMove(&m_entity, movement.x, movement.y);
}