#include "BMapStrategy.h"
#include "tools.h"

cocos2d::CCString BMapStrategy::getUrl( int x, int y, int z )
{
	const char* pUrl = "http://online1.map.bdimg.com/tile/?qt=tile&x=%s&y=%s&z=%d&styles=pl";
	z = 17 - z;
	z--;

	int offsetX = 2<<z;
	int offsetY = offsetX - 1;

	int numX = x - offsetX;
	int numY = -y + offsetY;

	z++;
	int num = (x + y)%8 + 1;

	CCString xStr;
	xStr.initWithFormat("%d",numX);
	std::string xS = xStr.getCString();
	Tools::string_replace(xS, "-", "M");
	xStr = xS;

	CCString yStr;
	yStr.initWithFormat("%d",numY);
	std::string yS = yStr.getCString();
	Tools::string_replace(yS, "-", "M");
	yStr = yS;


	//原来：http://q3.baidu.com/it/u=x=721;y=209;z=12;v=014;type=web&fm=44
	//更新：http://online1.map.bdimg.com/tile/?qt=tile&x=23144&y=6686&z=17&styles=pl
	CCString strUrl;
	strUrl.initWithFormat(pUrl,xStr.getCString(),yStr.getCString(),z);
	return strUrl;
}

cocos2d::CCString BMapStrategy::getDescription()
{
	return "BaiduMap";
}

