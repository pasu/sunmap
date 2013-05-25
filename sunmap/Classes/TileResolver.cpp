#include "TileResolver.h"
#include "PhysicMap.h"
#include "curl/curl.h"
#include "pthread.h"
#include "MapStrategyFactory.h"

void* Fun(void* arg)
{
	((TileLoader*)(arg))->run();

	return NULL;
}

// void* Fun(void* arg)
// {
// 	SqliteLocalStorage* pSqlite = (SqliteLocalStorage*)arg;
// 	pSqlite->getTile();
// 
// 	return NULL;
// }

TileResolver::TileResolver()
{
	m_pPhysicMap = NULL;
	m_nLoaded = 0;
	m_nStrategyId = 0;
	m_pTileLoader = new TileLoader((void*)this);
	cthread();
}

TileResolver::TileResolver( void* pPhysicMap )
{
	m_pPhysicMap = pPhysicMap;
	m_nLoaded = 0;
	m_nStrategyId = 0;
	m_pTileLoader = new TileLoader((void*)this);
	cthread();
}

TileResolver::~TileResolver()
{
	if (m_pTileLoader)
	{
		delete m_pTileLoader;
	}
}

void TileResolver::resetLoaded()
{
	this->m_nLoaded = 0; 
}

void TileResolver::incLoaded()
{
	this->m_nLoaded++;
}

int TileResolver::getMapSourceId()
{
	return m_nStrategyId;
}

void TileResolver::getTile(RawTile* pTile)
{
	if (pTile->s == -1) {
		return;
	}	

	CCImage* pTexture = NULL;
	pTexture = m_CacheProvider.getTile(pTile);
	if(pTexture != NULL)
	{
		m_nLoaded++;
		updateMap(pTile,pTexture);
	}
	else
	{
		//pTexture = m_SqliteProvider.getTile(pTile);		

		if(pTexture != NULL)
		{
			m_nLoaded++;
			updateMap(pTile,pTexture);
		}
		else
		{
			m_nLoaded++;
			load(pTile);
		}
	}
	return ;
}

void TileResolver::load( RawTile* pTile )
{
	m_pTileLoader->load(pTile);	
	return ;
}

void TileResolver::updateMap( RawTile* pTile, CCImage* pTexture )
{
	if (pTile->s == m_nStrategyId) {
		((PhysicMap*)m_pPhysicMap)->update(pTexture, pTile);
	}
}

int TileResolver::getLoaded()
{
	return m_nLoaded;
}

void TileResolver::cthread()
{
	//HANDLE hThread = CreateThread(NULL, 0, Fun, m_pTileLoader, 0, NULL);
	pthread_t pid;
	pthread_create(&pid, NULL, Fun, (void*)m_pTileLoader);
}

bool TileResolver::put2Cache( RawTile* pTile, CCImage* pTexture )
{
	return m_CacheProvider.putToCache(pTile,pTexture);
}

void TileResolver::clearQueue()
{
	m_pTileLoader->clearQueue();
}

void TileResolver::setMapSourceId(int sourceId)
{
	m_nStrategyId = sourceId;
	MapStrategy* pS = MapStrategyFactory::getStrategy(sourceId);
	m_pTileLoader->setMapStrategy(pS);
	clearCache(true);
	clearQueue();
	//((PhysicMap*)m_pPhysicMap)->releaseCells();
	((PhysicMap*)m_pPhysicMap)->reloadTiles();
}

void TileResolver::clearCache(bool bForce )
{
	m_CacheProvider.clear(bForce);
}
