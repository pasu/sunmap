#include "ArcGisMapStrategy.h"

cocos2d::CCString ArcGisMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://www.arcgisonline.cn/ArcGIS/rest/services/ChinaOnlineCommunity/MapServer/tile/%d/%d/%d";
	
	CCString strUrl;
	strUrl.initWithFormat(pUrl,17-z,y,x);
	return strUrl;
}

cocos2d::CCString ArcGisMapStrategy::getDescription()
{
	return "ArcGIS online";
}

