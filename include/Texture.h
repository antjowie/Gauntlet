//////////////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Handler
//////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

namespace NHTV
{
	//////////////////////////////////////////////////////////////////////////
	//Class to load in texture data
	//  A texture is just a buffer with some data in it regardig the pixel colours
	//  All texture data is pushed to the GPU on load so that it doesn't occupy memory on the CPU side
	//  This texture class uses simplistic reference counting to keep track of how many objects are using the texture
	//////////////////////////////////////////////////////////////////////////
	class CTexture
	{
	public:

		CTexture();
		CTexture(const char* filename);
		CTexture(CTexture &copy);
		virtual ~CTexture();

		unsigned int	Load(const char* filename);
		bool			UnLoad();

		unsigned int	GetTextureHandle() { return m_textureHandle; }
		const char*		GetFileName() { return m_pFilename; }
		void			GetTextureSize(unsigned int &iWidth, unsigned int &iHeight);
		unsigned int	GetTextureSizeInMemory();

		void			IncrememntRefCount();
		void			DecrementRefCount();

	private:
		unsigned int	m_textureHandle;
		int				m_iWidth;
		int				m_iHeight;
		int				m_iBitsPerPixel;
		char*			m_pFilename;
		//RefCount is the number of times this texture is being used
		unsigned int    m_iRefCount;
		unsigned int	m_iTextureMemory;
	};
}//end namespace NHTV

#endif //__TEXTURE_H__