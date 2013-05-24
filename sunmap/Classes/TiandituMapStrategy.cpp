#include "TiandituMapStrategy.h"

cocos2d::CCString TiandituMapStrategy::getUrl( int x, int y, int z )
{
	char* pUrl = "http://t0.tianditu.com/vec_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&TILEMATRIXSET=w&TILEMATRIX=%d&TILEROW=%d&TILECOL=%d&FORMAT=tiles";

	CCString strUrl;
	strUrl.initWithFormat(pUrl,17-z,y,x);
	return strUrl;
}

cocos2d::CCString TiandituMapStrategy::getDescription()
{
	return "Tianditu";
}

