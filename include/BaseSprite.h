//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	This is a basic textured screen-space quad
//////////////////////////////////////////////////////////////////////////
#ifndef _AIE_BASE_SPRITE_H_
#define _AIE_BASE_SPRITE_H_

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Utilities.h"

namespace NHTV
{

	//////////////////////////////////////////////////////////////////////////
	class BaseSprite
	{
	public:
		//Constructors Destructors
		BaseSprite(const char* a_pTexture, float a_fWidth, float a_fHeight, const NHTV::Vertex* a_vertexData, glm::vec4 a_v4colour = glm::vec4(1.f, 1.f, 1.f, 1.f));
		BaseSprite(const char* a_pTexture, const glm::vec2& a_v2Size, const glm::vec2& a_v2Origin, const glm::vec4 a_fv4UVCoordinates = glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec4 a_v4colour = glm::vec4(1.f, 1.f, 1.f, 1.f));
		BaseSprite(const BaseSprite& pSprite);
		~BaseSprite();
		//Draw functionality
		void	Draw(unsigned int a_uiProgram);
		///////////////////////////////////////////////////////////////////////////
		// Fucntions to set and Get sprite attributes 
		///////////////////////////////////////////////////////////////////////////
		void	SetRotation(float a_iRotation);
		void	SetSpritecolour(const glm::vec4& a_v4colour);
		void	GetSpritecolour(glm::vec4& a_v4colour) const;

		void	SetPosition(int a_iX, int a_iY);
		void	SetPosition(const float* a_fVec);
		void	GetPosition(float& a_iX, float& a_iY) const;

		void	SetVertexData(NHTV::Vertex* a_vertexData);
		const	NHTV::Vertex* GetVertexData() const;

		void	SetSpriteMatrix(const float* a_fm4);
		void	GetSpriteMatrix(float* a_fm4) const;

		void	SetOrigin(const glm::vec2& a_v2Origin);
		void	GetOrigin(glm::vec2& a_v2Origin) const;

		void	SetSize(float a_iWidth, float a_iHeight);
		void	GetSize(float& a_iWidth, float& a_iHeight) const;

		void	SetSpriteID(unsigned int a_uiSpriteID);
		void	GetSpriteID(unsigned int& a_uiSpriteID) const;

		unsigned int GetTextureID() const { return m_uiTextureID; }

		void	SetUVCoords(const glm::vec2& a_minUVCoords, const glm::vec2& a_maxUVCoords);
		void	GetUVCoords(glm::vec2& a_minUVCoords, glm::vec2& a_maxUVCoords) const;

		void	SetBlendParameters(const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor);
		void	GetBlendParameters(unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const;
		///////////////////////////////////////////////////////////////////////////
	private:
		NHTV::Vertex	m_aoVertices[4];

		glm::mat4			m_m4Matrix;
		glm::vec4			m_v4Spritecolour;
		glm::vec3			m_v2Scale;
		glm::vec2			m_v2Origin;
		//\========================================================================
		//\ UV Coordinate Member Variables
		//\========================================================================
		glm::vec2			m_minUVCoords;
		glm::vec2			m_maxUVCoords;
		glm::vec2			m_uvScale;
		glm::vec2			m_uvOffset;

		unsigned int	m_uiTextureID;
		unsigned int	m_uSourceBlendMode;
		unsigned int	m_uDestinationBlendMode;

	};
	///////////////////////////////////////////////////////////////////////////
	//inline functions 
	// these are inlined to improve performance as they are called frequently
	///////////////////////////////////////////////////////////////////////////
	inline void BaseSprite::SetPosition(int a_iX, int a_iY)
	{
		m_m4Matrix[3] = glm::vec4((float)a_iX, (float)a_iY, 0.f, 1.f);
	}

	inline void BaseSprite::SetPosition(const float* a_fVec)
	{
		m_m4Matrix[3] = glm::vec4(a_fVec[0], a_fVec[1], 0.f, 1.f);
	}

	inline void BaseSprite::SetSpriteMatrix(const float* a_fm4)
	{
		memcpy(glm::value_ptr(m_m4Matrix), a_fm4, sizeof(float) * 16);
	}

	inline void BaseSprite::GetSpriteMatrix(float* a_fm4) const
	{
		memcpy(a_fm4, glm::value_ptr(m_m4Matrix), sizeof(float) * 16);
	}

	inline void BaseSprite::GetPosition(float& a_iX, float& a_iY) const
	{
		a_iX = m_m4Matrix[3].x;
		a_iY = m_m4Matrix[3].y;
	}

	inline void BaseSprite::SetSize(float a_iWidth, float a_iHeight)
	{
		m_v2Scale = glm::vec3(a_iWidth, a_iHeight, 1.f);
	}

	inline void BaseSprite::GetSize(float& a_iWidth, float& a_iHeight) const
	{
		a_iWidth = m_v2Scale.x;
		a_iHeight = m_v2Scale.y;
	}

	inline void	BaseSprite::SetVertexData(NHTV::Vertex* a_vertexData)
	{
		memcpy(&m_aoVertices, a_vertexData, sizeof(NHTV::Vertex) * 4);
	}

	inline const NHTV::Vertex* BaseSprite::GetVertexData() const
	{
		return static_cast<const NHTV::Vertex*>(m_aoVertices);
	}

}//namespace NHTV

 //////////////////////////////////////////////////////////////////////////
#endif //_AIE_BASE_SPRITE_H_
 //////////////////////////////////////////////////////////////////////////
