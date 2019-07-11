#pragma once
#include "Object.h"
#include "Animation.h"

class Food : public Object
{
public:

	enum FoodType;
	Food(const unsigned id, const FoodType foodType);

	virtual void onCollide(Object *obj);

	enum FoodType
	{
		XX,
		Skull,
		Chicken,
		Hotdog,
		Meat
	};

private:

	FoodType type;
	Animation m_animation;
};