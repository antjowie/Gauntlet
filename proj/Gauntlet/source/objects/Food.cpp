#include "objects\Food.h"

#include "SpriteManager.h"

Food::Food(const unsigned id, const FoodType foodType):
	Object(id,Object::Type::Food,true),
	m_animation(foodType,foodType + 1,7,8,0.f)
{
	float u1, v1, u2, v2;
	m_animation.getCurrentFrame(u1, v1, u2, v2);
	NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
}

void Food::onCollide(Object * obj)
{
	switch (obj->m_type)
	{
	case Object::Type::Hero:
	case Object::Type::Flyable:
		setDead();
		break;
	}
}
