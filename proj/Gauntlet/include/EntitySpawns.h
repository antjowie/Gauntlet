#pragma once

class Map;
class PhysicsEngine;
class Player;
class Monster;

#pragma region Heros
Player * spawnWarrior(unsigned id, Map &map, PhysicsEngine &physics);

Player * spawnValkyrie(unsigned id, Map &map, PhysicsEngine &physics);

Player * spawnWizard(unsigned id, Map &map, PhysicsEngine &physics);

Player * spawnElf(unsigned id, Map &map, PhysicsEngine &physics);
#pragma endregion

#pragma region Monsters
Monster * spawnGhost(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnDemon(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnGrunt(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnSorcerer(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnDarkness(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnLobber(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);

Monster * spawnThief(unsigned id, Map &map, PhysicsEngine &physics, const unsigned level);
#pragma endregion