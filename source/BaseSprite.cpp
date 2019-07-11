//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	This is a basic textured screen-space quad
//////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BaseSprite.h"
#include "Utilities.h"
#include "TextureManager.h"

namespace NHTV
{
	//////////////////////////////////////////////////////////////////////////
	BaseSprite::BaseSprite(const char* a_pTexture, float a_fWidth, float a_fHeight, const NHTV::Vertex* a_vertexData, glm::vec4 a_v4colour)
	{
		memcpy(&m_aoVertices, a_vertexData, sizeof(NHTV::Vertex) * 4);
		m_m4Matrix = glm::mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		SetSize(a_fWidth, a_fHeight);
		m_v4Spritecolour = a_v4colour;
		m_uiTextureID = 0;
		m_minUVCoords = glm::vec2(0.f, 0.f);
		m_maxUVCoords = glm::vec2(1.f, 1.f);
		m_uvScale = glm::vec2(1.f, 1.f);
		m_uvOffset = glm::vec2(0.f, 0.f);
		m_uSourceBlendMode = GL_SRC_ALPHA;
		m_uDestinationBlendMode = GL_ONE_MINUS_SRC_ALPHA;

		if (CTextureManager::GetInstance())
		{
			m_uiTextureID = (a_pTexture != nullptr) ? CTextureManager::GetInstance()->LoadTexture(a_pTexture) : 0;
		}
	}

	BaseSprite::BaseSprite(const char* a_pTexture, const glm::vec2& a_v2Size, const glm::vec2& a_fOrigin, const glm::vec4 a_fv4UVCoordinates, glm::vec4 a_v4colour)
	{
		m_m4Matrix = glm::mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		m_v2Scale = glm::vec3(a_v2Size.x, a_v2Size.y, 1.f);
		SetOrigin(a_fOrigin);
		m_v4Spritecolour = a_v4colour;
		m_uiTextureID = 0;
		SetUVCoords(glm::vec2(a_fv4UVCoordinates.x, a_fv4UVCoordinates.y), glm::vec2(a_fv4UVCoordinates.z, a_fv4UVCoordinates.w));

		m_uSourceBlendMode = GL_SRC_ALPHA;
		m_uDestinationBlendMode = GL_ONE_MINUS_SRC_ALPHA;

		if (CTextureManager::GetInstance())
		{
			m_uiTextureID = (a_pTexture != nullptr) ? CTextureManager::GetInstance()->LoadTexture(a_pTexture) : 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	BaseSprite::BaseSprite(const BaseSprite& pSprite)
	{
		m_m4Matrix = pSprite.m_m4Matrix;
		m_v2Scale = pSprite.m_v2Scale;
		m_v4Spritecolour = pSprite.m_v4Spritecolour;
		m_uiTextureID = pSprite.m_uiTextureID;
		m_minUVCoords = pSprite.m_minUVCoords;
		m_maxUVCoords = pSprite.m_maxUVCoords;
		m_uvScale = pSprite.m_uvScale;
		m_uvOffset = pSprite.m_uvOffset;
		m_v2Origin = pSprite.m_v2Origin;

		memcpy(&m_aoVertices, pSprite.m_aoVertices, sizeof(NHTV::Vertex) * 4);
		m_uSourceBlendMode = pSprite.m_uSourceBlendMode;;
		m_uDestinationBlendMode = pSprite.m_uDestinationBlendMode;

		if (CTextureManager::GetInstance())
		{
			CTextureManager::GetInstance()->IncrementTextureReference(m_uiTextureID);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	BaseSprite::~BaseSprite()
	{
		if (CTextureManager::GetInstance())
		{
			CTextureManager::GetInstance()->RemoveTexture(m_uiTextureID);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void BaseSprite::Draw(unsigned int a_uiProgram)
	{
		glBlendFunc(m_uSourceBlendMode, m_uDestinationBlendMode);
		GLuint ModelID = glGetUniformLocation(a_uiProgram, "Model");
		glm::mat4 m4Scale;
		m4Scale = glm::scale(m4Scale, m_v2Scale);
		m4Scale = m_m4Matrix * m4Scale;

		glUniformMatrix4fv(ModelID, 1, false, glm::value_ptr(m4Scale));
		GLint rendercolour = glGetUniformLocation(a_uiProgram, "tintColor");
		glUniform4fv(rendercolour, 1, glm::value_ptr(m_v4Spritecolour));

		GLint  uvOffset = glGetUniformLocation(a_uiProgram, "uvOffset");
		glUniform2fv(uvOffset, 1, glm::value_ptr(m_uvOffset));

		GLint  uvScale = glGetUniformLocation(a_uiProgram, "uvScale");
		glUniform2fv(uvScale, 1, glm::value_ptr(m_uvScale));
		// set active texture, bind the crate quad's buffers and draw it
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
	}
	//////////////////////////////////////////////////////////////////////////
	// Set the origin point for the matrix
	// - Note: a_v2Origin should be a value between 0 and 1 for both x and y
	//   as this is a ratio position
	//////////////////////////////////////////////////////////////////////////
	void BaseSprite::SetOrigin(const glm::vec2& a_v2Origin)
	{
		m_v2Origin = a_v2Origin;
		if (m_v2Origin.x > 1.f || m_v2Origin.y > 1.f)
		{
			m_v2Origin = m_v2Origin / m_v2Scale.xy;
		}
		glm::vec2 v2TopLeft = glm::vec2(0.f, 1.f) - m_v2Origin;
		glm::vec2 v2TopRight = glm::vec2(1.f, 1.f) - m_v2Origin;
		glm::vec2 v2BottomLeft = glm::vec2(0.f, 0.f) - m_v2Origin;
		glm::vec2 v2BottomRight = glm::vec2(1.f, 0.f) - m_v2Origin;

		m_aoVertices[0].position = glm::vec4(v2BottomLeft.x, v2BottomLeft.y, 0.f, 1.f);
		m_aoVertices[0].colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		m_aoVertices[0].texCoord = glm::vec2(0.f, 0.f);
		m_aoVertices[1].position = glm::vec4(v2BottomRight.x, v2BottomRight.y, 0.f, 1.f);
		m_aoVertices[1].colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		m_aoVertices[1].texCoord = glm::vec2(1.f, 0.f);

		m_aoVertices[2].position = glm::vec4(v2TopLeft.x, v2TopLeft.y, 0.f, 1.f);
		m_aoVertices[2].colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		m_aoVertices[2].texCoord = glm::vec2(0.f, 1.f);

		m_aoVertices[3].position = glm::vec4(v2TopRight.x, v2TopRight.y, 0.f, 1.f);
		m_aoVertices[3].colour = glm::vec4(1.f, 1.f, 1.f, 1.f);
		m_aoVertices[3].texCoord = glm::vec2(1.f, 1.f);
	}

	void BaseSprite::GetOrigin(glm::vec2& a_v2Origin) const
	{
		a_v2Origin = m_v2Origin;
	}
	//////////////////////////////////////////////////////////////////////////
	void BaseSprite::SetRotation(float a_iRotation)
	{
		float fRadians = a_iRotation * DEG2RAD;
		m_m4Matrix = glm::rotate(m_m4Matrix, fRadians, glm::vec3(0.f, 0.f, 1.f));
	}
	///////////////////////////////////////////////////////////////////////////
	void	BaseSprite::SetSpritecolour(const glm::vec4& a_v4colour)
	{
		m_v4Spritecolour = a_v4colour;
	}

	void	BaseSprite::GetSpritecolour(glm::vec4& a_v4colour) const
	{
		a_v4colour = m_v4Spritecolour;
	}
	///////////////////////////////////////////////////////////////////////////
	void	BaseSprite::SetUVCoords(const glm::vec2& a_minUVCoords, const glm::vec2& a_maxUVCoords)
	{
		m_minUVCoords = a_minUVCoords;
		m_maxUVCoords = a_maxUVCoords;

		m_uvScale = m_maxUVCoords - m_minUVCoords;
		m_uvOffset = m_minUVCoords;

	}

	void	BaseSprite::GetUVCoords(glm::vec2& a_minUVCoords, glm::vec2& a_maxUVCoords) const
	{
		a_minUVCoords = m_minUVCoords;
		a_maxUVCoords = m_maxUVCoords;
	}

	void	BaseSprite::SetBlendParameters(const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor)
	{
		m_uSourceBlendMode = a_uiSourceFactor;
		m_uDestinationBlendMode = a_uiDestinationFactor;
	}

	void	BaseSprite::GetBlendParameters(unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const
	{
		a_uiSourceFactor = m_uSourceBlendMode;
		a_uiDestinationFactor = m_uDestinationBlendMode;
	}

} // namespace NHTV

