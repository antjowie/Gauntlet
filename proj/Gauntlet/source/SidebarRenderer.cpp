#include "SidebarRenderer.h"
#include "entities\Player.h"

#include "Text.h"

#include <string>

SidebarRenderer::SidebarRenderer():
	m_currentLevel(0)
{
}

void SidebarRenderer::draw() const
{
	const float xOffset = 180.f;
	float yOffset = 0;
	glm::vec4 color(1.f, 1.f, 1.f, 0.5f);

	// Draw current level
	yOffset = 160.f;
	Text::draw("Level", m_position.x + xOffset, m_position.y + yOffset,false, color);
	Text::draw(std::to_string(m_currentLevel), m_position.x + xOffset, m_position.y + yOffset - 30.f, false, color);

	// Draw health
	yOffset -= 60.f;
	Text::draw("Health", m_position.x + xOffset, m_position.y + yOffset, false, color);
	Text::draw(std::to_string(std::abs(m_player->getHealth())), m_position.x + xOffset, m_position.y + yOffset - 30.f, false, color);

	// Draw score
	yOffset -= 60.f;
	Text::draw("Score", m_position.x + xOffset, m_position.y + yOffset, false, color);
	Text::draw(std::to_string(m_player->getInventory()->getScore()), m_position.x + xOffset, m_position.y + yOffset - 30.f, false, color);

	// Draw keys
	yOffset -= 60.f;
	Text::draw("Keys", m_position.x + xOffset, m_position.y + yOffset, false, color);
	Text::draw(std::to_string(m_player->getInventory()->getKeyCount()), m_position.x + xOffset, m_position.y + yOffset - 30.f, false, color);

	// Draw potions
	yOffset -= 60.f;
	Text::draw("Potions", m_position.x + xOffset, m_position.y + yOffset, false, color);
	Text::draw(std::to_string(m_player->getInventory()->getPotionCount()), m_position.x + xOffset, m_position.y + yOffset - 30.f, false, color);
}

void SidebarRenderer::setCurrentLevel(unsigned currentLevel)
{
	m_currentLevel = currentLevel;
}

void SidebarRenderer::setPosition(const glm::vec2 & position)
{
	m_position = position;
}

void SidebarRenderer::setPlayer(Player *player)
{
	m_player = player;
}
