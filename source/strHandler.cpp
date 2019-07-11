//////////////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	30/10/2011
// Brief:	Cross-platform string helpers
//////////////////////////////////////////////////////////////////////////
#include "strHandler.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"
//////////////////////////////////////////////////////////////////////////
int StrHandler::NCopy(TCHAR *dst, int bufferSize, const TCHAR *src, size_t maxCount)
{
#ifdef _WIN32
	return strncpy_s(dst, bufferSize, src, maxCount);
#else
	// This returns dst if you simply return strncpy, thats not what you want to do here
	strncpy(dst, src, maxCount);
	//so return count as the return
	return (int)maxCount;
#endif
}
//////////////////////////////////////////////////////////////////////////
int StrHandler::ToUpper(char *str)
{
	int Len = 0;
	while (str)
	{
		*str = (char)toupper(*str);
		str++;
		Len++;
	}
	return Len;
}
//////////////////////////////////////////////////////////////////////////
int StrHandler::ICmp(char const * const str1, char const * const str2)
{
#ifdef _WIN32
	return _stricmp(str1, str2);
#else

	int n = 0;
	while (str1[n] || str2[n])
	{
		char a = toupper(str1[n]);
		char b = toupper(str2[n]);
		if (a != b)
		{
			return (a - b);
		}
		++n;
	}

	return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////
int StrHandler::SPrintf(char * buffer, size_t bufferSize, char const * fmt, ...)
{
	int ret;
	va_list argp;
	va_start(argp, fmt);
#ifdef _WIN32
	ret = vsprintf_s(buffer, bufferSize, fmt, argp);
#else
	ret = vsprintf(buffer, fmt, argp);
#endif
	va_end(argp);

	return ret;
}
//////////////////////////////////////////////////////////////////////////
int StrHandler::Length(const TCHAR *str)
{
#ifdef _WIN32
	return (int)_tcslen(str);
#else
	return (int)strlen(str);
#endif
}
//////////////////////////////////////////////////////////////////////////
void StrHandler::Copy(TCHAR *buffer, int bufferSize, const TCHAR *str)
{
#ifdef _WIN32
	_tcscpy_s(buffer, bufferSize, str);
#else
	strcpy(buffer, str);
#endif
}
//////////////////////////////////////////////////////////////////////////
int StrHandler::Cat(char *str1, const int bufferSize, const char * toAdd)
{
#ifdef _WIN32
	return strcat_s(str1, bufferSize, toAdd);
#else
	strcat(str1, toAdd);
	return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////
int	StrHandler::Cmp(TCHAR const *str1, TCHAR const *str2)
{
	return strcmp(str1, str2);
}
