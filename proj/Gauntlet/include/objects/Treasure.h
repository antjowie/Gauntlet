#pragma once
#include "Object.h"
#include "Animation.h"

class Treasure : public Object
{
public:
	
	Treasure(const unsigned id);

	// Temp
	virtual void update(const float elapsedTime);

private:

	void switchFrame();

	Animation m_animation;
	// This is used to get a random frame from the
	// animation when hit
	int m_animIndex;


	// Temp
	float m_elapsed;
};