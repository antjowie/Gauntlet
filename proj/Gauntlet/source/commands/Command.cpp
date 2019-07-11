#include "commands\Command.h"

#include "SpriteManager.h"

//void ShootCommand::excecute(Object * obj, const float elapsedTime)
//{
//	glm::vec2 pos;
//	NHTV::SpriteManager::GetInstance()->GetSpritePosition(obj->m_id, pos);
//	Projectile * projectile = m_launcher->shoot(m_direction, pos);
//
//	// Check if projectile is not spawned in wall
//	//glm::vec2 projectilePos(pos + m_direction * 32.f);
//	//Object * object = m_map.getSurroundingTiles(projectilePos.x, projectilePos.y)[1][1]->object;
//	//
//	//sprite->MoveSprite(projectile->m_id, glm::value_ptr(projectilePos));
//	//m_map.pushProjectile(projectile);
//	//if (object)
//	//{
//	//	object->onCollide(projectile);
//	//	projectile->onCollide(object);
//	//}
//	//
//	//player->m_movement = m_movement;
//}
//
//void ShootCommand::setDirection(const glm::vec2 & direction)
//{
//	if (direction == glm::vec2(0))
//		return;
//
//	m_direction = direction;
//	if (m_direction.x != 0.f) m_direction.x /= std::abs(m_direction.x);
//	if (m_direction.y != 0.f) m_direction.y /= std::abs(m_direction.y);
//}