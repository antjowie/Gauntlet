#include "commands\PlayerCommand.h"

#include "InputHandler.h"

#include "SpriteManager.h"
#include "Enumerations.h"

void PlayerKeyboardMove::excecute(Entity *entity,const float elapsedTime)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	
	glm::vec2 pos;
	sprite->GetSpritePosition(entity->m_id, pos);
	
	// Update position 
	glm::vec2 direction;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_D)) direction.x += (float)entity->getSpeed() * elapsedTime;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_A))	direction.x -= (float)entity->getSpeed() * elapsedTime;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_W))	direction.y += (float)entity->getSpeed() * elapsedTime;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_S))	direction.y -= (float)entity->getSpeed() * elapsedTime;
	pos += direction;

	entity->setLookDirection(direction);

	glm::vec2 movement;
	movement = pos;
	
	if (movement.x != 0) movement.x /= std::abs(movement.x);
	if (movement.y != 0) movement.y /= std::abs(movement.y);
	
	if(!InputHandler::getInstance().isKeyDown(NHTV::KEY_SPACE))
		entity->m_physics.requestMove(entity, pos.x, pos.y,false,true);

	// NOTE: This should not belong here because walk command
	// should not know about shoot command
	// If player is shooting don't move
	//glm::vec2 oldMovememnt = entity->m_movement;
	//entitye->m_movement = pos - oldPos;
	//if (InputHandler::getInstance().isKeyDown(NHTV::KEY_SPACE) && pos - oldPos == glm::vec2(0))
	//	player->m_movement = oldMovememnt;
	//
	//// Request move if object has moved and isn't shooting
	//if (pos != oldPos && !InputHandler::getInstance().isKeyDown(NHTV::KEY_SPACE))
	//	m_physics.requestMove(obj, pos.x, pos.y);
}

void PlayerKeyboardShoot::excecute(Entity * entity, const float elapsedTime)
{
	m_launcher->update(elapsedTime);
	
	// Get direction player is looking in
	glm::vec2 direction(0.f);
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_D)) direction.x += 1;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_A))	direction.x -= 1;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_W))	direction.y += 1;
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_S))	direction.y -= 1;

	m_launcher->setDirection(direction);
	if (InputHandler::getInstance().isKeyDown(NHTV::KEY_SPACE))
		m_launcher->shoot();
}

PlayerKeyboardShoot::PlayerKeyboardShoot(ProjectileLauncher * launcher):
	m_launcher(launcher)
{
}

PlayerKeyboardShoot::~PlayerKeyboardShoot()
{
	delete m_launcher;
}
