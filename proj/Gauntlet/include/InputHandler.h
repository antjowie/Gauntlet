#pragma once

namespace NHTV { class Application; }

// This is the input hanlder. It maps all the keyboard and mouse input to 
// boolean functions. The reason why this class is a singelton, is because
// I assume that only 1 keyboard and mouse is being used at a time.
//
// You could call this class a wrapper around the input handling function
// calls of the NHTV::Application class. The reason why I have an indirect 
// reference to them, is because I don't need to expose all the other functions
// that the class has.
class InputHandler
{
public:
	
	static InputHandler &getInstance();

	bool isKeyDown(unsigned int a_uiKey);
	void getMousePos(double& a_uiMousePosX, double& a_uiMousePosY);
	bool getMouseButtonDown(int a_iMouseButton);
	bool getMouseButtonReleased(int a_iMouseButton);

	NHTV::Application* m_app;

private:

	InputHandler();
};