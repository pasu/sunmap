#ifndef __COCOS2D_TileResolver_H__
#define __COCOS2D_TileResolver_H__

#include "cocos2d.h"
#include "RawTile.h"
#include "BitmapCache.h"
#include "SqliteLocalStorage.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"

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
    
    bool put2Cache(RawTile* pTile, CCImage* pTexture);
    
    void clearQueue();
    void clearCache(bool bForce = false);
    
    void load(RawTile* pTile);
private:
	
	
	int m_nStrategyId;
    
	
    
public:
	void* m_pPhysicMap;
    
	SqliteLocalStorage m_SqliteProvider;
	BitmapCache m_CacheProvider;
    
	int m_nLoaded;
    
    JniMethodInfo m_JniLoadFun;
    JniMethodInfo m_JniClearFun;
    
    static void* m_pThis;
};

#endif