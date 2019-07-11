#pragma once
#include "objects\Object.h"
#include "Animation.h"

class FSM;
class PhysicsEngine;
class Map;

// This class holds the base entity stat values
// needed for melee combat
class Entity : public Object
{
public:
	// ShootSpeed in pixel per second
	// ShootRate in shots per second
	Entity(const unsigned id, const Object::Type type, const int health,
		const unsigned meleePower, const unsigned moveSpeed, 
		const unsigned shootPower, const float shootSpeed, const float shootRate,
		FSM *fsm, PhysicsEngine & physics, Map & map, const unsigned yAnimIndex,
		const unsigned width = 28, const unsigned height = 28);
	virtual ~Entity() override;

	virtual void update(const float elapsedTime) override;
	virtual void updateAfterMove(const float elapsedTime) override final;
	virtual void onCollide(Object* obj) override;

	virtual void		loseHealth(const unsigned amount, bool blink); 
	virtual	void		gainHealth(const unsigned amount);
	
	virtual int			getHealth() const;
	virtual float		getSpeed() const;
	virtual unsigned	getMeleeDamage();

	virtual unsigned	getShootPower() const;
	virtual float		getShootSpeed() const;
	virtual float		getShootRate() const; // Shoot per second

	void setLookDirection(const glm::vec2 &direction);

	void setInvisibility(const bool isInvisible); 
	bool getInvisibility() const;

	Map &m_map;
	PhysicsEngine &m_physics;

protected:

	float		m_meleeTimeline;
	const float m_meleeCooldown;

private:

	EntityAnimation m_animation;
	FSM *m_fsm;

	int			m_health;
	unsigned	m_moveSpeed;
	unsigned	m_meleePower;
	bool		m_isInvisible;

	unsigned	m_shootPower;
	float		m_shootSpeed;
	float		m_shootRate;

	float	m_blinkTimeline;

	glm::vec2	m_moveDirection;
};