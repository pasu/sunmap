#include "SuperMapCloudMapStrategy.h"

cocos2d::CCString SuperMapCloudMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://t0.supermapcloud.com/FileService/image?map=quanguo&type=web&x=%d&y=%d&z=%d";
	CCString* pStrUrl = CCString::createWithFormat(pUrl,x,y,17-z);
	return pStrUrl->getCString();
}

cocos2d::CCString SuperMapCloudMapStrategy::getDescription()
{
	return "SuperMap Cloud";
}

