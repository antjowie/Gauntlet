#include "ObjectLibrary.h"

#include "EntitySpawns.h"
#include "entities\Player.h"
#include "entities\Monster.h"

#include "objects\Spawner.h"
#include "objects\Floor.h"
#include "objects\Wall.h"
#include "objects\Exit.h"
#include "objects\Teleporter.h"
#include "objects\Potion.h"
#include "objects\Food.h"
#include "objects\Key.h"
#include "objects\Door.h"
#include "objects\Treasure.h"

#include "SpriteManager.h"
#include <cassert>

Object * ObjectLibrary::getObject(int type)
{
	NHTV::SpriteManager *sprite = NHTV::SpriteManager::GetInstance();
	glm::vec2 size(32.f);
	glm::vec2 origin(0.5f);
	glm::vec4 color(1.f);
	glm::vec4 uv(0.f,0.f,1.f,1.f);	

	const unsigned id = sprite->AddSprite("images/spriteSheet.png", size, origin, uv, color);
	
	switch (static_cast<Object::Type>(type))
	{
#pragma region Players and monsters
	case 1:
		return spawnGhost(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 2:
		return spawnDemon(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 3:
		return spawnGrunt(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 4:
		return spawnSorcerer(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 5:
		return spawnDarkness(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 6:
		return spawnLobber(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 7:
		return spawnThief(id, *m_map, *m_physicsEngine, m_monsterLevel);
		break;

	case 8:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Ghost, *m_map, *m_physicsEngine,  spawnGhost);
		break;

	case 9:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Demon, *m_map, *m_physicsEngine,  spawnDemon);
		break;

	case 10:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Grunt, *m_map, *m_physicsEngine,  spawnGrunt);
		break;

	case 11:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Sorcerer, *m_map, *m_physicsEngine,  spawnSorcerer);
		break;

	case 12:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Darkness, *m_map, *m_physicsEngine,  spawnDarkness);
		break;

	case 13:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Lobber, *m_map, *m_physicsEngine,  spawnLobber);
		break;

	case 14:
		return new Spawner(id, m_monsterLevel, Monster::MonsterType::Thief, *m_map, *m_physicsEngine,  spawnThief);
		break;

	case 15:
		switch (static_cast<Player::HeroType>(m_heroType))
		{
		case Player::HeroType::Warrior:
			return spawnWarrior(id, *m_map, *m_physicsEngine);
			break;

		case Player::HeroType::Valkyrie:
			return spawnValkyrie(id, *m_map, *m_physicsEngine);
			break;

		case Player::HeroType::Wizard:
			return spawnWizard(id, *m_map, *m_physicsEngine);
			break;

		case Player::HeroType::Elf:
			return spawnElf(id, *m_map, *m_physicsEngine);
			break;
		}
		break;

#pragma endregion

#pragma region Consumables

	case 16: // XX Food
		return new Food(id,Food::XX);
		break;

	case 17: // Skull Food
		return new Food(id, Food::Skull);
		break;

	case 18: // Chicken
		return new Food(id, Food::Chicken);
		break;

	case 19: // Hotdog
		return new Food(id, Food::Hotdog);
		break;

	case 20: // Miku
		return new Food(id, Food::Meat);
		break;

	case 21: // Key
		return new Key(id);
		break;

	case 22: // Potion
		return new Potion(id, Potion::Magic);
		break;

	case 23: // Movespeed
		return new Potion(id, Potion::MoveSpeed);
		break;

	case 24: // Defense
		return new Potion(id, Potion::Defense);
		break;

	case 25: // Magic power
		return new Potion(id, Potion::MagicPower);
		break;

	case 26: // Shot damage
		return new Potion(id, Potion::ShotPower);
		break;

	case 27: // Attack speed
		return new Potion(id, Potion::AttackSpeed);
		break;

	case 28: // Melee damage
		return new Potion(id, Potion::Melee);
		break;

	case 38:
		return new Treasure(id);
		break;

#pragma endregion

#pragma region Tiles

	case 29: // Teleporter tile
		return new Teleporter(id);

	case 30:	// Exit
		return new Exit(id,1, *m_map); 
		break;

	case 31:	// Exit 4
		return new Exit(id,4, *m_map); 
		break;

	case 32:	// Exit 8
		return new Exit(id,8, *m_map); 
		break;

	case 33:	// Wall
		return new Wall(id, false, *m_map);
		break;

	case 34:	// Breakable wall
		return new Wall(id, true, *m_map);
		break;

	case 35:	// Door
		return new Door(id,*m_map);
		break;

	case 36:	// Floor
		return new Floor(id, false, *m_map);
		break;

	case 37:	// Trap floor
		return new Floor(id, true, *m_map);
		break;
#pragma endregion
	}

	assert(false && "Invalid object id given");
	return nullptr;
}

void ObjectLibrary::setMonsterLevel(const unsigned level)
{
	m_monsterLevel = level;
}

void ObjectLibrary::setHeroClass(const int heroType)
{
	m_heroType = heroType;
}

void ObjectLibrary::initialize(Map * map, PhysicsEngine * physicsEngine)
{
	m_map = map;
	m_physicsEngine = physicsEngine;
	m_heroType = 0;
}
