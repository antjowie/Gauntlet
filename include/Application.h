//\====

#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Utilities.h"
#include "LineRenderer.h"
#include "Enumerations.h"

struct GLFWwindow;

namespace NHTV
{


#pragma region SColor structure to describe a colour
	/**
	* @brief A Structure to describe a colour value as an unsigned int, and as individual 8 bit components (char values)
	**/
	struct SColour
	{
		/// @brief Default constructor for a SColour object
		SColour() {}
		/// @brief Constructor for an SColour obejct that constructs the SColour object from an unsigned integer value
		/// @param a_iColor the colour that the SColour will be created to specify
		SColour(unsigned int a_iColour) { argb.colour = a_iColour; }
		/// @brief Constructor to create an SColor object from four char values RGBA
		/// @param r unsigned char to represent the red component of an SColour object
		/// @param g unsigned char to represent the red component of an SColour object
		/// @param b unsigned char to represent the red component of an SColour object
		/// @param a unsigned char to represent the red component of an SColour object
		SColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			argb.colours.r = r;
			argb.colours.g = g;
			argb.colours.b = b;
			argb.colours.a = a;
		}
		/// and unnamed union that causes SColours member variables to use the same area of memory.
		/// this means that @param colour and @param colours occupy the same space, a change to one is 
		/// reflected in the other.
		union
		{
			unsigned int colour;
			struct SColours
			{
				unsigned char a, r, g, b;
			} colours;
		}argb;

	};
#pragma endregion

	class Application
	{
	public:

		Application() : m_running(false) {}
		virtual ~Application();

		bool	create(int a_argc, char* a_argv[]);
		void	run();
		void	destroy()	{ m_running = false; }

		// InputHandler needs access to this
		GLFWwindow*		m_window;

	protected:
		//To be implemented in a child class of our application
		virtual bool	onCreate(int a_argc, char* a_argv[]) = 0;
		virtual void	onUpdate(float a_deltaTime) = 0;
		virtual void	onDraw() = 0;
		virtual void	onDestroy() = 0;

		bool			m_running;

		//Functionality that we need in our base application
		void			GetScreenSize(int& a_iWidth, int& a_iHeight) const;
		void			clearScreen();
		void			setBackgroundColor(SColour a_bgColor);

#pragma region Line Drawing functionality

		/// @brief Function for drawing a simple line
		/// @param a_iStartX the starting X Position for the Line
		/// @param a_iStartY the starting Y Position for the line
		/// @param a_iEndX the end X Position for the line
		/// @param a_iEndY the end Y Position for the line
		void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY);
		/// @brief Function for drawing a line with a specific uniform colour
		/// @param a_iStartX the starting X Position for the Line
		/// @param a_iStartY the starting Y Position for the line
		/// @param a_iEndX the end X Position for the line
		/// @param a_iEndY the end Y Position for the line
		/// @param a_sColour a Vec4 representing the color for the line segment
		void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sColour);
		/// @brief Function for drawing a line that starts as one colour and finishes as another colour.
		/// @param a_iStartX the starting X Position for the Line
		/// @param a_iStartY the starting Y Position for the line
		/// @param a_iEndX the end X Position for the line
		/// @param a_iEndY the end Y Position for the line
		/// @param a_sStartColour a Vec4 denoting the colour of the starting point of the line segment 
		/// @param a_sEndColour a Vec4 denoting the colour of the end point of the line segment 
		void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sStartColour, SColour a_sEndColour);

#pragma endregion
#pragma region Input handling functionality
		/// @brief Function returns weather a key is currently being pressed
		/// @param a_uiKey the unique identifier for the key, these are kept in an enum referenced from GLFW
		/// @return boolean value to represent wheather a key is held down or not
		bool			IsKeyDown(unsigned int a_uiKey);
		/// @brief Function to get the position of the mouse, relative to the display window. Value is returned via function parameters passed in as reference arguments
		///			the returned values are given in pixels from the windows origin.
		/// @param a_uiMousePosX the reference argument that is to be given the value for the mouse cursors X position
		/// @param a_uiMousePosY the reference argument that is to be given the value for the mouse cursors Y position
		void			GetMousePos(double& a_uiMousePosX, double& a_uiMousePosY);
		/// @brief Function to test if a mouse button is being pressed. 
		/// @param a_iMouseButton and integer identifier to identify the button to test for
		/// @return boolean value indicating if button is currently being triggered
		bool			GetMouseButtonDown(int a_iMouseButton);
		/// @brief Function to Test to see if the mouse button has been released
		/// @param a_iMoustButton an integer identifier to identify the button to test for release
		/// @return boolean value to indicate weather button has just been released.
		bool			GetMouseButtonReleased(int a_iMouseButton);
#pragma endregion
#pragma region Sprite creation and mainpulation functionality
		/// Function to create a Sprite
		/// @param a_pTextureName - the name of the texture that the sprite image resides in
		/// @param a_fWidth - the width that the sprite object is to be draw on the screen, this width is in pixels
		/// @param a_fHeight - the height that the sprite object is to be draw on the screen, this height is in pixels
		/// @param a_bDrawFromCenter - weather or not the sprite is to be drawn from the center position, defaults to true
		/// @param a_v4color - the colour tint that the sprite is to be given, defaults to white.
		/// @return the ID for the sprite, this is an unsigned integer.
		///
		unsigned int	CreateSprite(const char* a_pTextureName, float a_fWidth, float a_fHeight, bool a_bDrawFromCenter = true, SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF) );
		/// Function to create a Sprite
		/// @param a_pTextureName - the name of the texture that the sprite image resides in
		/// @param a_fv2Size - the width and height of the sprite in Pixels as a float vector
		/// @param a_fv2Origin - The origin position for the sprite matrix, this dictates where the sprite will be drawn from (Top Left, Top Right etc)
		/// @param a_v4UVCoords - The UV Coordinates for the sprite as a float 4, components  1 & 2 correspond to origin point, components 3 & 4 correspond to end point.
		/// @param a_v4color - the colour tint that the sprite is to be given, defaults to white.
		/// @return the ID for the sprite, this is an unsigned integer.
		///
		unsigned int	CreateSprite(const char* a_pTextureName, const float* a_fv2Size, const float* a_fv2Origin, const float* a_fv4UVCoords = nullptr, SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));
		/// Function to Duplicate an existing Sprite
		/// @param a_uiSpriteID This is ID of the sprite that is to be duplicated
		/// @return Returns a new sprite ID.
		unsigned int	DuplicateSprite(unsigned int a_uiSpriteID);
		/// This function is used to free up the memory associated with the Sprite ID passed in. After this function is called the sprite can no longer be drawn as it ceases to exist
		/// @param a_uiSpriteID the ID of the sprite to be destroyed.
		void			DestroySprite(unsigned int a_uiSpriteID);
		/// This funciton moves a Sprite
		/// @param a_uiSpriteID the ID of the sprite to be moved
		/// @param a_fXPos the X co-ordinate that the sprite is to be moved to in screenspace
		/// @param a_fYPos the Y co-ordinate that the sprite is to be moved to in screenspace
		void			MoveSprite(unsigned int a_uiSpriteID, float a_fXPos, float a_fYPos);
		/// This funciton moves a Sprite
		/// @param a_uiSpriteID the ID of the sprite to be moved
		/// @param a_fVec a float vector that contains two values (X & Y) to relocate the sprite to
		void			MoveSprite(unsigned int a_uiSpriteID, const float* a_fVec);
		/// @brief This function gets the x y post of the Sprite
		/// @param a_uiSpriteID the unique identifier for the sprite
		/// @param a_fXPos the X co-ordinate that the sprite is at
		/// @param a_fYPos the Y co-ordinate that the sprite is at
		void			GetSpritePosition(unsigned int a_uiSpriteID, float& a_xPos, float& a_YPos) const;
		/// @brief This function sets the 4x4 matrix for the sprite
		/// @param a_uiSpriteID the unique identifier for the sprite
		/// @param a_fm4 a float pointer indicating the start of a float array containing 16 values representing a 4 x 4 matrix
		void			SetSpriteMatrix(unsigned int a_uiSpriteID, const float* a_fm4);
		/// @brief This function gets the 4x4 matrix for the sprite
		/// @param a_uiSpriteID the unique identifier for the sprite
		/// @param a_fm4 a float pointer indicating the start of a float array that is to be used to hold the 16 float values representing a 4 x 4 matrix
		void			GetSpriteMatrix(unsigned int a_uiSpriteID, float* a_fm4) const ;
		/// @brief Function to draw a sprite to the screen
		/// @param a_uiSpriteID the unique identifier for the Sprite
		void			DrawSprite(unsigned int a_uiSpriteID);
		/// @brief Function to rotate a sprite by a_fRotation number of degrees, positive rotation is in a clockwise direction
		/// @param a_uiSpriteID the unique ID for the sprite
		/// @param a_fRotation the amount in degrees to rotate the sprite by, this value is non-cumulative E.G passing in a rotation value of 1 Degree every frame will not incrementally rotate the sprite each frame.
		void			RotateSprite(unsigned int a_uiSpriteID, float a_fRotation);
		/// @brief This function sets the sprites UV coordinates
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_minUCoord this represents the starting point for the UV coordinates on the U Texture axis
		/// @param a_minVCoord this represents the starting point for the UV coordinates on the V Texture axis
		/// @param a_maxUCoord this represents the end point for the UV coordinates on the U Texture axis
		/// @param a_maxVCoord this represents the end point for the UV coordinates on the V Texture axis
		void			SetSpriteUVCoordinates(unsigned int a_uiSpriteID, float a_minUCoord, float a_minVCoord, float a_maxUCoord, float a_maxVCoord);
		/// @brief This function sets the sprites UV coordinates
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_v4UVVoordinates a float vector4 representing the minimum and maximum UV Coordinates for the sprite, the X and Y components of the Vec4 correspond to the minimum UV coordinates
		///			the Z & W components correspond to the maximum UV Coordinates
		void			SetSpriteUVCoordinates(unsigned int a_uiSpriteID, const float* a_fUVVec4);
		/// @brief This functions retrieves the Sprites current UV coordinates
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_minUCoord this represents the starting point for the UV coordinates on the U Texture axis
		/// @param a_minVCoord this represents the starting point for the UV coordinates on the V Texture axis
		/// @param a_maxUCoord this represents the end point for the UV coordinates on the U Texture axis
		/// @param a_maxVCoord this represents the end point for the UV coordinates on the V Texture axis
		void			GetSpriteUVCoordinates(unsigned int a_uiSpriteID, float& a_minUCoord, float& a_minVCoord, float& a_maxUCoord, float& a_maxVCoord) const;
		/// @brief This functions retrieves the Sprites current UV coordinates
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_v4UVVoordinates a float vector4 representing the minimum and maximum UV Coordinates for the sprite, the X and Y components of the Vec4 correspond to the minimum UV coordinates
		///			the Z & W components correspond to the maximum UV Coordinates
		void			GetSpriteUVCoordinates(unsigned int a_uiSpriteID, float* a_fUVVec4 ) const;
		/// @brief Used to set the colour for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_sColour the colour that the sprite is to be tinted with, this colour is passed in a vec4 with xyzw values mapping to rgba values
		void			SetSpriteColour(unsigned int a_uiSpriteID, const SColour& a_sColour);
		/// @brief Used to get the current colour for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_sColour the colour that the sprite is tinted with, this colour is accessed via a vec4 with xyzw values mapping to rgba values
		void			GetSpriteColour(unsigned int a_uiSpriteID, SColour& a_sColour) const;
		/// @brief Used to set the current scale for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions that the sprite is to be scaled by
		void			SetSpriteScale(unsigned int a_uiSpriteID, const float& a_fSpriteWidth, const float& a_fSpriteHeight);
		/// @brief Used to get the current scale for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions that the sprite is to currently scaled by
		void			GetSpriteScale(unsigned int a_uiSpriteID, float& a_fSpriteWidth, float& a_fSpriteHeight) const;
		/// @brief Used to set the current blend mode for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_uiSourceFactor the blend factor for the source image
		/// @param a_uiDestinationFactor the blend mode that is to be used by the destination image.
		void			SetSpriteBlendMode(unsigned int a_uiSpriteID, const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor);
		/// @brief Used to get the current blend mode for the sprite
		/// @param a_uiSpriteID the unique identifier for the given sprite
		/// @param a_uiSourceFactor the current blend factor for the source image
		/// @param a_uiDestinationFactor the current blend mode that is used by the destination image.
		void			GetSpriteBlendMode(unsigned int a_uiSpriteID, unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const;

#pragma endregion

		//////////////////////////////////////////////////////////////////////////
		/// @brief GetDeltaTime() a function to get the amount of time that has passed since the last time the CPU was polled
		///	@return the amount of time that has passed in seconds since the CPU was last polled.
		//////////////////////////////////////////////////////////////////////////
		float			GetDeltaTime();

		glm::mat4 m_CameraMatrix;

	private:
		//For the naming and creation of our application window
		bool			loadApplicationData();
		char			m_ApplicationTitle[256];

		typedef struct sApplicationWindow
		{
			char		title[256];
			int			width;
			int			height;
			int			xPos;
			int			yPos;
			GLFWwindow*	glfwWindowHandle;
		}ApplicationWindow;

		std::vector<ApplicationWindow>	m_windowHandles;

		//Rendering specific member variables that any child classes should not need to concern themselves
		//with
		GLuint m_ShaderID = 0;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		LineRenderer* m_pLineRenderer;

		float m_fDeltaTime;

	};


}//end namespace NHTV

#endif // __APPLICATION_H_

//////////////////////////////////////////////////////////////////////////
#define APPLICATION_ENTRY(appClass)									\
	int main(int a_argc, char* a_argv[])							\
	{																\
		NHTV::Application* app = new appClass();					\
		if (app->create(a_argc,a_argv) == true)						\
		{															\
			app->run();												\
			app->destroy();											\
		}															\
		else														\
		{															\
			/*NHTV::logError("Failed to setup application!\n");*/	\
		}															\
		delete app;													\
		return 0;													\
	}
//////////////////////////////////////////////////////////////////////////