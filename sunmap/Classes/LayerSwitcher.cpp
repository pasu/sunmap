#include "LayerSwitcher.h"
#include "MapStrategyFactory.h"
#include "VisibleRect.h"

#define LINE_SPACE          40

static CCPoint s_tCurPos = CCPointZero;

LayerSwitcher::LayerSwitcher( CCScene* pScene,MapControl* pControl ): m_tBeginPos(CCPointZero)
{
	m_pScene = pScene;
	m_pControl = pControl;

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSprite = CCSprite::create("combox.png");

	float nHeight = (MapStrategyFactory::m_mapStrategy.size() + 2) * LINE_SPACE;
	pSprite->setPosition(ccp(size.width/2,size.height-nHeight/2));

	float spx = pSprite->getTextureRect().getMaxX();
	float spy = pSprite->getTextureRect().getMaxY();

	pSprite->setScaleX(size.width/3/spx);
	pSprite->setScaleY(nHeight/spy);
	pSprite->setOpacity(180);

	addChild(pSprite);

	// add menu items for tests
	m_pItemMenu = CCMenu::create();
	for (int i = 0; i < MapStrategyFactory::m_mapStrategy.size()+1; ++i)
	{
		CCString strLabel = "Map Service Type:";
		if (i != 0)
		{
			strLabel = MapStrategyFactory::m_mapStrategy[i-1]->getDescription();
		}
		
		CCLabelTTF* label = CCLabelTTF::create(strLabel.getCString(), "Arial", 24);
		// #endif        
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(LayerSwitcher::menuCallback));

		m_pItemMenu->addChild(pMenuItem, i + 10000);
		pMenuItem->setPosition( ccp( VisibleRect::center().x, (VisibleRect::top().y - (i + 1) * LINE_SPACE) ));
	}

	m_pItemMenu->setContentSize(CCSizeMake(VisibleRect::getVisibleRect().size.width, (MapStrategyFactory::m_mapStrategy.size() + 1) * (LINE_SPACE)));
	m_pItemMenu->setPosition(s_tCurPos);
	addChild(m_pItemMenu);

	setTouchEnabled(true);
}

LayerSwitcher::~LayerSwitcher()
{

}

void LayerSwitcher::menuCallback( CCObject * pSender )
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder() - 10000;
	if (nIdx != 0)
	{
		m_pControl->getMap()->getTileResolver()->setMapSourceId(nIdx-1);

		setZOrder(-100);
	}
}


void LayerSwitcher::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	m_tBeginPos = touch->getLocation();    
}

void LayerSwitcher::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->getLocation();    
	float nMoveY = touchLocation.y - m_tBeginPos.y;

	CCPoint curPos  = m_pItemMenu->getPosition();
	CCPoint nextPos = ccp(curPos.x, curPos.y + nMoveY);

	if (nextPos.y < 0.0f)
	{
		m_pItemMenu->setPosition(CCPointZero);
		return;
	}

	if (nextPos.y > ((MapStrategyFactory::m_mapStrategy.size() + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
	{
		m_pItemMenu->setPosition(ccp(0, ((MapStrategyFactory::m_mapStrategy.size() + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
		return;
	}

	m_pItemMenu->setPosition(nextPos);
	m_tBeginPos = touchLocation;
	s_tCurPos   = nextPos;
}
