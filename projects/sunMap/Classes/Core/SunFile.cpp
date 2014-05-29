#include "SunFile.h"
#include "cocos2d.h"
#include "tools.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

#else
#include <Windows.h>
#include <assert.h>
#include <process.h>
#include <direct.h>
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#else

bool SunFile::IsExist( const char* strFileDir,bool bFolder )
{
	int nLen = strlen(strFileDir) + 1;
	if(nLen > 0)
	{
		char chLast = strFileDir[nLen-1];
		if(bFolder)
		{
			std::string strTemp = strFileDir;
			strTemp += ".";
			return InnerIsExist(strTemp.c_str());
		}
	}
	return InnerIsExist(strFileDir);
}

bool SunFile::InnerIsExist( const char* strFileDir )
{
	wchar_t *WStr;
	Tools::char2wchar(strFileDir,WStr);
	bool bReturn = false;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(WStr, &FindFileData);
	free(WStr);

	if(hFind != INVALID_HANDLE_VALUE)
	{ 
		bReturn = true;
	}
	
	FindClose(hFind);
	return bReturn;

}

bool SunFile::MkDir( const char* strDir )
{
	wchar_t *WStr;
	Tools::char2wchar(strDir,WStr);
	bool bReturn = _wmkdir(WStr) == 0?true:false;
	free(WStr);

	return bReturn;
}

bool SunFile::MkDirEx( const char* strDir )
{
	std::string strTempDir = strDir;

	std::vector<std::string> dirs = Tools::split(strTempDir,'/');

	strTempDir.empty();
	strTempDir.clear();

	bool bResult = true;	
	int i = 0;
	for(i = 0; i < (int)dirs.size(); i++)
	{
		strTempDir += dirs[i];
		strTempDir += "/";
		bResult = MkDir(strTempDir.c_str());
	}	
	return bResult;
}

#endif