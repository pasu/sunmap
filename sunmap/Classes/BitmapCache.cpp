#include "BitmapCache.h"
#include "pthread.h"

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

void BitmapCache::putToCache( RawTile* pTile, CCImage* pData )
{
	pthread_mutex_lock(&mutex);

	m_DataCache.insert(std::map<std::string,CCImage*>::value_type(pTile->toString().getCString(),pData));
    m_Index.push_back(pTile->toString().getCString());
    
//      if(m_Index.size()>BitmapCache::CACHE_SIZE)
//      {
//          for(int i=0;i<BitmapCache::CACHE_SIZE/2;i++)
//          {
//              std::string strIndex = m_Index[i];
//              
//              std::map<std::string,CCImage*>::iterator l_it;
//              
//              l_it=m_DataCache.find(strIndex);
//              if(l_it!=m_DataCache.end ())
//              {
//                  CCImage* ppData = l_it->second;
//              
//                  m_DataCache.erase(l_it);
//                 // delete ppData->pImage;
//                  delete ppData;
//              }
//              
//          }
//          
//          m_Index.erase(m_Index.begin(),m_Index.begin()+BitmapCache::CACHE_SIZE/2);
//     }    
	pthread_mutex_unlock(&mutex);
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
