#pragma once
#include "glm\vec2.hpp"
#include "entities\Entity.h"

#include "Map.h"
#include "PhysicsEngine.h"

class Command
{
public:

	virtual void excecute(Entity *entity, const float elapsedTime) = 0;
};