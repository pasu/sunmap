//
//  VectorLayer.cpp
//  sunmap
//
//  Created by 陆 国伟 on 13-6-23.
//
//

#include "VectorLayer.h"
#include "VisibleRect.h"

VectorLayer::VectorLayer(MapControl* pControl)
{
    m_pControl = pControl;
}

VectorLayer::~VectorLayer()
{
    
}

void VectorLayer::draw()
{
    static bool b = false;
    if(b)
    {
        glLineWidth( 5.0f );
        ccDrawColor4B(255,0,0,255);
        ccDrawLine( VisibleRect::leftTop(), VisibleRect::rightBottom() );

        return;
    }
    CHECK_GL_ERROR_DEBUG();
    
	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased
    //	glEnable(GL_LINE_SMOOTH);
    ccDrawLine( VisibleRect::leftBottom(), VisibleRect::rightTop() );
    b = true;

}

void VectorLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    m_tBeginPos = touch->getLocation();
}
void VectorLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    CCPoint touchLocation = touch->getLocation();
    
    CCActionInterval* actionBy = CCMoveBy::create(0.1, CCPoint(touchLocation.x - m_tBeginPos.x,touchLocation.y - m_tBeginPos.y));
    
    this->runAction(actionBy);
    /*
    CCObject* child;
    
    CCARRAY_FOREACH(getChildren(), child)
    {
        CCMenu* pNode = (CCMenu*) child;
        
        {
            CCActionInterval* actionBy = CCMoveBy::create(0.1, CCPoint(touchLocation.x - m_tBeginPos.x,touchLocation.y - m_tBeginPos.y));
            
            pNode->runAction(actionBy);
        }
    }
     */
    
    
    
    m_tBeginPos = touchLocation;
}

