#include "Animation.h"

#include "Utilities.h"

#include <iostream>

Animation::Animation(const unsigned xMinAnim, const unsigned xMaxAnim, const unsigned yMinAnim, const unsigned yMaxAnim, const float frameDuration):
	m_xFrames(32), m_yFrames(19),
	m_pxWidth(1024),m_pxHeight(608),
	m_xMinAnim(xMinAnim),m_xMaxAnim(xMaxAnim),
	m_yMinAnim(yMinAnim),m_yMaxAnim(yMaxAnim),

	m_frameDuration(frameDuration),
	m_elapsedTime(0),
	m_currrentFrame(0),
	m_frameCount(float(m_xMaxAnim - m_xMinAnim))
{
}

void Animation::update(const float elapsedTime)
{
	if (m_frameDuration == 0.f)
		return;

	m_elapsedTime += elapsedTime;
	
	// Move the frame every time a value is overwritten
	while (m_elapsedTime > m_frameDuration)
	{
		m_elapsedTime -= m_frameDuration;
		m_currrentFrame += 1;
	
		// Wrap if too large
		if (m_currrentFrame >= m_frameCount)
			m_currrentFrame = 0;
	}
}

void Animation::getCurrentFrame(float & minU, float & minV, float & maxU, float & maxV) const
{
	// Because of the way UV coordinates work in OpenGL. We need to count 0 with the tiles.
	// Here, every tile is assumed to be 32 x 32 in size, but it should be 31 x 31.
	
	// 128 - 159 for a tile
	minV = ((float)m_yMinAnim * 32.f	   + 1.f) / ((float)m_pxHeight - 1.f);
	maxV = ((float)(m_yMinAnim + 1) * 32.f - 1.f) / ((float)m_pxHeight - 1.f);
	
	minU = ((float)(m_xMinAnim + m_currrentFrame + 0) * 32.f + 1.f) / ((float)m_pxWidth - 1.f);
	maxU = ((float)(m_xMinAnim + m_currrentFrame + 1) * 32.f - 1.f) / ((float)m_pxWidth - 1.f);
}

unsigned Animation::getCurrentFrameCount() const
{
	return m_currrentFrame;
}

unsigned Animation::getTotalFrameCount() const
{
	return m_xMaxAnim-m_xMinAnim;
}

void Animation::setCurrentFrame(const unsigned frame)
{
	m_currrentFrame = frame;
}

void Animation::setXAnim(const unsigned xMinAnim, const unsigned xMaxAnim)
{
	if (m_xMinAnim > m_xMaxAnim)
		return;

	m_xMinAnim = xMinAnim;
	m_xMaxAnim = xMaxAnim;
	m_frameCount = (float)xMaxAnim - (float)xMinAnim;
	
	if (m_currrentFrame > m_frameCount)
		m_currrentFrame = (unsigned)m_frameCount;
}

void Animation::setYAnim(const unsigned yMinAnim, const unsigned yMaxAnim)
{
	if (m_yMinAnim > m_yMaxAnim)
		return;

	m_yMinAnim = yMinAnim;
	m_yMaxAnim = yMaxAnim;
}

EntityAnimation::EntityAnimation(const unsigned xMinAnim, const unsigned xMaxAnim, const unsigned yMinAnim, const unsigned yMaxAnim, const float frameDuration):
	Animation(xMinAnim,xMaxAnim,yMinAnim,yMaxAnim,frameDuration),
	m_offset(0)
{
}

void EntityAnimation::update(const float elapsedTime)
{
	// We calculate the direction offset here
	if (m_movement == glm::vec2(0.f));
	else if (m_movement.y > 0 && m_movement.x == 0)
		m_offset = 0;
	else if (m_movement.y > 0 && m_movement.x > 0)
		m_offset = 1;
	else if (m_movement.y == 0 && m_movement.x > 0)
		m_offset = 2;
	else if (m_movement.y < 0 && m_movement.x > 0)
		m_offset = 3;
	else if (m_movement.y < 0 && m_movement.x == 0)
		m_offset = 4;
	else if (m_movement.y < 0 && m_movement.x < 0)
		m_offset = 5;
	else if (m_movement.y == 0 && m_movement.x < 0)
		m_offset = 6;
	else if (m_movement.y > 0 && m_movement.x < 0)
		m_offset = 7;

	// Reset the frame if player stopped moving
	if (m_movement == glm::vec2(0))
		setCurrentFrame(0);
	else
		Animation::update(elapsedTime);
}

void EntityAnimation::getCurrentFrame(float & minU, float & minV, float & maxU, float & maxV) const
{
	Animation::getCurrentFrame(minU, minV, maxU, maxV);
	unsigned index = getCurrentFrameCount();
	minU = (index * 8.f + m_offset) / 32.f;
	maxU = (index * 8.f + m_offset + 1.f) / 32.f;
}

void EntityAnimation::setMovement(glm::vec2 & movement)
{
	m_movement = movement;
}
