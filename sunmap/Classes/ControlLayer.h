#ifndef __COCOS2D_ControlLayer_H__
#define __COCOS2D_ControlLayer_H__

#include "cocos2d.h"
#include "MapControl.h"

USING_NS_CC;

class ControlLayer : public CCLayer
{
public:
	ControlLayer(MapControl* pControl);

	// a selector callback
	void zoomIn(CCObject* pSender);
	void zoomOut(CCObject* pSender);
private:
	MapControl* m_pControl;
};

#endif