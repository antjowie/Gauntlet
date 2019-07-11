#include "Inventory.h"

Inventory::Inventory()
{
	m_upgradePotions.resize(Potion::Type::Count,false);
}

bool Inventory::popKey()
{
	if (m_keys > 0)
	{
		m_keys--;
		return true;
	}
	return false;
}

void Inventory::addKey()
{
	m_keys++;
}

unsigned Inventory::getKeyCount() const
{
	return m_keys;
}

bool Inventory::popPotion()
{
	if (m_potions > 0)
	{
		m_potions--;
		return true;
	}
	return false;
}

void Inventory::addPotion()
{
	m_potions++;
}

unsigned Inventory::getPotionCount() const
{
	return m_potions;
}

unsigned Inventory::getScore() const
{
	return m_score;
}

void Inventory::addScore(const unsigned score)
{
	m_score += score;
	if (m_score > 99999)
		m_score = 99999;
}

bool Inventory::hasUpgradePotion(Potion::PotionType potionType)
{
	return m_upgradePotions[potionType];
}