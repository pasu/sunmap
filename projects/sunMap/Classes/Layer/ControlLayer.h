#ifndef __COCOS2D_ControlLayer_H__
#define __COCOS2D_ControlLayer_H__

#include "cocos2d.h"
#include "MapControl.h"
#include "LayerSwitcher.h"

USING_NS_CC;

class ControlLayer : public CCLayer
{
public:
	ControlLayer(LayerSwitcher* pLayerSwitcher,MapControl* pControl);

	// a selector callback
	void zoomIn(CCObject* pSender);
	void zoomOut(CCObject* pSender);
	void layerswitch(CCObject* pSender);
    void updateScale();
private:
	LayerSwitcher* m_pLayerSwitcher;
	MapControl* m_pControl;
	std::vector<CCMenu*> m_ScaleList;
};

#endif