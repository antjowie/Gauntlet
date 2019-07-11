#pragma once
#include "Object.h"
#include "Animation.h"

class Explosion : public Object
{
public:
	
	enum ExplosionType;
	Explosion(glm::vec2 pos, const ExplosionType type);
	virtual ~Explosion();

	virtual void update(const float elapsedTime);

	enum ExplosionType
	{
		Small,
		Large
	};

private:

	Animation m_animation;
};