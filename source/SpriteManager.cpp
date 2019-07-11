//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	Class to manage sprites
//////////////////////////////////////////////////////////////////////////
#include "SpriteManager.h"
#include "Utilities.h"

namespace NHTV
{
	SpriteManager* SpriteManager::m_pInstance = 0;
	//////////////////////////////////////////////////////////////////////////
	SpriteManager* SpriteManager::CreateInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new SpriteManager();
		}
		return m_pInstance;
	}
	//////////////////////////////////////////////////////////////////////////
	SpriteManager* SpriteManager::GetInstance()
	{
		return m_pInstance;
	}
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	SpriteManager::SpriteManager()
	{
		unsigned int auiIndex[6] = {
			0,1,2,
			1,3,2
		};

		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);
		glGenBuffers(1, &m_uiVBO);
		glGenBuffers(1, &m_uiIBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(NHTV::Vertex), nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), auiIndex, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(NHTV::Vertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(NHTV::Vertex), ((char*)0) + 16);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(NHTV::Vertex), ((char*)0) + 32);
		
		glBindVertexArray(0);

	}
	//////////////////////////////////////////////////////////////////////////
	SpriteManager::~SpriteManager()
	{
		glDeleteVertexArrays(1, &m_uiVAO);
		glDeleteBuffers(1, &m_uiVBO);
		glDeleteBuffers(1, &m_uiIBO);
	}
	//////////////////////////////////////////////////////////////////////////
	//Draw Sprite
	// This goes through a list of sprites that have been told to draw and 
	// renders each one out, this list is a copy of sprites that are loaded
	// and each draw call gets emptied out
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::Draw(unsigned int a_uiShaderID)
	{

		while (!m_pSpritesToDrawThisFrame.empty())
		{
			BaseSprite* pBaseSprite = m_pSpritesToDrawThisFrame.front();
			if (pBaseSprite != NULL)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(NHTV::Vertex), pBaseSprite->GetVertexData());
				
				glBindVertexArray(m_uiVAO);
				pBaseSprite->Draw(a_uiShaderID);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				delete pBaseSprite;
			}
			m_pSpritesToDrawThisFrame.pop();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Adds a sprite to the sprite manager, 
	//////////////////////////////////////////////////////////////////////////
	unsigned int SpriteManager::AddSprite(const char* a_pTexture, glm::vec2& a_v2Size, glm::vec2& a_v2Origin, glm::vec4 a_vfUVCoords, glm::vec4 a_v4colour)
	{
		BaseSprite* pBasicSprite = new BaseSprite(a_pTexture, a_v2Size, a_v2Origin, a_vfUVCoords, a_v4colour);
		if (m_Sprites.size() > 0)
		{
			std::vector< BaseSprite* >::iterator sprite_iter = m_Sprites.begin();
			for (unsigned int i = 0; sprite_iter != m_Sprites.end(); ++sprite_iter, ++i)
			{
				if ((*sprite_iter) == NULL)
				{
					(*sprite_iter) = pBasicSprite;
					return i;
				}
			}
		}
		m_Sprites.push_back(pBasicSprite);
		return ((unsigned int)m_Sprites.size() - 1);
	}
	//////////////////////////////////////////////////////////////////////////
	// duplicates a sprite that already exists in the sprite manager
	//////////////////////////////////////////////////////////////////////////
	unsigned int SpriteManager::DuplicateSprite(unsigned int a_uiSpriteID)
	{
		if (m_Sprites.size() > 0)
		{
			BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
			if (pBasicSprite != NULL)
			{
				BaseSprite* pCopyOfSprite = new BaseSprite(*pBasicSprite);
				std::vector< BaseSprite* >::iterator sprite_iter = m_Sprites.begin();
				for (unsigned int i = 0; sprite_iter != m_Sprites.end(); ++sprite_iter, ++i)
				{
					if ((*sprite_iter) == NULL)
					{
						(*sprite_iter) = pCopyOfSprite;
						return i;
					}
				}
				m_Sprites.push_back(pCopyOfSprite);
				return ((unsigned int)m_Sprites.size() - 1);
			}
		}
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	//Gets rid of a sprite from our sprite manager
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::RemoveSprite(unsigned int a_uiSpriteID)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			delete pBasicSprite;
			m_Sprites[a_uiSpriteID] = pBasicSprite = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::MoveSprite(unsigned int a_uiSpriteID, int a_iXPos, int a_iYPos)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetPosition(a_iXPos, a_iYPos);
		}
	}

	void SpriteManager::MoveSprite(unsigned int a_uiSpriteID, const float* a_fVec)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetPosition(a_fVec);
		}
	}

	void SpriteManager::GetSpritePosition(unsigned int a_uiSpriteID, glm::vec2& a_v2Pos) const
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->GetPosition(a_v2Pos.x, a_v2Pos.y);
		}
	}

	void SpriteManager::SetSpriteMatrix(unsigned int a_uiSpriteID, const float* a_fm4)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetSpriteMatrix(a_fm4);
		}
	}

	void SpriteManager::GetSpriteMatrix(unsigned int a_uiSpriteID, float* a_fm4)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->GetSpriteMatrix(a_fm4);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::RotateSprite(unsigned int a_uiSpriteID, float a_iRotation)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetRotation(a_iRotation);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Mark for Draw
	// Pushes a copy of the sprite into the draw queue
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::MarkSpriteForDraw(unsigned int a_uiSpriteID)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			BaseSprite* pCopyOfSprite = new BaseSprite(*pBasicSprite);
			m_pSpritesToDrawThisFrame.push(pCopyOfSprite);
		}
	}
	////////////////////////////////////////////////////////////////////////////
	void SpriteManager::SetSpriteUVCoordinates(unsigned int a_uiSpriteID, glm::vec2& a_minUVCoord, glm::vec2& a_maxUVCoord)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetUVCoords(a_minUVCoord, a_maxUVCoord);
		}
	}
	////////////////////////////////////////////////////////////////////////////
	void SpriteManager::GetSpriteUVCoordinates(unsigned int a_uiSpriteID, glm::vec2& a_minUVCoord, glm::vec2& a_maxUVCoord)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->GetUVCoords(a_minUVCoord, a_maxUVCoord);
		}
	}
	////////////////////////////////////////////////////////////////////////////
	void SpriteManager::SetSpriteColour(unsigned int a_uiSpriteID, glm::vec4& a_v4Colour)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetSpritecolour(a_v4Colour);
		}
	}
	////////////////////////////////////////////////////////////////////////////
	void SpriteManager::GetSpriteColour(unsigned int a_uiSpriteID, glm::vec4& a_v4Colour)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->GetSpritecolour(a_v4Colour);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//sets the scale for the sprite
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::SetSpriteScale(unsigned int a_uiSpriteID, glm::vec2& a_v2SpriteScale)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetSize(a_v2SpriteScale.x, a_v2SpriteScale.y);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void SpriteManager::GetSpriteScale(unsigned int a_uiSpriteID, glm::vec2& a_v2SpriteScale)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			float iWidth = 0; float iHeight = 0;
			pBasicSprite->GetSize(iWidth, iHeight);
			a_v2SpriteScale.x = iWidth;
			a_v2SpriteScale.y = iHeight;
		}
	}

	void SpriteManager::SetSpriteBlendMode(unsigned int a_uiSpriteID, const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor)
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->SetBlendParameters(a_uiSourceFactor, a_uiDestinationFactor);
		}
	}

	void SpriteManager::GetSpriteBlendMode(unsigned int a_uiSpriteID, unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor) const
	{
		BaseSprite* pBasicSprite = m_Sprites[a_uiSpriteID];
		if (pBasicSprite != NULL)
		{
			pBasicSprite->GetBlendParameters(a_uiSourceFactor, a_uiDestinationFactor);
		}
	}

}//namespace NHTV
