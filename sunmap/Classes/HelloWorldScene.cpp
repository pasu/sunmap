#include "HelloWorldScene.h"
#include "MapControl.h"
#include "MapStrategyFactory.h"
#include "ControlLayer.h"

using namespace cocos2d;

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
		MapControl* pLayer = new MapControl(tile);
		pHandle->attach((void*)pLayer);
		pLayer->getMap()->m_pHandler = pHandle;
		pLayer->getMap()->getTileResolver()->setMapSourceId(0);
		pLayer->scheduleUpdate();
        CC_BREAK_IF(! pLayer);

        // add layer as a child to scene
        scene->addChild(pLayer);
		pLayer->setCenter(39.54,116.23,12);

		ControlLayer* pControlLayer = new ControlLayer(pLayer);
		scene->addChild(pControlLayer);
    } while (0);

    // return the scene
    return scene;
}


