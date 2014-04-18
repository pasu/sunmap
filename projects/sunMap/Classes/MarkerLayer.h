#ifndef __COCOS2D_MarkerLayer_H__
#define __COCOS2D_MarkerLayer_H__

#include "cocos2d.h"
#include "MapControl.h"

USING_NS_CC;

class MarkerLayer : public CCLayer
{
public:
	MarkerLayer(MapControl* pControl);
    ~MarkerLayer();
    void AddMarker(CCString strImage,CCPoint pnt);
    
    //virtual void update(float delta);
    
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

    
private:
    std::vector<CCMenu*> m_markers;
    std::vector<CCPoint> m_pnt;
    MapControl* m_pControl;
    
    CCPoint m_tBeginPos;
    
};



#endif