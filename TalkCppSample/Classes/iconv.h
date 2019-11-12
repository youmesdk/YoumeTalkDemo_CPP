#ifndef __ICONV_H__
#define __ICONV_H__

#include <string.h>
#ifdef WIN32
#include <malloc.h>
#endif

#ifdef WIN32
extern const char* gb23122utf8(const char* gb2312);
#define UTEXT(str) gb23122utf8(str)
#else
#define UTEXT(str) str
#endif

#endif
