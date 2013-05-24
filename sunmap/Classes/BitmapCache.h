#ifndef __COCOS2D_BitmapCache_H__
#define __COCOS2D_BitmapCache_H__

#include "cocos2d.h"
#include "RawTile.h"
#include "TileLoader.h"

USING_NS_CC;

class BitmapCache
{
public:

	~BitmapCache();

	static int CACHE_SIZE;

	CCImage* getTile(RawTile* pTile);
	void clear(bool bForce = false);

	void putToCache(RawTile* pTile, CCImage* pData);

private:
	std::map<std::string,CCImage*> m_DataCache;
    std::vector<std::string> m_Index;
};

#endif