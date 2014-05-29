#include "TileResolver_android.h"
#include "PhysicMap.h"
#include "curl/curl.h"
#include "MapStrategyFactory.h"

void* TileResolver::m_pThis = NULL;


extern "C"

{
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
    
    void download(void* pCurl)
    {
        
        RawTile* pTile = ((curl_data*)pCurl)->pRawTile;
               
        
        TileResolver* pTileResolver = ((curl_data*)pCurl)->pTileResolver;
        //try
        {
            
            CCString strUrl = MapStrategyFactory::getStrategy(pTileResolver->getMapSourceId())->getUrl(pTile->x,pTile->y,pTile->z);
            
            std::string strResponse;
            
            CURLcode res;
            CURL* curl = curl_easy_init();
            
            curl_easy_setopt(curl, CURLOPT_URL, strUrl.getCString());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            
            
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
                curl_easy_cleanup(curl);
                
                delete (curl_data*)pCurl;
                pCurl = NULL;
            }
        }
        
        return ;
    }
    
    void Java_com_map_sunMap_sunMap_nativeDownload(JNIEnv*  env, jobject thiz,jint nAddress)
    
    {
        RawTile* pTile = (RawTile*)nAddress;
        TileResolver* pResolver = (TileResolver*)(TileResolver::m_pThis);
        if (pTile && pResolver)
        {
            curl_data* pcData = new curl_data;
            pcData->pRawTile = pTile;
            pcData->pTileResolver = pResolver;
            download(pcData);
        }
        
    }
    
    
    
}

TileResolver::TileResolver()
{
	m_pPhysicMap = NULL;
	m_nLoaded = 0;
	m_nStrategyId = 0;
	
    m_pThis = this;
    
	
     
    
    
}

TileResolver::TileResolver( void* pPhysicMap )
{
	m_pPhysicMap = pPhysicMap;
	m_nLoaded = 0;
	m_nStrategyId = 0;
     m_pThis = this;
}

TileResolver::~TileResolver()
{
	//m_JniLoadFun.env->DeleteLocalRef(m_JniLoadFun.classID);
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
    
    CCString strAddress;
    strAddress.initWithFormat("%d",(int)pTile);
    JniMethodInfo JniLoadFun;
    
    JniHelper::getStaticMethodInfo(JniLoadFun,
                                   "com/map/sunMap/sunMap",
                                   "load",
                                   "(Ljava/lang/String;)V");

    jstring stringArg = JniLoadFun.env->NewStringUTF(strAddress.getCString());// $
    
    JniLoadFun.env->CallStaticVoidMethod(JniLoadFun.classID, JniLoadFun.methodID, stringArg);
    
    JniLoadFun.env->DeleteLocalRef(stringArg);
    
    JniLoadFun.env->DeleteLocalRef(JniLoadFun.classID);
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

bool TileResolver::put2Cache( RawTile* pTile, CCImage* pTexture )
{
	return m_CacheProvider.putToCache(pTile,pTexture);
}

void TileResolver::clearQueue()
{
	
}

void TileResolver::setMapSourceId(int sourceId)
{
	m_nStrategyId = sourceId;
	
	clearCache(true);
	clearQueue();
	//((PhysicMap*)m_pPhysicMap)->releaseCells();
	((PhysicMap*)m_pPhysicMap)->reloadTiles();
}

void TileResolver::clearCache(bool bForce )
{
	m_CacheProvider.clear(bForce);
}
