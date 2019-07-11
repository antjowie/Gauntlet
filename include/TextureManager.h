////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Manager
///////////////////////////////////////////////////////////////
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "Utilities.h"
#include <map>
#include <string>
#include "Texture.h"

namespace NHTV
{
	//////////////////////////////////////////////////////////////////////////
	//A manager class for the textures currently in memory
	//  - Singleton class
	//////////////////////////////////////////////////////////////////////////
	class CTextureManager
	{
	protected:
		CTextureManager();
		~CTextureManager();
	public:
		static CTextureManager*		CreateInstance();
		static CTextureManager*		GetInstance();
		static void					DestroyInstance();
		//
		unsigned int				LoadTexture(const char* a_pTextureName);
		void					    RemoveTexture(unsigned int& a_uiTextureID);
		void						IncrementTextureReference(unsigned int a_uiTextureID);
		void						DecrementTextureReference(unsigned int a_uiTextureID);
	private:
		static CTextureManager*					m_pInstance;
		std::map< std::string, CTexture* >		m_pTextureDictionary;
		unsigned int							m_pTotalTextureMemory;
	};

}//namespace NHTV

#endif //__TEXTUREMANAGER_H__