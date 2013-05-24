#ifndef __COCOS2D_TileResolver_H__
#define __COCOS2D_TileResolver_H__

#include "cocos2d.h"
#include "RawTile.h"
#include "TileLoader.h"
#include "BitmapCache.h"
#include "SqliteLocalStorage.h"

USING_NS_CC;

class TileResolver
{
public:
	TileResolver();
	TileResolver(void* pPhysicMap);
	~TileResolver();

	void resetLoaded();
	void incLoaded();
	int getLoaded();

	void getTile(RawTile* pTile);

	void setMapSourceId(int sourceId);
	int getMapSourceId();

	 void updateMap(RawTile* pTile, CCImage* pTexture);

	 void put2Cache(RawTile* pTile, CCImage* pTexture);

	 void clearQueue();
	 void clearCache(bool bForce = false);

	 void load(RawTile* pTile);
private:	
	
	void cthread();
private:
	
	int m_nStrategyId;

	

public:
	void* m_pPhysicMap;
	TileLoader* m_pTileLoader;

	SqliteLocalStorage m_SqliteProvider;
	BitmapCache m_CacheProvider;

	int m_nLoaded;
};

#endif