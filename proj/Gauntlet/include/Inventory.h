#pragma once
#include "objects\Potion.h"

#include <vector>

class Inventory
{
public:

	Inventory();

	// If user has a key, return true and pop one
	bool popKey();
	void addKey();
	unsigned getKeyCount() const;

	// If user has a potion, return true and pop one
	bool popPotion();
	void addPotion();
	unsigned getPotionCount() const;

	unsigned getScore() const;
	void addScore(const unsigned score);

	bool hasUpgradePotion(Potion::PotionType potionType);

private:
	
	std::vector<bool> m_upgradePotions;
	unsigned m_score;

	int m_keys;
	int m_potions;
};