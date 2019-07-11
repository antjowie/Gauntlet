#pragma once
#include "Entity.h"
#include "Inventory.h"

class Player : public Entity
{
public:
	
	enum HeroType;
	Player(const unsigned id, const HeroType heroType, const int health,
		const unsigned meleePower, const unsigned moveSpeed, 
		const unsigned magic, const unsigned armor, 
		const unsigned shootPower, const float shootSpeed, const float shootRate, 
		FSM *fsm, PhysicsEngine & physicsEngine, Map &map,
		const unsigned width = 28.f, const unsigned height = 28.f);

	virtual void onCollide(Object *obj) override final;
	virtual void update(const float elapsedTime) override final;

	Inventory *getInventory();
	
	enum HeroType
	{
		Warrior,
		Valkyrie,
		Wizard,
		Elf
	};

private:

	float m_dotTimeline;

	Inventory *m_inventory;
};