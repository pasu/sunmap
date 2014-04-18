#include "AMapStrategy.h"

cocos2d::CCString AMapStrategy::getUrl( int x, int y, int z )
{
	const char* pUrl = "http://webrd02.is.autonavi.com/appmaptile?x=%d&y=%d&z=%d&lang=zh_cn&size=1&scale=1&style=7";

	CCString strUrl;
	strUrl.initWithFormat(pUrl,x,y,17-z);
	return strUrl;
}

cocos2d::CCString AMapStrategy::getDescription()
{
	return "AMap";
}

