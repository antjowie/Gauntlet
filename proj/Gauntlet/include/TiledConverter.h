#pragma once
#include <vector>

class TiledConverter
{
public:

	TiledConverter();

	void importTiled(std::vector<std::vector<unsigned>> &map);

	void increaseCurrentLevel(const unsigned offset);
	void setCurrentLevel(const unsigned level);

	const unsigned getCurrentLevel() const;
	const unsigned getMonsterLevel() const;

private:

	unsigned m_monsterLevel;
	// This count is the level file count
	unsigned m_currentLevel;

	// This count is displayed
	unsigned m_levelCount;
};