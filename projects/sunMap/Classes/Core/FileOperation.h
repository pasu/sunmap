#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include <string>

class FileOperation 
{
public:
	static void saveFile(char*& buf,int length,std::string strName);
	static void readFile(std::string strName);
	static std::string getFilePath(std::string strName);
};

#endif
