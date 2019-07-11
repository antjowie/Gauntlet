#pragma once
#include "glm/vec2.hpp"

// This class returns the correct uv values needed for animation management
class Animation
{
public:

	// x- and yMin/MaxAnim are the amount of rows we want to iterate over
	// MinAnim is including the frame
	// MaxAnim is up to the frame
	// Range [0;Frames]
	Animation(
		const unsigned xMinAnim, const unsigned xMaxAnim, 
		const unsigned yMinAnim, const unsigned yMaxAnim, 
		const float frameDuration);

	virtual void update(const float elapsedTime);

	//--------------------------------------

	virtual void getCurrentFrame(float &minU, float &minV, float &maxU, float &maxV) const;
	unsigned getCurrentFrameCount() const;
	unsigned getTotalFrameCount() const;

	//--------------------------------------

	void setCurrentFrame(const unsigned frame);
	void setXAnim(const unsigned xMinAnim, const unsigned xMaxAnim);
	void setYAnim(const unsigned yMinAnim, const unsigned yMaxAnim);


protected:

	// The amount of frames the image contains
	const unsigned m_xFrames, m_yFrames;
	const unsigned m_pxWidth, m_pxHeight;
	// The amount of rows we want to iterate over
	// Range: [0,xFrames]
	unsigned m_xMinAnim, m_xMaxAnim;
	// The amount of columns we want to iterate over
	// Range: [0,yFrames]
	unsigned m_yMinAnim, m_yMaxAnim;

	// These vars change over time
	float m_frameCount;
	float m_elapsedTime;
	unsigned m_currrentFrame;
	const float m_frameDuration;
};

class EntityAnimation : public Animation
{
public:

	// x- and yMin/MaxAnim are the amount of rows we want to iterate over
	// MinAnim is including the frame
	// MaxAnim is up to the frame
	// Range [0;Frames]
	EntityAnimation(
		const unsigned xMinAnim, const unsigned xMaxAnim,
		const unsigned yMinAnim, const unsigned yMaxAnim,
		const float frameDuration);

	virtual void update(const float elapsedTime) override final;
	virtual void getCurrentFrame(float &minU, float &minV, float &maxU, float &maxV) const override final;

	void setMovement(glm::vec2 &movement);

private:

	glm::vec2 m_movement;
	float m_offset;
};