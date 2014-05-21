#ifndef __COCOS2D_SUNFILE_H__
#define __COCOS2D_SUNFILE_H__

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#define ISPATHSEP(c) ((c)==('/'))
#else
#define ISPATHSEP(c) ((c)==('/') || (c)==('\\'))
#endif

class SunFile
{
public:
	// Ä¿Â¼Îª"/"»ò"\\"
	static bool IsExist(const char* strFileDir,bool bFolder);

	static bool MkDir(const char* strDir);
	static bool MkDirEx(const char* strDir);
private:
	static bool InnerIsExist(const char* strFileDir);
};

#endif