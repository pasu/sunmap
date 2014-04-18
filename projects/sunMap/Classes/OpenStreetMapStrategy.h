#ifndef __COCOS2D_OpenStreetMapStrategy_H__
#define __COCOS2D_OpenStreetMapStrategy_H__

#include "cocos2d.h"
#include "MapStrategy.h"

USING_NS_CC;

class OpenStreetMapStrategy : public MapStrategy
{
public:
	CCString getUrl(int x, int y, int z);
	CCString getDescription();
};

#endif