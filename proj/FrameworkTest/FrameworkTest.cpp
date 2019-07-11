#include "Application.h"
#include "FrameworkTest.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

APPLICATION_ENTRY(FrameworkTest);
#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720

FrameworkTest::FrameworkTest()
{

}

FrameworkTest::~FrameworkTest()
{

}

bool FrameworkTest::onCreate(int a_argc, char* a_argv[])
{
	setBackgroundColor(NHTV::SColour(0x00, 0x00, 0x00, 0xFF));
	//\Now lets create the sprite for our players cannon. That's right in space invaders we control a cannon
	//\So lets create that with an appropriate variable name and move it to a suitable location (say the middle of our screen)
	m_iPlayerSprite = CreateSprite("./images/cannon.png", 64, 32, true);

	GetScreenSize(m_iScreenWidth, m_iScreenHeight);
	MoveSprite(m_iPlayerSprite, m_iScreenWidth * 0.5f, m_iScreenHeight * 0.5f);

	return true;
}

void FrameworkTest::onUpdate(float a_deltaTime)
{
	float xPos = 0; float yPos = 0;
	GetSpritePosition(m_iPlayerSprite, xPos, yPos);
	if (IsKeyDown(NHTV::KEY_RIGHT))
	{
		MoveSprite(m_iPlayerSprite, xPos + 0.1f, yPos);
	}
	if (IsKeyDown(NHTV::KEY_LEFT))
	{
		MoveSprite(m_iPlayerSprite, xPos - 0.1f, yPos);
	}
	// quit our application when escape is pressed
	if ( IsKeyDown(NHTV::KEY_ESCAPE) )
		destroy();
}

void FrameworkTest::onDraw()
{
	clearScreen();

	float fDeltaT = GetDeltaTime();

	DrawSprite(m_iPlayerSprite);

	//DrawString("SCORE < 1 >", m_iScreenWidth * 0.025f, m_iScreenHeight - 2, 0.5f);
	//DrawString("HI-SCORE", m_iScreenWidth * 0.4f, m_iScreenHeight - 2, 0.5f);

	/*DrawString("SCORE < 2 >", m_iScreenWidth * 0.725f, m_iScreenHeight - 2, 0.5f);
	DrawString("CREDIT", m_iScreenWidth * 0.65f, 38);
*/
	DrawLine(0, 40, m_iScreenWidth, 40, NHTV::SColour(0x00, 0xFC, 0x00, 0xFF));
	std::string timeFrame = "DeltaTime: ";
	timeFrame.append(std::to_string(fDeltaT));
//	DrawString(timeFrame.c_str(), 10, 720, 0.75f);


}

void FrameworkTest::onDestroy()
{
	
}