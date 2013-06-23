//
//  VectorLayer.h
//  sunmap
//
//  Created by 陆 国伟 on 13-6-23.
//
//

#ifndef __sunmap__VectorLayer__
#define __sunmap__VectorLayer__

#include "cocos2d.h"
#include "MapControl.h"

USING_NS_CC;

class VectorLayer : public CCLayer
{
public:
	VectorLayer(MapControl* pControl);
    ~VectorLayer();
    
    virtual void draw();

    //void AddFeature(CCString strImage,CCPoint pnt);
    
    //virtual void update(float delta);
    
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	//virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    
private:
    std::vector<CCMenu*> m_markers;
    std::vector<CCPoint> m_pnt;
    MapControl* m_pControl;
    
    CCPoint m_tBeginPos;
    
};


#endif /* defined(__sunmap__VectorLayer__) */
