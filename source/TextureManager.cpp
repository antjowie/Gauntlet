////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Manager
///////////////////////////////////////////////////////////////

#include "TextureManager.h"

using namespace NHTV;

#pragma region Singleton Funtionality
///////////////////////////////////////////////////////////////
//Singleton functionality
///////////////////////////////////////////////////////////////
CTextureManager* CTextureManager::m_pInstance = nullptr;

CTextureManager* CTextureManager::CreateInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CTextureManager();
	}
	return m_pInstance;
}
///////////////////////////////////////////////////////////////
CTextureManager* CTextureManager::GetInstance()
{
	return m_pInstance;
}
///////////////////////////////////////////////////////////////
void CTextureManager::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
///////////////////////////////////////////////////////////////
#pragma endregion
///////////////////////////////////////////////////////////////
CTextureManager::CTextureManager()
{
	m_pTotalTextureMemory = 0;
}

CTextureManager::~CTextureManager()
{
}
///////////////////////////////////////////////////////////////
//Load Texture 
// - This uses an STD map as a texture atlas/dictionary to keep track of 
//   textures that are currently in memory and increments/decrements references 
//   as they are used or released
///////////////////////////////////////////////////////////////
unsigned int CTextureManager::LoadTexture(const char* a_pTextureName)
{
	CTexture* pTexture = nullptr;
	if (a_pTextureName != nullptr)
	{
		std::map< std::string, CTexture* >::iterator dictionaryIter = m_pTextureDictionary.find(a_pTextureName);
		if (dictionaryIter != m_pTextureDictionary.end())
		{
			//We have found our Texture in the Dictionary return a reference to to it and 
			//increment the reference count for the texture too
			pTexture = dictionaryIter->second;
			pTexture->IncrememntRefCount();
		}
		else
		{
			//THis texture was not in memory so we best load it into memory
			pTexture = new CTexture(a_pTextureName);
			m_pTextureDictionary[a_pTextureName] = pTexture;
			m_pTotalTextureMemory += pTexture->GetTextureSizeInMemory();
		}
	}
	return (pTexture) ? pTexture->GetTextureHandle() : -1;
}
///////////////////////////////////////////////////////////////
// Removes a texture from memory
// - internally just decrements a reference counter, when that
// gets to 0 it will unload it.
///////////////////////////////////////////////////////////////
void CTextureManager::RemoveTexture(unsigned int& a_uiTextureID)
{
	std::map< std::string, CTexture* >::iterator dictionaryIter = m_pTextureDictionary.begin();
	for (; dictionaryIter != m_pTextureDictionary.end(); ++dictionaryIter)
	{
		CTexture* pTexture = static_cast<CTexture*>(dictionaryIter->second);
		if (pTexture->GetTextureHandle() == a_uiTextureID)
		{
			if (pTexture->UnLoad())
			{
				m_pTotalTextureMemory -= pTexture->GetTextureSizeInMemory();
				m_pTextureDictionary.erase(dictionaryIter);
				delete pTexture;
				pTexture = nullptr;
			}
			a_uiTextureID = -1;
			break;
		}
	}
}
///////////////////////////////////////////////////////////////
void CTextureManager::IncrementTextureReference(unsigned int a_uiTextureID)
{
	std::map< std::string, CTexture* >::iterator dictionaryIter = m_pTextureDictionary.begin();
	for (; dictionaryIter != m_pTextureDictionary.end(); ++dictionaryIter)
	{
		CTexture* pTexture = static_cast<CTexture*>(dictionaryIter->second);
		if (pTexture->GetTextureHandle() == a_uiTextureID)
		{
			pTexture->IncrememntRefCount();
		}
	}
}
///////////////////////////////////////////////////////////////
void CTextureManager::DecrementTextureReference(unsigned int a_uiTextureID)
{
	std::map< std::string, CTexture* >::iterator dictionaryIter = m_pTextureDictionary.begin();
	for (; dictionaryIter != m_pTextureDictionary.end(); ++dictionaryIter)
	{
		CTexture* pTexture = static_cast<CTexture*>(dictionaryIter->second);
		if (pTexture->GetTextureHandle() == a_uiTextureID)
		{
			pTexture->DecrementRefCount();
		}
	}
}

