#pragma once
#include "AdjacentObject.h"
#include "Animation.h"

class Map;
class Wall :public AdjacentTile
{
public:

	Wall(unsigned id, bool isBreakable, Map &map);

	virtual void onCollide(Object* obj) override final;

	void setBreakable(const bool isBreakable);
	bool getBreakable() const;

private:
	bool m_isBreakable;
};