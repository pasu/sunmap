#include "HelloWorldScene.h"
#include "MapControl.h"
#include "MapStrategyFactory.h"
#include "ControlLayer.h"
#include "LayerSwitcher.h"
#include "MarkerLayer.h"
#include "VectorLayer.h"

#include <geos/geom/GeometryFactory.h>
#include "geos/geom/CoordinateArraySequenceFactory.h"
#include "geos/geom/Point.h"
#include "geos/geom/Polygon.h"
#include "Feature.h"
#include <geos/geom.h>

#include "CacheMaker/CacheMaker.h"
#include "Layer/LocalTileManager.h"
using namespace cocos2d;
using namespace geos;
using namespace geos::geom;

CCScene* HelloWorld::scene(PublicMsgHandler* pHandle)
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		MapStrategyFactory::initMapStrategy();

		////////////////////////////////////////////
		if (true)
		{
			CCPoint pnt1,pnt2;
			pnt1.x = 126.54;
			pnt1.y = 49;

			pnt2.x = 136;
			pnt2.y = 39;

			std::vector<int> nZoomArray;
			nZoomArray.push_back(10);
			nZoomArray.push_back(11);
			nZoomArray.push_back(12);
			nZoomArray.push_back(13);
			nZoomArray.push_back(14);
			CacheMaker make("Test",pnt1,pnt2,nZoomArray,0);

			make.setSavePath("E://Test");
			make.setImageType("tiff");
			make.generate();
		}
		
		LocalSotrageManager::GetSingletonPtr()->AddLocalConfig("E://Test","Test.config");
		////////////////////////////////////////////

		// 'layer' is an autorelease object
		RawTile tile(106,54,13,-1);
		MapControl* pLayer = new MapControl(tile);
		pLayer->setMapType(0);

		pHandle->attach((void*)pLayer);
		pLayer->getMap()->m_pHandler = pHandle;
		pLayer->getMap()->getTileResolver()->setMapSourceId(0);
		CC_BREAK_IF(! pLayer);

		// add layer as a child to scene
		scene->addChild(pLayer);
		pLayer->setCenter(39.54,116.23,12);

		LayerSwitcher* pLayerSwitcher = new LayerSwitcher(scene,pLayer);
		scene->addChild(pLayerSwitcher);
		pLayerSwitcher->setZOrder(-100);

		ControlLayer* pControlLayer = new ControlLayer(pLayerSwitcher,pLayer);
		scene->addChild(pControlLayer);
		pLayer->attachControlLayer(pControlLayer);

		MarkerLayer* pMarkerLayer = new MarkerLayer(pLayer);
		scene->addChild(pMarkerLayer);
		pMarkerLayer->AddMarker("person.png", CCPoint(116.23,39.54));
		pMarkerLayer->AddMarker("person.png", CCPoint(151.71,-33.55));

		VectorLayer* pV = new VectorLayer(pLayer);
		scene->addChild(pV);


		CoordinateArraySequence * cs = new CoordinateArraySequence();
		cs->add(Coordinate(116.23,39.54,0));
		cs->add(Coordinate(116.23,42.54,0));
		cs->add(Coordinate(118.23,42.54,0));
		cs->add(Coordinate(118.23,39.54,0));
		cs->add(Coordinate(116.23,39.54,0));

		GeometryFactory factory;
		LinearRing* viewBounds = factory.createLinearRing(cs);
		geos::geom::Polygon* p = GeometryFactory::getDefaultInstance()->createPolygon(viewBounds,NULL);

		int nCount = p->getNumPoints();
		nCount = ((Geometry*)viewBounds)->clone()->getNumPoints();
		//oordinateSequence* pCs = ((Geometry*)viewBounds)->getCoordinates();

		pV->AddFeatureLayer(new Feature(p));

		//const GeometryFactory* pF = geos::geom::GeometryFactory::getDefaultInstance();

		//Coordinate p(110,500);
		//Point* p1 = pF->createPoint(p);
		//pF->createLineString();        
		//p1->getX();
	} while (0);

	// return the scene
	return scene;
}


