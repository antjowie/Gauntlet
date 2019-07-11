////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Handler
///////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Utilities.h"

#include <cstdio>
#include <string.h>
#include <stdlib.h>

using namespace NHTV;
///////////////////////////////////////////////////////////////
//constructor
///////////////////////////////////////////////////////////////
CTexture::CTexture()
{
	m_iRefCount = 0;
	m_iTextureMemory = 0;
	m_textureHandle = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iBitsPerPixel = 0;
	m_pFilename = nullptr;
}

CTexture::CTexture(const char* filename)
{
	m_iRefCount = 0;
	m_iTextureMemory = 0;
	m_textureHandle = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iBitsPerPixel = 0;
	m_pFilename = nullptr;
	if (filename != NULL)
	{
		int len = (int)strlen(filename);
		m_pFilename = new char[len + 1];
		strcpy(m_pFilename, filename);
		Load(filename);
	}
}

CTexture::CTexture(CTexture &copy)
{
	if (copy.m_pFilename != NULL)
	{
		int len = (int)strlen(copy.m_pFilename);
		m_pFilename = new char[len + 1];
		strcpy(m_pFilename, copy.m_pFilename);
	}
	m_iRefCount = 0;
	m_textureHandle = copy.m_textureHandle;
	m_iWidth = copy.m_iWidth;
	m_iHeight = copy.m_iHeight;
	m_iBitsPerPixel = copy.m_iBitsPerPixel;
	m_iTextureMemory = copy.m_iTextureMemory;
}
///////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
CTexture::~CTexture()
{
	if (m_pFilename)
	{
		delete[]m_pFilename;
	}
}
///////////////////////////////////////////////////////////////	
unsigned int CTexture::Load(const char* filename)
{
	unsigned int uiTextureID = -1;
	if (filename != nullptr)
	{
		if (m_pFilename == nullptr)
		{
			int len = (int)strlen(filename);
			m_pFilename = new char[len + 1];
			strcpy(m_pFilename, filename);
		}
		uiTextureID = NHTV::Utility::loadTexture(m_pFilename, m_iWidth, m_iHeight, m_iBitsPerPixel);
		m_iTextureMemory = m_iWidth * m_iHeight * m_iBitsPerPixel;
		m_textureHandle = uiTextureID;
		IncrememntRefCount();
	}
	return uiTextureID;
}
///////////////////////////////////////////////////////////////
// Unload() 
//  - decrements the reference count.. 
//    if this count gets to 0 then nothing is using the texture and we
//    can unload it from the GPU Memory.
///////////////////////////////////////////////////////////////
bool CTexture::UnLoad()
{
	DecrementRefCount();
	if (m_iRefCount <= 0)
	{
		glDeleteTextures(1, &m_textureHandle);
		return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
void CTexture::GetTextureSize(unsigned int &iWidth, unsigned int &iHeight)
{
	iWidth = m_iWidth;
	iHeight = m_iHeight;
}
///////////////////////////////////////////////////////////////
// Increment ref count
//  - increment the reference count
///////////////////////////////////////////////////////////////
void CTexture::IncrememntRefCount()
{
	++m_iRefCount;
}
///////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void CTexture::DecrementRefCount()
{
	--m_iRefCount;
}
///////////////////////////////////////////////////////////////
// Get the size of this texture in memory
///////////////////////////////////////////////////////////////
unsigned int CTexture::GetTextureSizeInMemory()
{
	return m_iTextureMemory;
}

