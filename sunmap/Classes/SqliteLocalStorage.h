#ifndef __COCOS2D_SqliteLocalStorage_H__
#define __COCOS2D_SqliteLocalStorage_H__

#include "cocos2d.h"
#include "RawTile.h"
#include "TileLoader.h"
#include "sqlite3.h"

USING_NS_CC;

class SqliteLocalStorage
{
public:
	SqliteLocalStorage();
	~SqliteLocalStorage();

	bool isExist(RawTile* pTile);
	CCImage* getTile(RawTile* pTile);
	void putToCache(RawTile* pTile, CCImage* pData);	
private:
	sqlite3* m_pConn;
};

#endif