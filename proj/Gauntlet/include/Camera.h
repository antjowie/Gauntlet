#pragma once
#include "glm/mat4x4.hpp"

class Camera
{
public:

	Camera(glm::mat4 & cameraMatrix, unsigned screenWidth, const unsigned screenHeight);

	void setPosition(const float x, const float y);
	void setZoom(const float factor);
	
	void getScreenSize(float &width, float &height);

private:
	
	glm::mat4 &m_cameraMatrix;

	float m_scale;

	const unsigned m_screenWidth;
	const unsigned m_screenHeight;
};