#include "GoogleSatelliteMapStrategy.h"

cocos2d::CCString GoogleSatelliteMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://khms.google.com/kh/v=129&src=app&x=%d&y=%d&z=%d&s=";
	
	CCString strUrl;
	strUrl.initWithFormat(pUrl,x,y,17-z);
	return strUrl;
}

cocos2d::CCString GoogleSatelliteMapStrategy::getDescription()
{
	return "Google Maps Satellite";
}

