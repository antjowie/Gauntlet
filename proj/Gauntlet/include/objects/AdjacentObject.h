#pragma once
#include "Object.h"
#include "Animation.h"
#include "Map.h"

// This is to avoid writing the same code inside door and wall
class AdjacentObject : public Object
{
public:

	AdjacentObject(const unsigned id, Map &map, const Object::Type type,const bool isSolid);
	virtual void setCorrectFrame() = 0;

protected:

	// Object array takes a 2D 3x3 array
	void getSurroundingTiles(const Object::Type type, bool objectArray[3][3]);

	Map &m_map;
	Animation m_animation;
};

// Tiles have a different texture mapping then floors. So we derive this function so that we don't need to know the layout.
class AdjacentTile : public AdjacentObject
{
public:

	AdjacentTile(const unsigned id, Map &map, const Object::Type type, const bool isSolid);

	virtual void setCorrectFrame() override final;
};

class AdjacentFloor : public AdjacentObject
{
public:

	AdjacentFloor(const unsigned id, Map &map, const Object::Type type, const bool isSolid);

	virtual void setCorrectFrame() override final;
};