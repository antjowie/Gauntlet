#pragma once
#include "Object.h"
#include "Animation.h"

class Key : public Object
{
public:

	Key(const unsigned id);
	virtual void onCollide(Object *obj) override final;

private:

	Animation m_animation;
};