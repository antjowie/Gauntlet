#pragma once
#include "Object.h"
#include "Animation.h"

class Potion : public Object
{
public:

	enum PotionType;
	Potion(const unsigned id, const PotionType PotionType);

	enum PotionType
	{
		Magic,
		Melee,
		Defense,
		MagicPower,
		ShotPower,
		AttackSpeed,
		MoveSpeed,
		Count
	};

private:

	PotionType m_potionType;
	Animation m_animation;
};