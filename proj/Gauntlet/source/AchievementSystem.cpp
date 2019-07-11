#include "AchievementSystem.h"
#include "Text.h"

#include "SpriteManager.h"

AchievementSystem::AchievementSystem():
	m_displayTime(0),
	m_drawPosition(0)
{
	m_dimID = NHTV::SpriteManager::GetInstance()->AddSprite("", glm::vec2(1280.f, 720.f), glm::vec2(640.f,360.f), glm::vec4(0, 0, 1, 1), glm::vec4(0.75f));

	m_achievements.push_back(Achievement(Object::Type::Hero, Object::Type::Enemy, "Monsters hurt"));
	m_achievements.push_back(Achievement(Object::Type::Hero, Object::Type::Key, "Keys open doors"));
	m_achievements.push_back(Achievement(Object::Type::Hero, Object::Type::Potion, "Potions do nothing"));
	m_achievements.push_back(Achievement(Object::Type::Hero, Object::Type::Food, "Food heals health"));
	m_achievements.push_back(Achievement(Object::Type::Hero, Object::Type::Treasure, "Treasure gives score"));
	
	m_achievements.push_back(Achievement(Object::Type::Flyable, Object::Type::Potion, "Potions can be destroyed"));
	m_achievements.push_back(Achievement(Object::Type::Flyable, Object::Type::Food, "Food can be destroyed"));
	m_achievements.push_back(Achievement(Object::Type::Flyable, Object::Type::Spawner, "Generator can be destoyed"));
}

AchievementSystem::~AchievementSystem()
{
	NHTV::SpriteManager::GetInstance()->RemoveSprite(m_dimID);
}

void AchievementSystem::onEvent(Object * obj1, Object * obj2)
{
	for(std::vector<Achievement>::iterator iter = m_achievements.begin(); iter != m_achievements.end(); iter++)
		if ((iter->type1 == obj1->m_type && iter->type2 == obj2->m_type) ||
			(iter->type2 == obj1->m_type && iter->type1 == obj2->m_type))
		{
			if (!iter->isCalled)
			{
				m_textToDraw = iter->string;
				iter->isCalled = true;
				m_displayTime = 4.f;
			}
			return;
		}
}

bool AchievementSystem::updateElapsedTime(const float elapsedTime)
{
	m_displayTime -= elapsedTime;
	if (m_displayTime > 0.f)
		return true;
	else
		m_displayTime = 0.f;
	return false;
}

void AchievementSystem::setTextDrawPosition(const glm::vec2 & pos)
{
	m_drawPosition = pos;
}

void AchievementSystem::resetAchievements()
{
	for (std::vector<Achievement>::iterator iter = m_achievements.begin(); iter != m_achievements.end(); iter++)
		iter->isCalled = false;
}

void AchievementSystem::draw()
{
	if (m_displayTime > 0.f)
	{
		NHTV::SpriteManager::GetInstance()->MoveSprite(m_dimID,glm::value_ptr(glm::vec2(m_drawPosition.x,m_drawPosition.y)));
		NHTV::SpriteManager::GetInstance()->MarkSpriteForDraw(m_dimID);

		Text::draw(m_textToDraw, m_drawPosition.x, m_drawPosition.y,true);
	}
}

AchievementSystem::Achievement::Achievement(const Object::Type type1, const Object::Type type2, const std::string string):
	type1(type1),type2(type2),string(string),isCalled(false)
{
}
