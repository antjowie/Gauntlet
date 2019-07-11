//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	Class to manage sprites
//////////////////////////////////////////////////////////////////////////
#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <queue>
#include "BaseSprite.h"
#include "Utilities.h"

namespace NHTV
{
	//////////////////////////////////////////////////////////////////////////
	class SpriteManager
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		//This class is a static instance or a singleton class
		//////////////////////////////////////////////////////////////////////////
		static SpriteManager* CreateInstance();
		static SpriteManager* GetInstance();
		static void DestroyInstance();
		//////////////////////////////////////////////////////////////////////////
		void Draw(unsigned int a_uiShaderID);
		//////////////////////////////////////////////////////////////////////////
		unsigned int	AddSprite(const char* a_pTexture, glm::vec2& a_v2Size, glm::vec2& a_v2Origin, glm::vec4 a_vfUVCoords, glm::vec4 a_v4colour);
		unsigned int	DuplicateSprite(unsigned int a_uiSpriteID);
		void			MoveSprite(unsigned int a_uiSpriteID, int a_iXPos, int a_iYPos);
		void			MoveSprite(unsigned int a_uiSpriteID, const float* a_fVec);
		void			GetSpritePosition(unsigned int a_uiSpriteID, glm::vec2& a_v2Pos) const;
		void			SetSpriteMatrix(unsigned int a_uiSpriteID, const float* a_fm4);
		void			GetSpriteMatrix(unsigned int a_uiSpriteID, float* a_fm4);
		void			RotateSprite(unsigned int a_uiSpriteID, float a_iRotation);
		void			RemoveSprite(unsigned int a_uiSpriteID);
		void			SetSpriteUVCoordinates(unsigned int a_uiSpriteID, glm::vec2& a_minUVCoord, glm::vec2& a_maxUVCoord);
		void			GetSpriteUVCoordinates(unsigned int a_uiSpriteID, glm::vec2& a_minUVCoord, glm::vec2& a_maxUVCoord);
		void			MarkSpriteForDraw(unsigned int a_uiSpriteID);
		void			SetSpriteColour(unsigned int a_uiSpriteID, glm::vec4& a_v4Colour);
		void			GetSpriteColour(unsigned int a_uiSpriteID, glm::vec4& a_v4Colour);
		void			SetSpriteScale(unsigned int a_uiSpriteID, glm::vec2& a_v2SpriteScale);
		void			GetSpriteScale(unsigned int a_uiSpriteID, glm::vec2& a_v2SpriteScale);
		void			SetSpriteBlendMode(unsigned int a_uiSpriteID, const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor);
		void			GetSpriteBlendMode(unsigned int a_uiSpriteID, unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const;
	protected:

		SpriteManager();
		~SpriteManager();

	private:
		std::vector<BaseSprite*>				m_Sprites;
		std::queue<BaseSprite*>					m_pSpritesToDrawThisFrame;
		static SpriteManager*					m_pInstance;

		GLuint m_uiVAO;
		GLuint m_uiVBO;
		GLuint m_uiIBO;

		NHTV::Vertex aoVertices[4];
		NHTV::Vertex aoUpperLeftVertices[4];

	};

}//end namespace NHTV

 //////////////////////////////////////////////////////////////////////////
#endif //_SPRITEMANAGER_H_
 //////////////////////////////////////////////////////////////////////////