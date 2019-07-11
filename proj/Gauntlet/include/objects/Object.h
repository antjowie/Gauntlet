#pragma once

// The object class functions as the base object for every object.
// It is an interface for the map.
class Object
{
public:

	enum Type;
	Object(const unsigned id, const Type type, const bool isSolid, const unsigned width = 32, const unsigned heigth = 32);
	virtual ~Object();

	virtual void update(const float elapsedTime);
	virtual void updateAfterMove(const float elapsedTime);

	virtual void onCollide(Object *obj);

	// Object meta data
	const Type m_type;
	const unsigned m_id;
	const unsigned m_width;
	const unsigned m_height;

	// Flags
	virtual void setDead();
	bool getDead() const;

	bool m_isSolid;
	bool m_shouldDraw;

	enum Type
	{
		Hero,
		Enemy,
		Spawner,
		Floor,
		Wall,
		Food,
		Key,
		Potion,
		Gate,
		Treasure,
		Teleporter,
		Exit,
		Flyable,
		Count
	};

private:

	bool m_isDead;
};