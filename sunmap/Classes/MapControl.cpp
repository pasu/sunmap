#include "MapControl.h"
#include "GeoUtils.h"
#include "ControlLayer.h"

MapControl::MapControl(): m_tBeginPos(CCPointZero)
{
	setTouchEnabled(true);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	RawTile rawTile;
    m_pSprite = NULL;
	m_Map.initPhysicMap(size.width,size.height,rawTile,(void*)this);

	m_bIsNew = true;	
	m_firstEnd.tv_sec = m_firstEnd.tv_usec = 0;
	autorelease();
    
    CCImage* pImage = m_Map.getBgImage();
    CCTexture2D* pTexture = new CCTexture2D();
    pTexture->initWithImage(pImage);
    
    
    m_pSprite = CCSprite::createWithTexture(pTexture);
    m_pSprite->setPosition(ccp(size.width/2, size.height/2));
    float winw = size.width; //获取屏幕宽度
    float winh = size.height;//获取屏幕高度
    
    float spx = m_pSprite->getTextureRect().getMaxX();
    float spy = m_pSprite->getTextureRect().getMaxY();
    
    m_pSprite->setScaleX(winw/spx); //设置精灵宽度缩放比例
    m_pSprite->setScaleY(winh/spy);
    m_pSprite->retain();
    m_pControlLayer = NULL;
    m_bMutiTouch = false;
    m_fMutiDistance = 0.0;
    m_offsetX = m_offsetY = 0.0;
    m_nTouches = 0;
    m_dScale = 1.0;
}


MapControl::MapControl( const RawTile& tile ): m_tBeginPos(CCPointZero)
{
	setTouchEnabled(true);
	m_bIsNew = true;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    m_pSprite = NULL;
	m_Map.initPhysicMap(size.width,size.height,tile,(void*)this);
	m_firstEnd.tv_sec = m_firstEnd.tv_usec = 0;
    
    CCImage* pImage = m_Map.getBgImage();
    CCTexture2D* pTexture = new CCTexture2D();
    pTexture->initWithImage(pImage);
    
    
    m_pSprite = CCSprite::createWithTexture(pTexture);
    m_pSprite->setPosition(ccp(size.width/2, size.height/2));
    float winw = size.width; //获取屏幕宽度
    float winh = size.height;//获取屏幕高度
    
    float spx = m_pSprite->getTextureRect().getMaxX();
    float spy = m_pSprite->getTextureRect().getMaxY();
    
    m_pSprite->setScaleX(winw/spx); //设置精灵宽度缩放比例
    m_pSprite->setScaleY(winh/spy);
    m_pSprite->retain();
    
    m_pControlLayer = NULL;
    m_bMutiTouch = false;
    m_fMutiDistance = 0.0;
    m_offsetX = m_offsetY = 0.0;
    m_nTouches = 0;
    m_dScale = 1.0;
}

MapControl::~MapControl()
{

}

void MapControl::attachControlLayer(void* pControlLayer)
{
    m_pControlLayer = pControlLayer;
}

void MapControl::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
    m_nTouches += pTouches->count();
    if(pTouches->count()>1)
        return;
	m_Map.m_bInMove = false;
    
    if(m_nTouches == 1)
    {
        CCSetIterator it = pTouches->begin();
        CCTouch* touch = (CCTouch*)(*it);

        m_tBeginPos = touch->getLocation();
	
        CCTime::gettimeofdayCocos2d(&m_now, NULL);
        if (m_firstEnd.tv_sec != 0)
        {
            long nTime = (m_now.tv_sec - m_firstEnd.tv_sec)*1000 + (m_now.tv_usec-m_firstEnd.tv_usec)/1000;
            if(nTime>100 && nTime<250)
            {
                zoomOut();
                if(m_pControlLayer)
                {
                    ((ControlLayer*)m_pControlLayer)->updateScale();
                }
            }
        }
	
        m_firstEnd.tv_sec = m_now.tv_sec;
        m_firstEnd.tv_usec = m_now.tv_usec;
    }
}

void MapControl::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    if(pTouches->count() == 1 && m_bMutiTouch==false)
    {
        cc_timeval m_now2;
        CCTime::gettimeofdayCocos2d(&m_now2, NULL);
        
        m_Map.m_bInMove = true;
        CCSetIterator it = pTouches->begin();
        CCTouch* touch = (CCTouch*)(*it);
        
        CCPoint touchLocation = touch->getLocation();
        
        float nMoveX = m_Map.getGlobalOffset().x + touchLocation.x - m_tBeginPos.x;
        float nMoveY = m_Map.getGlobalOffset().y - (touchLocation.y - m_tBeginPos.y);
        
        
        m_Map.setGlobalOffset(CCPoint(nMoveX,nMoveY));
        //updateScreen();
        //return;
        
        if(m_tBeginPos.equals(touchLocation) == false)
        {
            CCObject* child;
            
            CCARRAY_FOREACH(getChildren(), child)
            {
                CCSprite* pNode = (CCSprite*) child;
                if (pNode && pNode->getTag() != 0)
                {
                    CCActionInterval* actionBy = CCMoveBy::create(0.1, CCPoint(touchLocation.x - m_tBeginPos.x,touchLocation.y - m_tBeginPos.y));
                    
                    
                    pNode->runAction(actionBy);
                }
            }
            
            
            m_tBeginPos = touchLocation;
        }
        
        if ((m_now2.tv_sec> m_now.tv_sec ))
        {
            m_Map.m_bInMove = false;
            CCSetIterator it = pTouches->begin();
            CCTouch* touch = (CCTouch*)(*it);
            
            CCPoint touchLocation = touch->getLocation();
            
            float nMoveX = m_Map.getGlobalOffset().x + touchLocation.x - m_tBeginPos.x;
            float nMoveY = m_Map.getGlobalOffset().y - (touchLocation.y - m_tBeginPos.y);
            
            m_Map.setGlobalOffset(CCPoint(nMoveX,nMoveY));
            m_Map.quickHack();
            updateScreen();
            m_tBeginPos = touchLocation;
            
            m_now.tv_usec = m_now2.tv_usec;
        }
    }
    else if(pTouches->count() == 2)
    {
        
        CCTouch *pCurTouch1;
        CCTouch *pCurTouch2;
        int i = 0;
        for( CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++ )
        {
            if( i == 0 )
            {
                pCurTouch1 = (CCTouch*)*iterTouch;
            }else if( i == 1 )
            {
                pCurTouch2 = ( CCTouch* )*iterTouch;
                break;
            }
            i++;
        }
        
        float lastDistance = ccpDistance( pCurTouch1->getLocation(), pCurTouch2->getLocation() );
        if(fabs(m_fMutiDistance-0)<0.1)
        {
            m_fMutiDistance = lastDistance;	
        }
        
        if(m_bMutiTouch == false)
        {
            m_fMutiDistance = lastDistance;
            m_bMutiTouch = true;
            
            CCObject* child;
            
            CCARRAY_FOREACH(getChildren(), child)
            {
                CCSprite* pNode = (CCSprite*) child;
                if (pNode && pNode->getTag() != 0)
                {
                    m_spritePnt.push_back(pNode->getPosition());
                    
                }
            }
            CCTime::gettimeofdayCocos2d(&m_lastTouchTime, NULL);
        }
        else
        {
            cc_timeval m_nowTouchTime;
            CCTime::gettimeofdayCocos2d(&m_nowTouchTime, NULL);
            
            if(m_nowTouchTime.tv_sec == m_lastTouchTime.tv_sec && m_nowTouchTime.tv_usec-m_lastTouchTime.tv_usec<20000)
                return;
            
            m_lastTouchTime = m_nowTouchTime;
            float offsetX,offsetY;
            if( lastDistance - m_fMutiDistance > 0 )
            {
                m_dScale += 0.1;// (m_dScale * sqrt(lastDistance/m_fMutiDistance) );
                if(m_dScale>2)
                    m_dScale = 2;
            }
            else
            {
                
                m_dScale -= 0.1;// (m_dScale * sqrt(lastDistance/m_fMutiDistance) );
                if(m_dScale<0.5)
                    m_dScale = 0.5;
            }
            
                        
            //m_dScale = (m_dScale * lastDistance/m_fMutiDistance );
            
            CCPoint pntOrg(m_Map.getWidth()/2,m_Map.getHeight()/2);
            int nIndex = 0;
            if(fabs(m_dScale-1)>0.1)
            {
                
                CCObject* child;
                
                CCARRAY_FOREACH(getChildren(), child)
                {
                    CCSprite* pNode = (CCSprite*) child;
                    if (pNode && pNode->getTag() != 0)
                    {
                        CCPoint pntNow = pNode->getPosition();
                        CCPoint pnt = m_spritePnt[nIndex++];
                        offsetX = pntOrg.x + (pnt.x - pntOrg.x)*m_dScale;
                        offsetY = pntOrg.y + (pnt.y - pntOrg.y)*m_dScale;
                        
                        CCActionInterval* actionBy = CCMoveBy::create(0.00001, CCPoint(offsetX - pntNow.x,offsetY-pntNow.y));
                        CCActionInterval* ScaleTo = CCScaleBy::create(0.00001, m_dScale/pNode->getScale());

                        pNode->runAction(CCSpawn::create( ScaleTo,actionBy , NULL));
                    }
                }
            }
        }
        
        
    }
}

void MapControl::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    m_nTouches -= pTouches->count();
    if (m_bMutiTouch == true) {
        if(m_nTouches == 0)
        {
            m_Map.m_bInMove = false;
            
            m_bMutiTouch = false;
            
            if(m_dScale>1.4)
            {
                zoomOut();
                if(m_pControlLayer)
                {
                    ((ControlLayer*)m_pControlLayer)->updateScale();
                }
                
            }
            else if(m_dScale<0.6)
            {
                zoomIn();
                if(m_pControlLayer)
                {
                    ((ControlLayer*)m_pControlLayer)->updateScale();
                }
                
            }
            else
            {
                CCObject* child;
                int nIndex = 0;
                
                CCARRAY_FOREACH(getChildren(), child)
                {
                    CCSprite* pNode = (CCSprite*) child;
                    if (pNode && pNode->getTag() != 0)
                    {                                               
                        CCActionInterval* actionTo = CCMoveTo::create(0.1, m_spritePnt[nIndex++]);
                        CCActionInterval* ScaleTo = CCScaleTo::create(0.1, 1);
                        
                        pNode->runAction(CCSpawn::create( actionTo, ScaleTo, NULL));
                    }
                    
                }
            }
            m_offsetX = m_offsetY = 0.0;
            m_dScale = 1.0;
            m_spritePnt.clear();
        }
        return;
    }
    
	m_Map.m_bInMove = false;
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->getLocation();    

	float nMoveX = m_Map.getGlobalOffset().x + touchLocation.x - m_tBeginPos.x;
	float nMoveY = m_Map.getGlobalOffset().y - (touchLocation.y - m_tBeginPos.y);

	m_Map.setGlobalOffset(CCPoint(nMoveX,nMoveY));
	m_Map.quickHack(true);
	//updateScreen();
	m_tBeginPos = touchLocation;
}

void MapControl::setCenter( double lat, double lon, int zoom )
{
	double latFix = lat + m_GPSOffset.y*pow(10.0, -5);
	double lonFix = lon + m_GPSOffset.x*pow(10.0, -5);
	CCPoint p = GeoUtils::toTileXY(latFix, lonFix, zoom);
	CCPoint off = GeoUtils::getPixelOffsetInTile(latFix, lonFix, zoom);

	goTo((int) p.x, (int) p.y, zoom, (int) off.x, (int) off.y);
}

CCPoint MapControl::getGpsOffset()
{
    return  m_GPSOffset;
}

void MapControl::goTo( int x, int y, int z, int offsetX, int offsetY )
{
	m_Map.goTo(x, y, z, offsetX, offsetY);

	updateScreen();
}

void MapControl::updateScreen()
{
	this->removeAllChildrenWithCleanup(true);
    if(m_pSprite !=  NULL)
    {
        addChild(m_pSprite,0,0);
    }
    

	for (int i = 2; i < m_Map.m_nTotalCellsX+2; i++) {
		for (int j = 2; j < m_Map.m_nTotalCellsY+2; j++) {
			if ((i > 1 && i < m_Map.m_nTotalCellsX+2) && ((j > 1 && j < m_Map.m_nTotalCellsY+2))) {
				CCImage* pData =  m_Map.getCell(i - 2, j - 2);
				if (pData != NULL && pData->getData() != NULL) {
					m_bIsNew = false;

					CCTexture2D* pTexture = new CCTexture2D();
					pTexture->initWithImage(pData);

					CCSprite* pSprite = new CCSprite();
					pSprite->init();
					pSprite->initWithTexture(pTexture);
					CCPoint point((i - 2) * GeoUtils::TILE_SIZE + m_Map.getGlobalOffset().x+ GeoUtils::TILE_SIZE/2,m_Map.getHeight()-((j - 2) * GeoUtils::TILE_SIZE + m_Map.getGlobalOffset().y+ GeoUtils::TILE_SIZE/2));
					pSprite->setPosition(point);
					//pSprite->autorelease();
					addChild(pSprite,1,i+j);

					pTexture->release();
					pSprite->release();

					//m_Map.setBitmap((CCImage*)NULL,i - 2, j - 2);
				}
			}
// 			else {
// 				if (pmap.scaleFactor == 1) {
// 					cs.drawBitmap(CELL_BACKGROUND, 
// 						(i - 2) * TILE_SIZE + pmap.getGlobalOffset().x, 
// 						(j - 2) * TILE_SIZE + pmap.getGlobalOffset().y, paint);
// 				} else {
// 					cs.drawBitmap(EMPTY_BACKGROUND, 
// 						(i - 2) * TILE_SIZE + pmap.getGlobalOffset().x, 
// 						(j - 2) * TILE_SIZE + pmap.getGlobalOffset().y, paint);
// 				}
// 			}
		}
	}
}

void MapControl::zoomOut()
{
	//this->setScale(2);
	m_Map.zoomS(2);
}

void MapControl::zoomIn()
{
	//this->setScale(0.5);
	m_Map.zoomS(0.5);
}

PhysicMap* MapControl::getMap()
{
	return &m_Map;
}

int MapControl::getZoomLevel()
{
	return m_Map.getZoomLevel();
}
