#include "BingMapStrategy.h"

cocos2d::CCString BingMapStrategy::getUrl( int x, int y, int z )
{
	z = 17-z;
	std::string strUrl = "";

	int i = 'z';
	for (i = z;i>0;--i)
	{
		char digit = '0';
		int mask = 1 << (i - 1);
		if ((x & mask) != 0) {
			digit++;
		}
		if ((y & mask) != 0) {
			digit++;
			digit++;
		}
		strUrl += digit;
	}
	const char* pUrl = "http://r0.tiles.ditu.live.com/tiles/r%s.jpeg?g=91&mkt=zh-Hans&shading=hill";

	cocos2d::CCString strUrl2;
	strUrl2.initWithFormat(pUrl,strUrl.c_str());
	return strUrl2;
}

cocos2d::CCString BingMapStrategy::getDescription()
{
	return "Bing";
}

