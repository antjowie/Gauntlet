#include "EntitySpawns.h"

#include "entities\Monster.h"
#include "entities\Player.h"

#include "fsms\PlayerFSM.h"
#include "fsms\MoveFSM.h"
#include "fsms\MoveShootFSM.h"

#include "commands\PlayerCommand.h"
#include "commands\MonsterCommand.h"

#include "objects\ProjectileSpawns.h"
#include "objects\Projectile.h"

#pragma region Heros
Player * spawnWarrior(unsigned id, Map &map, PhysicsEngine &physics)
{
	ProjectileLauncher * launcher = new ProjectileLauncher(Projectile::ProjectileType::Axe,spawnProjectile,map,physics,false);

	FSM *move = new PlayerFSM(new PlayerKeyboardMove(), new PlayerKeyboardShoot(launcher));
	Player * player = new Player(id, Player::HeroType::Warrior, 500, 3, 2, 1, 4, 
		2, 150.f, 0.2f, move, physics, map);

	launcher->setEntity(player);
	move->setEntityHandle(player);
	return player;
}

Player * spawnValkyrie(unsigned id, Map &map, PhysicsEngine &physics)
{
	ProjectileLauncher * launcher = new ProjectileLauncher(Projectile::ProjectileType::Sword, spawnProjectile, map, physics, false);

	FSM *move = new PlayerFSM(new PlayerKeyboardMove(), new PlayerKeyboardShoot(launcher));
	Player * player = new Player(id, Player::HeroType::Valkyrie, 500, 2, 2, 2, 5,
		2, 150.f, 0.2f, move, physics, map);

	launcher->setEntity(player);
	move->setEntityHandle(player);
	return player;
}

Player * spawnWizard(unsigned id, Map &map, PhysicsEngine &physics)
{
	ProjectileLauncher * launcher = new ProjectileLauncher(Projectile::ProjectileType::Fireball, spawnProjectile, map, physics, false);

	FSM *move = new PlayerFSM(new PlayerKeyboardMove(), new PlayerKeyboardShoot(launcher));
	Player * player = new Player(id, Player::HeroType::Wizard, 500, 1, 3, 5, 1,
		2, 150.f, 0.2f, move, physics, map);

	launcher->setEntity(player);
	move->setEntityHandle(player);
	return player;
}

Player * spawnElf(unsigned id, Map &map, PhysicsEngine &physics)
{
	ProjectileLauncher * launcher = new ProjectileLauncher(Projectile::ProjectileType::Arrow, spawnProjectile, map, physics, false);

	FSM *move = new PlayerFSM(new PlayerKeyboardMove(), new PlayerKeyboardShoot(launcher));
	Player * player = new Player(id, Player::HeroType::Elf, 500, 1, 4, 3, 2, 
		2, 150.f, 0.2f, move, physics, map);

	launcher->setEntity(player);
	move->setEntityHandle(player);
	return player;
}
#pragma endregion

#pragma region Monsters
Monster * spawnGhost(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	FSM *move = new MoveFSM(new MonsterWalk());

	Monster * monster = new Monster(id, Monster::MonsterType::Ghost, level, 3, 30, 3,
		0,0,0, move, physics, map,100);

	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnDemon(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	ProjectileLauncher *launcher = new ProjectileLauncher(Projectile::ProjectileType::Fireball, spawnProjectile, map, physics, true);

	FSM *move = new MoveShootFSM(new MonsterWalk(),new MonsterShoot(launcher));

	Monster * monster = new Monster(id, Monster::MonsterType::Demon, level, 5, 5, 2,
		2, 96.f, 1.f, move, physics, map, 200);

	launcher->setEntity(monster);
	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnGrunt(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	FSM *move = new MoveFSM(new MonsterWalk());
	Monster * monster = new Monster(id, Monster::MonsterType::Grunt, level, 10, 30, 2,
		0, 0, 0, move, physics, map, 200);

	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnSorcerer(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	ProjectileLauncher *launcher = new ProjectileLauncher(Projectile::ProjectileType::PurpleFireball,spawnProjectile,map,physics,true);

	FSM *move = new MoveShootFSM(new MonsterWalk(),new MonsterShoot(launcher));
	Monster * monster = new Monster(id, Monster::MonsterType::Sorcerer, level, 10, 30, 2, 
		2, 96.f, 0.2f, move, physics, map, 300);

	launcher->setEntity(monster);
	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnDarkness(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	FSM *move = new MoveFSM(new MonsterWalk());
	Monster * monster = new Monster(id, Monster::MonsterType::Darkness, level, 100, 30, 3, 
		0, 0, 0, move, physics, map, 300);

	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnLobber(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	ProjectileLauncher *launcher = new ProjectileLauncher(Projectile::ProjectileType::LobberBall, spawnProjectile, map, physics, true);

	FSM *move = new MoveShootFSM(new MonsterWalk(),new MonsterShoot(launcher));
	Monster * monster = new Monster(id, Monster::MonsterType::Lobber, level, 5, 30, 4, 
		3, 32.f, 1.f, move, physics, map, 300);

	launcher->setEntity(monster);
	move->setEntityHandle(monster);
	return  monster;
}

Monster * spawnThief(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level)
{
	FSM *move = new MoveFSM(new MonsterWalk());
	Monster * monster = new Monster(id, Monster::MonsterType::Thief, level, 1, 15, 8, 
		0, 0, 0, move, physics, map, 300);

	move->setEntityHandle(monster);
	return  monster;
}
#pragma endregion