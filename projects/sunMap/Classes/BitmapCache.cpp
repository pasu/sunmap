#include "BitmapCache.h"

#ifdef WIN
#include "pthread/pthread.h"
#elseif
#include <pthread.h>
#endif





static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int BitmapCache::CACHE_SIZE = 200;

CCImage* BitmapCache::getTile( RawTile* pTile )
{
	CCImage* pData = NULL;
	std::map<std::string,CCImage*>::iterator l_it;

	l_it=m_DataCache.find(pTile->toString().getCString());
	 if(l_it!=m_DataCache.end ())
	{
		pData = l_it->second;
	}

	return pData;
}

bool BitmapCache::putToCache( RawTile* pTile, CCImage* pData )
{
	pthread_mutex_lock(&mutex);
    
	std::map<std::string,CCImage*>::iterator l_it;

	l_it=m_DataCache.find(pTile->toString().getCString());
	if(l_it!=m_DataCache.end ())
	{
		delete pData;
		pData = NULL;
		pthread_mutex_unlock(&mutex);
		return false;
	}

	m_DataCache.insert(std::map<std::string,CCImage*>::value_type(pTile->toString().getCString(),pData));
	m_Index.push_back(pTile->toString().getCString());
    
	pthread_mutex_unlock(&mutex);
	return true;
}

BitmapCache::~BitmapCache()
{
	clear(true);
}

void BitmapCache::clear(bool bForce)
{
	if(bForce == true || m_DataCache.size()>BitmapCache::CACHE_SIZE)
	{
        pthread_mutex_lock(&mutex);
		std::map<std::string,CCImage*>::iterator it = m_DataCache.begin();   
		for(;it!=m_DataCache.end();++it)
		{
			CCImage* pData = it->second;
			delete pData;
		}

		m_DataCache.clear();
		m_Index.clear();
        pthread_mutex_unlock(&mutex);
	}
}
