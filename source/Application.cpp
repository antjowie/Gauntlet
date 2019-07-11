#include "Application.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tinyxml.h"

#include "Utilities.h"
#include "SpriteManager.h"
#include "TextureManager.h"

namespace NHTV
{
	bool Application::create(int a_argc, char* a_argv[])
	{
#pragma region CreateGLFWWindow
		// Load the application data from our appData.xml
		if (!loadApplicationData())
		{
			std::cout << "Unable to load Application Data" << std::endl;
			return false;
		}
		// initialise glfw systems
		if (glfwInit() != GL_TRUE)
		{
			std::cout << "Unable to initialize GLFW" << std::endl;
			return false;
		}

		// create a windowed mode window and its OpenGL context
		for (unsigned int i = 0; i < m_windowHandles.size(); ++i)
		{
			glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
			ApplicationWindow* appWindow = &m_windowHandles[i];
			appWindow->glfwWindowHandle = glfwCreateWindow(appWindow->width, appWindow->height, appWindow->title, nullptr, nullptr);
			if (appWindow->glfwWindowHandle == nullptr)
			{
				std::cout << "Unable to create a GLFW Window" << std::endl;
				glfwTerminate();
				return false;
			}
			glfwSetWindowPos(appWindow->glfwWindowHandle, appWindow->xPos, appWindow->yPos);
			glfwShowWindow(appWindow->glfwWindowHandle);
		}

		m_window = m_windowHandles[0].glfwWindowHandle;

		int major = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
		int minor = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
		int revision = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_REVISION);
		std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;

		// make the window's context current
		glfwMakeContextCurrent(m_window);

		// initialise glew systems to wrangle GL extensions
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Unable to initialize GLEW" << std::endl;
			glfwTerminate();
			return false;
		}
#pragma endregion

		//Create some of the components we require
		SpriteManager::CreateInstance();
		CTextureManager::CreateInstance();

		m_pLineRenderer = new LineRenderer();
		//Set up the camera, load and link default shaders and set up view space
		// setup camera and view matrices
		m_CameraMatrix = glm::mat4(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 1.f, 1.f);
		int iScreenWidth = 0, iScreenHeight = 0;
		glfwGetWindowSize(m_window, &iScreenWidth, &iScreenHeight);
		m_ProjectionMatrix = glm::ortho(0.f, (float)iScreenWidth, 0.f, (float)iScreenHeight, 0.01f, 1000.f);
		m_ViewMatrix = glm::inverse(m_CameraMatrix);

		// load shader
		const char* aszInputs[] = { "Position", "Colour", "UV" };
		const char* aszOutputs[] = { "vUV", "vColour" };

		m_ShaderID = NHTV::Utility::loadShaderFromFile(3, aszInputs, 2, aszOutputs, "./shaders/default.vsh", "./shaders/default.fsh");
		
		// set matrix uniforms within the shaders
		GLint ProjectionID = glGetUniformLocation(m_ShaderID, "Projection");
		GLint ViewID = glGetUniformLocation(m_ShaderID, "View");
		
		glUniformMatrix4fv(ProjectionID, 1, false, glm::value_ptr(m_ProjectionMatrix));
		glUniformMatrix4fv(ViewID, 1, false, glm::value_ptr(m_ViewMatrix));

		// set the texture to use slot 0 in the shader
		GLuint texUniformID = glGetUniformLocation(m_ShaderID, "diffuseTexture");
		glUniform1i(texUniformID, 0);

		// set clear colour
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// start our timer
		NHTV::Utility::resetTimer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_fDeltaTime = NHTV::Utility::tickTimer();


		//Now call our overridden onCreate function to set up application specifics
		bool result = onCreate(a_argc, a_argv);
		if (result == false)
		{
			std::cout << "Application onCreate failure, terminating application" << std::endl;
			glfwTerminate();
		}
		return result;
	}

	void Application::run()
	{
		//Utility::resetTimer();
		m_running = true;
		do
		{
			float fDeltaTime = NHTV::Utility::tickTimer();
			double mX; double mY;
			GetMousePos(mX, mY);

			onUpdate(fDeltaTime);

			onDraw();

			// set active shader
			glUseProgram(m_ShaderID);
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			// convert camera's world matrix to a view matrix
			m_ViewMatrix = glm::inverse(m_CameraMatrix);
			// set current transforms in the shader
			GLuint ViewID = glGetUniformLocation(m_ShaderID, "View");
			glUniformMatrix4fv(ViewID, 1, false, glm::value_ptr(m_ViewMatrix));
			//call to draw sprites
			SpriteManager::GetInstance()->Draw(m_ShaderID);
			//switch blending draw the font
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//draw any lines that we have in the scene
			m_pLineRenderer->draw(m_ViewMatrix, m_ProjectionMatrix);
			// clear all lines from last frame
			m_pLineRenderer->clear();
			// bring the back-buffer to the monitor
			glfwSwapBuffers(m_window);
			glfwPollEvents();

		} while (m_running == true && glfwWindowShouldClose(m_window) == 0);

		onDestroy();
	}

	bool Application::loadApplicationData()
	{
		FILE* fh = fopen("appData.xml", "rb");
		if (fh != NULL)
		{
			TiXmlDocument doc("appData.xml");
			doc.LoadFile();

			TiXmlNode *xmlNodeAppData = doc.FirstChild("appData");
			if (!xmlNodeAppData)
			{
				std::cout << "Could not find 'appData' node in XML!\n" << std::endl;
				return false;
			}

			TiXmlElement *xmlNodeTitle = xmlNodeAppData->FirstChildElement("title");
			if (!xmlNodeTitle)
			{
				std::cout << "Could not find 'title' node in XML!\n" << std::endl;
				return false;
			}
			strcpy(m_ApplicationTitle, xmlNodeTitle->GetText());
			std::cout << m_ApplicationTitle << std::endl;

			TiXmlNode *xmlNodeWindows = xmlNodeAppData->FirstChild("windows");
			if (!xmlNodeWindows)
			{
				std::cout << "Could not find 'windows' node in AppData" << std::endl;
				return false;
			}

			TiXmlNode* window = xmlNodeWindows->FirstChild("window");
			if (!window)
			{
				std::cout << "Could not find individual 'window' node in AppData" << std::endl;
				return false;
			}
			while (window)
			{
				TiXmlElement *element = window->ToElement();

				ApplicationWindow appWindow;

				const char *title = element->Attribute("title");
				const char *width = element->Attribute("width");
				const char *height = element->Attribute("height");
				const char *xPos = element->Attribute("xPos");
				const char *yPos = element->Attribute("yPos");
				if (!title || !width || !height || !xPos || !yPos)
				{
					std::cout << "Error loading AppData: window element from appData does not contain all required details" << std::endl;
					return false;
				}
				strcpy(appWindow.title, title);
				appWindow.width = atoi(width);
				appWindow.height = atoi(height);
				appWindow.xPos = atoi(xPos);
				appWindow.yPos = atoi(yPos);
				appWindow.glfwWindowHandle = nullptr;

				m_windowHandles.push_back(appWindow);

				window = window->NextSibling("window");
			}
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//Gets the screen size of the default window
	//////////////////////////////////////////////////////////////////////////
	void Application::GetScreenSize( int& a_iWidth, int& a_iHeight) const
	{
		glfwGetWindowSize(m_window, &a_iWidth, &a_iHeight);
	}
	//////////////////////////////////////////////////////////////////////////
	// Does what it says on the tin clears the render buffer or screen
	//////////////////////////////////////////////////////////////////////////
	void Application::clearScreen()
	{
		m_pLineRenderer->clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	//////////////////////////////////////////////////////////////////////////////
	// Changes the background colour of the render window
	//////////////////////////////////////////////////////////////////////////////
	void Application::setBackgroundColor(SColour a_bgColor)
	{
		glm::vec4 bgColor = glm::vec4(a_bgColor.argb.colours.r / 255.f, 
			a_bgColor.argb.colours.g / 255.f, 
			a_bgColor.argb.colours.b / 255.f, 
			a_bgColor.argb.colours.a / 255.f);

		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	}

#pragma region Sprite creation and mainpulation functionality
	//////////////////////////////////////////////////////////////////////////
	// Sprite creation, copy and destruction
	//////////////////////////////////////////////////////////////////////////
	unsigned int Application::CreateSprite(const char* a_pTextureName, float a_fWidth, float a_fHeight, bool a_bDrawFromCenter, SColour a_sColour /* = SColour( 0xFF, 0xFF, 0xFF, 0xFF )*/)
	{
		glm::vec2 v2Size = glm::vec2(a_fWidth, a_fHeight);
		glm::vec2 v2Origin = a_bDrawFromCenter ? glm::vec2(0.5f, 0.5f) : glm::vec2(0.f, 1.f);
		glm::vec4 uvCoords = glm::vec4(0.f, 0.f, 1.f, 1.f);
		return CreateSprite(a_pTextureName, glm::value_ptr(v2Size), glm::value_ptr(v2Origin), glm::value_ptr(uvCoords),	a_sColour);
	}

	unsigned int Application::CreateSprite(const char* a_pTextureName, const float* a_fv2Size, const float* a_fv2Origin, const float* a_fv4UVCoords /*= nullptr*/, SColour a_sColour)
	{
		//Set up the UV Coordinates
		glm::vec4 v4UVCoords = (a_fv4UVCoords) ? 
			glm::vec4(a_fv4UVCoords[0], a_fv4UVCoords[1], a_fv4UVCoords[2], a_fv4UVCoords[3]) :
			glm::vec4(0.f, 0.f, 1.f, 1.f);
		//Set up the colour
		glm::vec4 v4Color = glm::vec4(a_sColour.argb.colours.r / 255.f,
			a_sColour.argb.colours.g / 255.f,
			a_sColour.argb.colours.b / 255.f,
			a_sColour.argb.colours.a / 255.f);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 v2Size = glm::vec2(a_fv2Size[0], a_fv2Size[1]);
			glm::vec2 v2Origin = glm::vec2(a_fv2Origin[0], a_fv2Origin[1]);
			return pSpriteManager->AddSprite(a_pTextureName, v2Size, v2Origin, v4UVCoords, v4Color);
		}
		return 0;
	}

	unsigned int Application::DuplicateSprite(unsigned int a_uiSpriteID)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			return pSpriteManager->DuplicateSprite(a_uiSpriteID);
		}
		return 0;
	}

	void Application::DestroySprite(unsigned int a_uiSpriteID)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->RemoveSprite(a_uiSpriteID);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//Drawing
	//////////////////////////////////////////////////////////////////////////
	void Application::DrawSprite(unsigned int a_uiSpriteID)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->MarkSpriteForDraw(a_uiSpriteID);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//Movement, Rotation and Scale
	//////////////////////////////////////////////////////////////////////////
	void Application::MoveSprite(unsigned int a_uiSpriteID, float a_fXPos, float a_fYPos)
	{
		glm::vec2 v2Pos = glm::vec2(a_fXPos, a_fYPos);
		MoveSprite(a_uiSpriteID, glm::value_ptr(v2Pos));
	}

	void Application::MoveSprite(unsigned int a_uiSpriteID, const float* a_vFloatVec)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->MoveSprite(a_uiSpriteID, a_vFloatVec);
		}
	}

	void Application::GetSpritePosition(unsigned int a_uiSpriteID, float& a_xPos, float& a_YPos) const
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 v2Pos;
			pSpriteManager->GetSpritePosition(a_uiSpriteID, v2Pos);
			a_xPos = v2Pos.x; a_YPos = v2Pos.y;
		}
	}

	void Application::SetSpriteMatrix(unsigned int a_uiSpriteID, const float* a_fm4)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->SetSpriteMatrix(a_uiSpriteID, a_fm4);
		}
	}

	void Application::GetSpriteMatrix(unsigned int a_uiSpriteID, float* a_fm4) const
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->GetSpriteMatrix(a_uiSpriteID, a_fm4);
		}
	}

	void Application::RotateSprite(unsigned int a_uiSpriteID, float a_fRotation)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->RotateSprite(a_uiSpriteID, a_fRotation);
		}
	}
	
	void Application::SetSpriteScale(unsigned int a_uiSpriteID, const float& a_fSpriteWidth, const float& a_fSpriteHeight)
	{
		glm::vec2 v2Scale = glm::vec2(a_fSpriteWidth, a_fSpriteHeight);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->SetSpriteScale(a_uiSpriteID, v2Scale);
		}
	}

	void Application::GetSpriteScale(unsigned int a_uiSpriteID, float& a_fSpriteWidth, float& a_fSpriteHeight) const
	{
		glm::vec2 v2Scale = glm::vec2(a_fSpriteWidth, a_fSpriteHeight);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->GetSpriteScale(a_uiSpriteID, v2Scale);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//UV Coordinates
	//////////////////////////////////////////////////////////////////////////
	void Application::SetSpriteUVCoordinates(unsigned int a_uiSpriteID, float a_minUCoord, float a_minVCoord, float a_maxUCoord, float a_maxVCoord)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 minUV(a_minUCoord, a_minVCoord);
			glm::vec2 maxUV(a_maxUCoord, a_maxVCoord);
			pSpriteManager->SetSpriteUVCoordinates(a_uiSpriteID, minUV, maxUV);
		}
	}

	void Application::SetSpriteUVCoordinates(unsigned int a_uiSpriteID, const float* a_fUVVec4)
	{
		glm::vec4 v4SpriteCoordinates = glm::vec4(a_fUVVec4[0], a_fUVVec4[1], a_fUVVec4[2], a_fUVVec4[3]);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 minUV(v4SpriteCoordinates.x, v4SpriteCoordinates.y);
			glm::vec2 maxUV(v4SpriteCoordinates.z, v4SpriteCoordinates.w);
			pSpriteManager->SetSpriteUVCoordinates(a_uiSpriteID, minUV, maxUV);
		}
	}

	void Application::GetSpriteUVCoordinates(unsigned int a_uiSpriteID, float& a_minUCoord, float& a_minVCoord, float& a_maxUCoord, float& a_maxVCoord) const
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 minUVCoord;
			glm::vec2 maxUVCoord;
			pSpriteManager->GetSpriteUVCoordinates(a_uiSpriteID, minUVCoord, maxUVCoord);

			a_minUCoord = minUVCoord.x; a_minVCoord = minUVCoord.y;
			a_maxUCoord = maxUVCoord.x; a_maxVCoord = maxUVCoord.y;
		}
	}

	void Application::GetSpriteUVCoordinates(unsigned int a_uiSpriteID, float* a_fUVVec4) const 
	{
		glm::vec4 v4SpriteCoordinates = glm::vec4(0.f, 0.f, 0.f, 0.f);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			glm::vec2 minUVCoord;
			glm::vec2 maxUVCoord;
			pSpriteManager->GetSpriteUVCoordinates(a_uiSpriteID, minUVCoord, maxUVCoord);

			v4SpriteCoordinates.x = minUVCoord.x; v4SpriteCoordinates.y = minUVCoord.y;
			v4SpriteCoordinates.z = maxUVCoord.x; v4SpriteCoordinates.w = maxUVCoord.y;
		}
		a_fUVVec4 = glm::value_ptr(v4SpriteCoordinates);
	}
	//////////////////////////////////////////////////////////////////////////
	//Color
	//////////////////////////////////////////////////////////////////////////
	void Application::SetSpriteColour(unsigned int a_uiSpriteID, const SColour& a_sColour)
	{
		glm::vec4 v4colour = glm::vec4(a_sColour.argb.colours.r / 255.f, a_sColour.argb.colours.g / 255.f, a_sColour.argb.colours.b / 255.f, a_sColour.argb.colours.a / 255.f);
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->SetSpriteColour(a_uiSpriteID, v4colour);
		}
	}
	void Application::GetSpriteColour(unsigned int a_uiSpriteID, SColour& a_sColour) const 
	{
		glm::vec4 v4colour;
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->GetSpriteColour(a_uiSpriteID, v4colour);
		}
		a_sColour.argb.colours.r = (char)(255.f * v4colour.x);
		a_sColour.argb.colours.g = (char)(255.f * v4colour.y);
		a_sColour.argb.colours.b = (char)(255.f * v4colour.z);
		a_sColour.argb.colours.a = (char)(255.f * v4colour.w);
	}
	//////////////////////////////////////////////////////////////////////////
	//Blend mode settings
	//////////////////////////////////////////////////////////////////////////
	void Application::SetSpriteBlendMode(unsigned int a_uiSpriteID, const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor)
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->SetSpriteBlendMode(a_uiSpriteID, a_uiSourceFactor, a_uiDestinationFactor);
		}
	}

	void Application::GetSpriteBlendMode(unsigned int a_uiSpriteID, unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const
	{
		SpriteManager* pSpriteManager = SpriteManager::GetInstance();
		if (pSpriteManager)
		{
			pSpriteManager->GetSpriteBlendMode(a_uiSpriteID, a_uiSourceFactor, a_uiDestinationFactor);
		}
	}
#pragma endregion

#pragma region Line Drawring Functionality
	//////////////////////////////////////////////////////////////////////////
	/// @name Basic Line Drawing Functionality
	//////////////////////////////////////////////////////////////////////////
	/**@{**/

	//////////////////////////////////////////////////////////////////////////
	void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY)
	{
		m_pLineRenderer->addLine(glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f), glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f));
	}

	//////////////////////////////////////////////////////////////////////////
	void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sColour)
	{
		glm::vec4 lineColor = glm::vec4(a_sColour.argb.colours.r / 255.f,
			a_sColour.argb.colours.g / 255.f,
			a_sColour.argb.colours.b / 255.f,
			a_sColour.argb.colours.a / 255.f);

		m_pLineRenderer->addLine(glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f), glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f), lineColor);
	}

	//////////////////////////////////////////////////////////////////////////
	void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sStartColour, SColour a_sEndColour)
	{
		glm::vec4 startColor = glm::vec4(a_sStartColour.argb.colours.r / 255.f,
			a_sStartColour.argb.colours.g / 255.f,
			a_sStartColour.argb.colours.b / 255.f,
			a_sStartColour.argb.colours.a / 255.f);

		glm::vec4 endColor = glm::vec4(a_sEndColour.argb.colours.r / 255.f,
			a_sEndColour.argb.colours.g / 255.f,
			a_sEndColour.argb.colours.b / 255.f,
			a_sEndColour.argb.colours.a / 255.f);

		m_pLineRenderer->addLine(glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f), glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f), startColor, endColor);
	}
	/**@}**/
#pragma endregion
#pragma region Input handling functionality
	//////////////////////////////////////////////////////////////////////////
	/// @name Input handling functionality
	//////////////////////////////////////////////////////////////////////////
	/**@{**/

	//////////////////////////////////////////////////////////////////////////
	bool Application::IsKeyDown(unsigned int a_uiKey)
	{
		return (glfwGetKey(m_window, a_uiKey) == GLFW_PRESS);
	}

	//////////////////////////////////////////////////////////////////////////
	void Application::GetMousePos(double& a_uiMousePosX, double& a_uiMousePosY)
	{
		glfwGetCursorPos(m_window, &a_uiMousePosX, &a_uiMousePosY);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Application::GetMouseButtonDown(int a_uiMouseButton)
	{
		return (glfwGetMouseButton(m_window, a_uiMouseButton) == GLFW_PRESS);
	}

	bool Application::GetMouseButtonReleased(int a_uiMouseButton)
	{
		return (glfwGetMouseButton(m_window, a_uiMouseButton) == GLFW_RELEASE);
	}
	/**@}**/
#pragma endregion
	float Application::GetDeltaTime()
	{
		return NHTV::Utility::getDeltaTime();
	}
	//////////////////////////////////////////////////////////////////////////////
	//Destructor for the Application
	//////////////////////////////////////////////////////////////////////////
	Application::~Application()
	{
		glDeleteShader(m_ShaderID);
		glfwTerminate();
	}
}//end namespace NHTV