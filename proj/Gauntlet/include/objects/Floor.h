#pragma once
#include "AdjacentObject.h"
#include "Animation.h"

class Map;
class Floor : public AdjacentFloor
{
public:

	Floor(const unsigned id, const bool isTrap, Map &map);

	void setTrap(bool isTrap);
	bool getTrap() const;

private:

	bool m_isTrap;
};