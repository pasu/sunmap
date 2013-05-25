#include "TileLoader.h"
#include "curl/curl.h"
#include "TileResolver.h"
#include "pthread.h"

#include "PhysicMap.h"

int TileLoader::Max_Thread =9;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct curl_data
{
	TileResolver* pTileResolver;
	RawTile* pRawTile;
	char nTryTime;
};

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);  
	if( NULL == str || NULL == buffer )  
	{  
		return -1;  
	}  

	char* pData = (char*)buffer;  
	str->append(pData, size * nmemb);  
	return nmemb;  
}  

void* download(void* pCurl)
{
	
	RawTile* pTile = ((curl_data*)pCurl)->pRawTile;
	TileResolver* pTileResolver = ((curl_data*)pCurl)->pTileResolver;
	 ((curl_data*)pCurl)->nTryTime--;
	//try
	{	
		CCString strUrl = pTileResolver->m_pTileLoader->getStrategy()->getUrl(pTile->x,pTile->y,pTile->z);

		std::string strResponse;

		CURLcode res;
		CURL* curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, strUrl.getCString());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 25);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

		res = curl_easy_perform(curl);

		int retcode = 0;  
		res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
		double length;
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 

		if (res==CURLE_OK && retcode == 200 && ((int)length)>0 && strResponse.size()>0)
		{
			CCImage* pImage = new CCImage;
			pImage->initWithImageData((void*)strResponse.c_str(),(int)length);
			
			if(((TileResolver*)(pTileResolver))->put2Cache(pTile,pImage))
			{
				((PhysicMap* )(((TileResolver*)(pTileResolver))->m_pPhysicMap))->m_pHandler->addEvent(pTile,pImage);
			}
			((TileResolver*)(pTileResolver))->m_pTileLoader->m_nCount--;
			curl_easy_cleanup(curl);

			delete (curl_data*)pCurl;
			pCurl = NULL;
		}
		else
		{
			curl_easy_cleanup(curl);

			if(((curl_data*)pCurl)->nTryTime>0)
			{
				pthread_t pid;
				pthread_create(&pid, NULL, download, (void*)pCurl);
			}
			else
			{
				((TileResolver*)(pTileResolver))->m_pTileLoader->m_nCount--;
				delete (curl_data*)pCurl;
				pCurl = NULL;
			}
			
			//pthread_mutex_unlock(&mutex);
		}
	}				

	return NULL;
}

TileLoader::TileLoader()
{
	m_bLoading = false;
	m_pTileResolver = NULL;
	m_nCount = 0;
}

TileLoader::TileLoader( void* pTileResolver )
{
	m_bLoading = false;
	m_pTileResolver = pTileResolver;
	m_nCount = 0;
}

TileLoader::~TileLoader()
{
	m_nCount = 0;
}

void TileLoader::addToQueue( RawTile* pTile )
{
	pthread_mutex_lock(&mutex);
	m_Queue.push_back(pTile);
	pthread_mutex_unlock(&mutex);
}

void TileLoader::load( RawTile* pTile )
{
	pthread_mutex_lock(&mutex);
	m_Queue.push_back(pTile);
	pthread_mutex_unlock(&mutex);
}

RawTile* TileLoader::getFromQueue()
{
	pthread_mutex_lock(&mutex);
	int nCount = m_Queue.size();
	if (nCount>0)
	{
		RawTile* pTile = m_Queue[nCount-1];
		m_Queue.pop_back();
		pthread_mutex_unlock(&mutex);
		return pTile;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void TileLoader::clearQueue()
{
	pthread_mutex_lock(&mutex);
	m_Queue.clear();
	pthread_mutex_unlock(&mutex);
}

void TileLoader::run()
{
	while (true) {
		if(m_Queue.size()>0 && m_nCount< TileLoader::Max_Thread)
		{
			//try
			{
				RawTile* pTile = getFromQueue();
				if (pTile)
				{
					m_nCount++;
					pthread_t pid;
					curl_data* pcData = new curl_data;
					pcData->pRawTile = pTile;
					pcData->pTileResolver = (TileResolver*)m_pTileResolver;
					pcData->nTryTime = 5;
					pthread_create(&pid, NULL, download, (void*)pcData);
				}
			}			
		}
	}
}

void TileLoader::setMapStrategy( MapStrategy* pMapStrategy )
{
	m_pMapStrategy = pMapStrategy;
}

MapStrategy* TileLoader::getStrategy()
{
	return m_pMapStrategy;
}

RawTileObject::RawTileObject()
{
	m_pTile = NULL;
}

RawTileObject::~RawTileObject()
{
	m_pTile = NULL;
}
