#pragma once
#include "Object.h"
#include "Animation.h"

class Map;
class Exit : public Object
{
public:

	Exit(unsigned id, unsigned level, Map &map);

	virtual void onCollide(Object *obj) override final;

private:
	
	const unsigned m_level;

	Animation m_animation;
	Map &m_map;
};