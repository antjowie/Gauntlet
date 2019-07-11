#include "InputHandler.h"
#include "Application.h"

InputHandler &InputHandler::getInstance()
{
	static InputHandler instance;
	return instance;
}

bool InputHandler::isKeyDown(unsigned int a_uiKey)
{
	return (glfwGetKey(m_app->m_window, a_uiKey) == GLFW_PRESS);
}

void InputHandler::getMousePos(double& a_uiMousePosX, double& a_uiMousePosY)
{
	glfwGetCursorPos(m_app->m_window, &a_uiMousePosX, &a_uiMousePosY);
}

bool InputHandler::getMouseButtonDown(int a_uiMouseButton)
{
	return (glfwGetMouseButton(m_app->m_window, a_uiMouseButton) == GLFW_PRESS);
}

bool InputHandler::getMouseButtonReleased(int a_uiMouseButton)
{
	return (glfwGetMouseButton(m_app->m_window, a_uiMouseButton) == GLFW_RELEASE);
}

InputHandler::InputHandler()
{
}