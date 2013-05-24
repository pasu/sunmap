#include "GoogleTerrainMapStrategy.h"

cocos2d::CCString GoogleTerrainMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://mts.google.com/vt/lyrs=t@130,r@207000000&hl=zh-CN&src=app&x=%d&y=%d&z=%d&s=";
	
	CCString strUrl;
	strUrl.initWithFormat(pUrl,x,y,17-z);
	return strUrl;
}

cocos2d::CCString GoogleTerrainMapStrategy::getDescription()
{
	return "Google Maps Terrain";
}

