#ifndef __COCOS2D_Tools_H__
#define __COCOS2D_Tools_H__

#include "cocos2d.h"

USING_NS_CC;

class Tools
{
public:
	static void string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst);
	static void char2wchar(const char*& strSrc,wchar_t*& strTarget);
	static std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);
};

#endif