#include "PublicMsgHandler.h"
#ifdef WIN
#include "pthread/pthread.h"
#elseif
#include <pthread.h>
#endif
#include "MapControl.h"
#include "PhysicMap.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

PublicMsgHandler::PublicMsgHandler()
{
	this->onEnter();
	this->onEnterTransitionDidFinish();
	this->scheduleUpdate();

	m_pMap = NULL;
}

PublicMsgHandler::~PublicMsgHandler()
{
	this->unscheduleUpdate();
}

void PublicMsgHandler::update( float fDelta )
{
	
    if(m_pData.size()>0)
    {
        pthread_mutex_lock(&mutex);
        if(m_pMap != NULL)
        {
            PhysicMap* pPhysicMap = ((MapControl*)m_pMap)->getMap();
            int nCount = m_pTile.size();
            for (int i=0;i<nCount;i++)
            {
                RawTile* pTile = m_pTile[i];
                CCImage* pData = m_pData[i];
                
                pPhysicMap->getTileResolver()->updateMap(pTile,pData);
            }
            
            m_pTile.clear();
            m_pData.clear();
        }
        pthread_mutex_unlock(&mutex);
        
        ((MapControl*)m_pMap)->updateScreen();
    }		
}

void PublicMsgHandler::addEvent( RawTile* pTile,CCImage* pData)
{
	pthread_mutex_lock(&mutex);
	m_pTile.push_back(pTile);
	m_pData.push_back(pData);
	pthread_mutex_unlock(&mutex);
}

void PublicMsgHandler::attach( void* pMap )
{
	m_pMap = pMap;
}
