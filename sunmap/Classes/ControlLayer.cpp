#include "ControlLayer.h"


ControlLayer::ControlLayer( LayerSwitcher* pLayerSwitcher,MapControl* pControl )
{
	m_pLayerSwitcher = pLayerSwitcher;
	m_pControl = pControl;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pZoomIn = CCMenuItemImage::create(
			"zoomin.png",
			"zoomin.png",
			this,
			menu_selector(ControlLayer::zoomIn));
		CC_BREAK_IF(! pZoomIn);

		// Place the menu item bottom-right conner.
		pZoomIn->setPosition(ccp(25, 25));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pZoomIn, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pZoomOut = CCMenuItemImage::create(
			"zoomout.png",
			"zoomout.png",
			this,
			menu_selector(ControlLayer::zoomOut));
		CC_BREAK_IF(! pZoomOut);

		// Place the menu item bottom-right conner.
		pZoomOut->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 25, 25));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu2 = CCMenu::create(pZoomOut, NULL);
		pMenu2->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu2);

		this->addChild(pMenu2,2);

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pLayerSwitch = CCMenuItemImage::create(
			"r_overlays.png",
			"r_overlays.png",
			this,
			menu_selector(ControlLayer::layerswitch));
		CC_BREAK_IF(! pLayerSwitch);

		// Place the menu item bottom-right conner.
		pLayerSwitch->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height-25));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu3 = CCMenu::create(pLayerSwitch, NULL);
		pMenu3->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu3);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu3, 3);
	} while (0);
}

void ControlLayer::zoomIn(CCObject* pSender)
{
	m_pControl->zoomIn();
}

void ControlLayer::zoomOut(CCObject* pSender)
{
	m_pControl->zoomOut();
}

void ControlLayer::layerswitch( CCObject* pSender )
{
	m_pLayerSwitcher->setZOrder(100);
	setTouchEnabled(true);
}
