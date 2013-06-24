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

void VectorLayer::AddFeatureLayer(Feature* pFeature)
{
    m_Features.push_back(pFeature);
}

void VectorLayer::AddFeatureLayers(Feature* pFeature,int length)
{
    for (int i=0; i<length; i++) {
        m_Features.push_back(pFeature++);
    }
}

