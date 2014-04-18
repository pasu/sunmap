#include "OpenStreetMapStrategy.h"

cocos2d::CCString OpenStreetMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://b.tile.openstreetmap.org/%d/%d/%d.png";
	
	CCString strUrl;
	strUrl.initWithFormat(pUrl,17-z,x,y);
	return strUrl;
}

cocos2d::CCString OpenStreetMapStrategy::getDescription()
{
	return "OSM";
}

