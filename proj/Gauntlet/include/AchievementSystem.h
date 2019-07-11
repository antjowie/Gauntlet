#pragma once
#include "PhysicsEngine.h"
#include "objects/Object.h"

class AchievementSystem : public Listener
{
public:

	AchievementSystem();
	~AchievementSystem();

	virtual void onEvent(Object *obj1, Object *obj2) override final;

	bool updateElapsedTime(const float elapsedTime);
	void setTextDrawPosition(const glm::vec2 &pos);

	void resetAchievements();

	void draw();

private:

	float m_displayTime;

	glm::vec2 m_drawPosition;
	std::string m_textToDraw;
	unsigned m_dimID;

	struct Achievement
	{
		Achievement(const Object::Type type1, const Object::Type type2, const std::string string);

		const Object::Type type1;
		const Object::Type type2;
		const std::string string;
		bool isCalled;
	};

	std::vector<Achievement> m_achievements;
};