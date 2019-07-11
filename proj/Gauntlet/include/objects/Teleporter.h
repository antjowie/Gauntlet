#pragma once
#include "Object.h"
#include "Animation.h"

class Teleporter : public Object
{
public:

	Teleporter(const unsigned id);

	virtual void update(const float elapsedTime) override final;

private:

	Animation m_animation;
};