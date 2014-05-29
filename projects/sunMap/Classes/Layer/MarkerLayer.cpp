#include "MarkerLayer.h"
#include "GeoUtils.h"

MarkerLayer::MarkerLayer(MapControl* pControl)
{
    m_pControl = pControl;
    setTouchEnabled(true);
}

MarkerLayer::~MarkerLayer()
{
    
}

void MarkerLayer::AddMarker(CCString strImage,CCPoint pnt)
{    
    CCMenuItemImage *pMarkerItem = CCMenuItemImage::create(strImage.getCString(),strImage.getCString());
    
    // Place the menu item bottom-right conner.
    //pMarkerItem->setPosition(ccp(100, 35));
    
    // Create a menu with the "close" menu item, it's an auto release object.
    CCMenu* pMarkerMenu = CCMenu::create(pMarkerItem, NULL);
    pMarkerMenu->setPosition(CCPointZero);
    pMarkerMenu->retain();
    m_markers.push_back(pMarkerMenu);
    m_pnt.push_back(pnt);
    
    if (m_pControl) {
        double lat = pnt.y;
        double lon = pnt.x;
        CCPoint offsetGps = m_pControl->getGpsOffset();
        int zoom = 17 - m_pControl->getZoomLevel();
        double latFix = lat + offsetGps.y*pow(10.0, -5);
        double lonFix = lon + offsetGps.x*pow(10.0, -5);
        RawTile dTile = m_pControl->getMap()->m_defTile;
        CCPoint pntOffset = m_pControl->getMap()->getGlobalOffset();
        
        CCPoint pntTopLeft,pntRightBottom,pntMarker;
        pntTopLeft.x= dTile.x*256 - pntOffset.x;
        pntTopLeft.y= dTile.y*256 - pntOffset.y;
        pntRightBottom.x = pntTopLeft.x + m_pControl->getMap()->getWidth();
        pntRightBottom.y = pntTopLeft.y + m_pControl->getMap()->getHeight();
        
        CCPoint p = GeoUtils::toTileXY(latFix, lonFix, zoom);
        CCPoint off = GeoUtils::getPixelOffsetInTile(latFix, lonFix, zoom);
        
        pntMarker.x = (p.x)*256 + off.x;
        pntMarker.y = (p.y)*256 + off.y;
        if (pntMarker.x>pntTopLeft.x && pntMarker.y>pntTopLeft.y && pntMarker.x<pntRightBottom.x && pntMarker.y<pntRightBottom.y) {
            CCPoint pp(pntMarker.x-pntTopLeft.x, m_pControl->getMap()->getHeight()-(pntMarker.y-pntTopLeft.y));
            pMarkerMenu->setPosition(pp);
            addChild(pMarkerMenu);
        }        
    }

}

void MarkerLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    m_tBeginPos = touch->getLocation();
}
void MarkerLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    CCPoint touchLocation = touch->getLocation();
    
    
    CCObject* child;
    
    CCARRAY_FOREACH(getChildren(), child)
    {
        CCMenu* pNode = (CCMenu*) child;
        
        {
            CCActionInterval* actionBy = CCMoveBy::create(0.1, CCPoint(touchLocation.x - m_tBeginPos.x,touchLocation.y - m_tBeginPos.y));            
            
            pNode->runAction(actionBy);
        }
    }
    
    
    m_tBeginPos = touchLocation;
}
void MarkerLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    removeAllChildren();
    
    int nCount = m_markers.size();
    for (int i = 0; i < nCount; ++i)
    {
        CCPoint pnt = m_pnt[i];
        
        if (m_pControl) {
            double lat = pnt.y;
            double lon = pnt.x;
            CCPoint offsetGps = m_pControl->getGpsOffset();
            int zoom = 17 - m_pControl->getZoomLevel();
            double latFix = lat + offsetGps.y*pow(10.0, -5);
            double lonFix = lon + offsetGps.x*pow(10.0, -5);
            RawTile dTile = m_pControl->getMap()->m_defTile;
            CCPoint pntOffset = m_pControl->getMap()->getGlobalOffset();
            
            CCPoint pntTopLeft,pntRightBottom,pntMarker;
            pntTopLeft.x= dTile.x*256 - pntOffset.x;
            pntTopLeft.y= dTile.y*256 - pntOffset.y;
            pntRightBottom.x = pntTopLeft.x + m_pControl->getMap()->getWidth();
            pntRightBottom.y = pntTopLeft.y + m_pControl->getMap()->getHeight();
            
            CCPoint p = GeoUtils::toTileXY(latFix, lonFix, zoom);
            CCPoint off = GeoUtils::getPixelOffsetInTile(latFix, lonFix, zoom);
            
            pntMarker.x = (p.x)*256 + off.x;
            pntMarker.y = (p.y)*256 + off.y;
            if (pntMarker.x>pntTopLeft.x && pntMarker.y>pntTopLeft.y && pntMarker.x<pntRightBottom.x && pntMarker.y<pntRightBottom.y) {
                CCPoint pp(pntMarker.x-pntTopLeft.x, m_pControl->getMap()->getHeight()-(pntMarker.y-pntTopLeft.y));
                CCMenu* pMarkerMenu = m_markers[i];
                pMarkerMenu->setPosition(pp);
                addChild(pMarkerMenu);
            }        
        }
    }
}
