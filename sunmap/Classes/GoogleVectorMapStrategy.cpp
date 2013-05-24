#include "GoogleVectorMapStrategy.h"

cocos2d::CCString GoogleVectorMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://mts.google.com/vt/lyrs=m@216000000&hl=zh-CN&src=app&x=%d&y=%d&z=%d&s=";
	
	CCString strUrl;
	strUrl.initWithFormat(pUrl,x,y,17-z);
	return strUrl;
}

cocos2d::CCString GoogleVectorMapStrategy::getDescription()
{
	return "Google Maps Vector";
}

