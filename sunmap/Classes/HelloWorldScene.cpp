#include "HelloWorldScene.h"
#include "MapControl.h"
#include "MapStrategyFactory.h"

using namespace cocos2d;

MapControl* pLayer = NULL;
CCScene* HelloWorld::scene(PublicMsgHandler* pHandle)
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

		MapStrategyFactory::initMapStrategy();
        // 'layer' is an autorelease object
		RawTile tile(106,54,13,-1);
		pLayer = new MapControl(tile);
		pHandle->attach((void*)pLayer);
		pLayer->getMap()->m_pHandler = pHandle;
		pLayer->getMap()->getTileResolver()->setMapSourceId(6);
		pLayer->scheduleUpdate();
        CC_BREAK_IF(! pLayer);

        // add layer as a child to scene
        scene->addChild(pLayer);
		pLayer->setCenter(39.54,116.23,12);

		HelloWorld *layer = HelloWorld::create();
		scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
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
            menu_selector(HelloWorld::zoomIn));
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
			menu_selector(HelloWorld::zoomOut));
		CC_BREAK_IF(! pZoomOut);

		// Place the menu item bottom-right conner.
		pZoomOut->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 25, 25));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu2 = CCMenu::create(pZoomOut, NULL);
		pMenu2->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu2);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu2,2);
        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::zoomIn(CCObject* pSender)
{
	pLayer->zoomIn();
}

void HelloWorld::zoomOut(CCObject* pSender)
{
	pLayer->zoomOut();
}

