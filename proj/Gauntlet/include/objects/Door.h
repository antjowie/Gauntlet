#pragma once
#include "AdjacentObject.h"

class Map;
class Door : public AdjacentTile
{
public:

	Door(unsigned id,Map &map);

	virtual void setDead() override final;
};