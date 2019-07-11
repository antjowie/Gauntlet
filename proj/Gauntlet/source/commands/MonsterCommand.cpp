#include "commands\MonsterCommand.h"

#include "SpriteManager.h"
#include "Utilities.h"

void MonsterCommand::excecute(Entity * entity, const float elapsedTime)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	
	sprite->GetSpritePosition(entity->m_map.getPlayerID(), m_playerPos);
	sprite->GetSpritePosition(entity->m_id, m_monsterPos);

	m_distance = m_playerPos - m_monsterPos;
}

// Monster Walk
//----------------------------------------------------------

MonsterWalk::MonsterWalk():
	m_targetTile(0),
	m_moveProgress(1.f),
	m_oldPos(0)
{
}

void MonsterWalk::excecute(Entity *entity, const float elapsedTime)
{
	MonsterCommand::excecute(entity, elapsedTime);
	
	//m_searchCooldown -= elapsedTime;
	//if (m_searchCooldown <= 0.f)
	//{
	//	m_searchCooldown = 0.5f;

	if (m_moveProgress == 1.f)
	{
		m_moveProgress = 0.f;

		// Get the cell position of the entities
		glm::vec2 targetPos;
		NHTV::SpriteManager::GetInstance()->GetSpritePosition(entity->m_map.posToCell(m_playerPos)->tile->m_id, targetPos);

		// Calculate the direction we have to walk towards
		m_direction = targetPos - m_monsterPos;

		// The abs function call makes sure that negative values stay negative
		// We just calcalute to a normal/unit because the game doesn't normalize values
		if (m_direction.x != 0) m_direction.x /= std::abs(m_direction.x);
		if (m_direction.y != 0) m_direction.y /= std::abs(m_direction.y);
	
		// If place is occupied, don't move, just wait
		Map::Data * cell;
		
		// Check if corner is occupied
		cell = entity->m_map.posToCell(m_monsterPos + m_direction * 32.f);
		if (cell->isClaimed || (cell->object && cell->object->m_type != Object::Hero))
		{
			glm::vec2 mag = m_monsterPos - m_playerPos;
			if (std::abs(mag.x) > std::abs(mag.y))
				m_direction.y = 0;
			else
				m_direction.x = 0;
		}

		// Check collision on x axis
		cell = entity->m_map.posToCell(m_monsterPos + glm::vec2(m_direction.x, 0) * 32.f);
		if (m_direction.x != 0 && (cell->isClaimed || cell->object && cell->object->m_type != Object::Hero))
			m_direction.x = 0;
		
		// Check collision on y axis
		cell = entity->m_map.posToCell(m_monsterPos + glm::vec2(0, m_direction.y) * 32.f);
		if (m_direction.y != 0 && (cell->isClaimed || cell->object && cell->object->m_type != Object::Hero))
			m_direction.y = 0;

		m_targetTile = m_monsterPos + m_direction * 32.f;

		m_previousTile = m_monsterPos;
		
		// Update claimed states because some monsters tend to walk into eachother
		entity->m_map.posToCell(m_previousTile)->isClaimed = false;
		cell = entity->m_map.posToCell(m_targetTile);
		if(cell->object && cell->object->m_type != Object::Hero)
			cell->isClaimed = true;

		//// Change color for debuggin purposes
		//NHTV::SpriteManager::GetInstance()->SetSpriteColour(entity->m_map.posToCell(m_previousTile)->tile->m_id, glm::vec4(1, 1, 1, 1));
		//NHTV::SpriteManager::GetInstance()->SetSpriteColour(entity->m_map.posToCell(m_targetTile)->tile->m_id, glm::vec4(1, 0, 0, 1));
	}

	glm::vec2 movement;

	// Check if object actually has moved
	if(m_monsterPos == m_previousTile || m_oldPos != m_monsterPos)
		m_moveProgress += entity->getSpeed() / 32.f * elapsedTime;
	if (m_moveProgress > 1.f)
		m_moveProgress = 1.f;

	movement.x = NHTV::Lerp(m_previousTile.x, m_targetTile.x, m_moveProgress);
	movement.y = NHTV::Lerp(m_previousTile.y, m_targetTile.y, m_moveProgress);

	m_oldPos = m_monsterPos;
	entity->m_physics.requestMove(entity, movement.x, movement.y,false,true,false);

	// Update movement texture
	if (m_direction != glm::vec2(0.f))
		entity->setLookDirection(m_direction);
}

// Monster Shoot
//----------------------------------------------------------

MonsterShoot::MonsterShoot(ProjectileLauncher * launcher) :
	m_launcher(launcher)
{
}

MonsterShoot::~MonsterShoot()
{
	delete m_launcher;
}

void MonsterShoot::excecute(Entity *entity, const float elapsedTime)
{
	glm::vec2 playerPos;
	glm::vec2 monsterPos;

	NHTV::SpriteManager::GetInstance()->GetSpritePosition(entity->m_map.getPlayerID(), playerPos);
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(entity->m_id, monsterPos);

	NHTV::SpriteManager::GetInstance()->GetSpritePosition(entity->m_map.posToCell(playerPos)->tile->m_id,playerPos);
	NHTV::SpriteManager::GetInstance()->GetSpritePosition(entity->m_map.posToCell(monsterPos)->tile->m_id,monsterPos);

	glm::vec2 direction = playerPos - monsterPos;

	entity->setLookDirection(direction);
	m_launcher->setDirection(direction);
	m_launcher->update(elapsedTime);
	m_launcher->shoot();
}