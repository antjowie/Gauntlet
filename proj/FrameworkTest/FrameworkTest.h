#ifndef __Tutorial1_H_
#define __Tutorial1_H_

#include "Application.h"
#include <glm/glm.hpp>

// Derived application class that wraps up all globals neatly
class FrameworkTest : public NHTV::Application
{
public:

	FrameworkTest();
	virtual ~FrameworkTest();

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

private:
	int m_iScreenWidth;
	int m_iScreenHeight;

	unsigned int m_iPlayerSprite;
};

#endif // __Tutorial1_H_