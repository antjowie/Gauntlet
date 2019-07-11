#include "TiledConverter.h"

#include <tinyxml2.h>
#include <string>
#include <algorithm>
#include <iostream>

TiledConverter::TiledConverter():
	m_currentLevel(0),
	m_levelCount(1)
{
}

void TiledConverter::importTiled(std::vector<std::vector<unsigned>> &map)
{
	map.clear();
	map.reserve(32);
	map.push_back({});
	map[0].reserve(32);

	// Load level
	char path[32];
	std::sprintf(path, "levels/%i.tmx", m_currentLevel);
	std::cout << "Loading path " << path << '\n';

	using namespace tinyxml2;

	// Load xml file
	XMLDocument doc;
	doc.LoadFile(path);
	std::cout << "Status: " << doc.ErrorName() << '\n';

	// File name not found, restart from level 0
	if (doc.Error())
	{
		m_currentLevel = 0;
		importTiled(map);
		return;
	}


	XMLElement *prop = doc.FirstChildElement("map")->FirstChildElement("properties");
	if (prop)
		m_monsterLevel = prop->FirstChildElement("property")->UnsignedAttribute("value", 1);
	else
		m_monsterLevel = 1;

	XMLElement *xmlMap= doc.FirstChildElement("map")->FirstChildElement("layer");
	std::string buffer = xmlMap->FirstChildElement("data")->GetText();

	// Load the data into the vector
	// Trash first newline
	std::string::iterator beg = buffer.begin() + 1, end = std::find(beg, buffer.end(), ',');

	// This section loads the id's into the map
	size_t y = 0;
	for (; end != buffer.end(); end = std::find(beg, buffer.end(), ','))
	{
		if (std::count(beg, end, '\n') == 1)
		{
			y++;
			map.push_back({});
			map[y].reserve(32);
		}

		int id = std::stoi(std::string(beg, end));
		map[y].push_back(id);

		beg = end + 1;
	}

	// Because we skipped over the last one (there is no comma at the end)
	// we have to push it once more 
	int id = std::stoi(std::string(beg, end));
	map[y].push_back(id);
}

void TiledConverter::increaseCurrentLevel(const unsigned offset)
{
	m_currentLevel += offset;
	m_levelCount += offset;
}

void TiledConverter::setCurrentLevel(const unsigned level)
{
	m_currentLevel = level;
	m_levelCount = level;
}

const unsigned TiledConverter::getCurrentLevel() const
{
	return m_levelCount;
}

const unsigned TiledConverter::getMonsterLevel() const
{
	return m_monsterLevel;
}
