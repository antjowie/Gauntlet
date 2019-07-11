#include "Camera.h"
#include "glm/gtx/transform.hpp"

Camera::Camera(glm::mat4 & cameraMatrix, unsigned screenWidth, const unsigned screenHeight):
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_cameraMatrix(cameraMatrix),
	m_scale(1)
{
}

void Camera::setPosition(const float x, const float y)
{
	// The camera matrix should be responsible for which part of the game is visible. 
	// We first move the matrix to a position. Because we don't save the old position, we use an identity
	// matrix to always reset the position to a center position. 
	m_cameraMatrix = glm::translate(glm::mat4(1.0f),
		{ x - (m_screenWidth * 0.5f / m_scale),y - (m_screenHeight * 0.5f / m_scale),1.f });
	m_cameraMatrix = glm::scale(m_cameraMatrix, { 1.f/m_scale,1.f/m_scale,1.f });
}

void Camera::setZoom(const float factor)
{
	m_scale = factor;
}

void Camera::getScreenSize(float & width, float & height)
{
	width = m_screenWidth / m_scale;
	height = m_screenHeight / m_scale;
}
