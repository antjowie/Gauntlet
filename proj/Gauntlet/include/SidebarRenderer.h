#pragma once
#include "glm\vec2.hpp"

class Player;
class SidebarRenderer
{
public:

	SidebarRenderer();

	void draw() const;
	void setPosition(const glm::vec2 &position);

	void setPlayer(Player *player);
	void setCurrentLevel(unsigned currentLevel);

private:

	Player *m_player;
	glm::vec2 m_position;

	unsigned m_currentLevel;
};