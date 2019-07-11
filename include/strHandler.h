//////////////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	30/10/2011
// Brief:	Cross-platform string helpers - MS kinda shits me
//////////////////////////////////////////////////////////////////////////
#ifndef _STR_HANDLER_H
#define _STR_HANDLER_H
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#ifdef _WIN32
#include <tchar.h>
#else
typedef char TCHAR;
#endif

typedef char TAnsiChar;
#ifdef _WIN32
typedef wchar_t TUnicodeChar;
#else
typedef unsigned short TUnicodeChar;
#endif

//////////////////////////////////////////////////////////////////////////
class StrHandler
{
public:
	// Standard string functions
	static	int		Length(const TCHAR *str);
	static	void	Copy(TCHAR *buffer, int bufferSize, const TCHAR *str);
	static	int		Cat(char * str1, const int bufferSize, const char * toAdd);
	static	int		NCopy(TCHAR *dst, int bufferSize, const TCHAR *src, size_t maxCount);
	static	int		Cmp(TCHAR const *str1, TCHAR const *str2);
	static	int		SPrintf(char * buffer, size_t bufferSize, char const * fmt, ...);
	static	int		ToUpper(char *str);
	static	int		ICmp(char const * const str1, char const * const str2);
private:
	// Temporary storage
	TCHAR	m_szBuffer[2048];
};

//////////////////////////////////////////////////////////////////////////
#endif // _STR_HANDLER_H
//////////////////////////////////////////////////////////////////////////