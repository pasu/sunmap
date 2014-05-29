#ifndef __COCOS2D_LayerSwitcher_H__
#define __COCOS2D_LayerSwitcher_H__

#include "cocos2d.h"
#include "MapControl.h"

USING_NS_CC;

class LayerSwitcher : public CCLayer
{
public:
	LayerSwitcher(CCScene* pScene,MapControl* pControl);
	~LayerSwitcher();

	void menuCallback(CCObject * pSender);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
private:
	CCScene* m_pScene;
	MapControl* m_pControl;
	CCPoint m_tBeginPos;
	CCMenu* m_pItemMenu;
};

#endif