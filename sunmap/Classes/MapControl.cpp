#include "MapControl.h"
#include "GeoUtils.h"

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
}

MapControl::~MapControl()
{

}

void MapControl::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	m_Map.m_bInMove = false;
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	m_tBeginPos = touch->getLocation();    
	
	CCTime::gettimeofdayCocos2d(&m_now, NULL);
	if(m_now.tv_sec*1000+ m_now.tv_usec/1000 - m_firstEnd.tv_sec*1000 - m_firstEnd.tv_usec/1000<200)
	{
		zoomOut();
	}
	m_firstEnd.tv_sec = m_now.tv_sec;
	m_firstEnd.tv_usec = m_now.tv_usec;
}

void MapControl::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
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

void MapControl::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
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

// void MapControl::update( float delta )
// {
// 	if(m_Map.m_update == true)
// 	{
// 		m_Map.m_update = false;
// 		updateScreen();
// 	}
// }

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
