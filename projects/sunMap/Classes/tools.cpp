#include "tools.h"

std::vector<std::string>& Tools::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> Tools::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	Tools::split(s, delim, elems);
	return elems;
}

void Tools::string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst)

{

	std::string::size_type pos = 0;

	while( (pos = strBig.find(strsrc, pos)) != std::string::npos)

	{

		strBig.replace(pos, strsrc.length(), strdst);

		pos += strdst.length();

	}

}

void Tools::char2wchar(const char*& strSrc,wchar_t*& strTarget)
{
	size_t len = strlen(strSrc) + 1;
	size_t converted = 0;
	//wchar_t *WStr;
	strTarget=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, strTarget, len, strSrc, _TRUNCATE);
}