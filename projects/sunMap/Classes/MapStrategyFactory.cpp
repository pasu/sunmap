#include "MapStrategyFactory.h"
#include "GoogleVectorMapStrategy.h"
#include "GoogleSatelliteMapStrategy.h"
#include "GoogleTerrainMapStrategy.h"
#include "OpenStreetMapStrategy.h"
#include "TiandituMapStrategy.h"
#include "BingMapStrategy.h"
#include "SuperMapCloudMapStrategy.h"
#include "AMapStrategy.h"
#include "ArcGisMapStrategy.h"

int MapStrategyFactory::GOOGLE_VECTOR = 0;
int MapStrategyFactory::GOOGLE_SATELLITE  = 1;
int MapStrategyFactory::GOOGLE_TERRAIN= 2;
int MapStrategyFactory::OPENSTREET_VECTOR= 3;
int MapStrategyFactory::TIANDITU_VECTOR= 4;
int MapStrategyFactory::BING_VECTOR= 5;
//int MapStrategyFactory::SMCLOUD_VECTOR= 7;
int MapStrategyFactory::AMAP_VECTOR= 6;
int MapStrategyFactory::ArcMAP_VECTOR= 7;

std::map<int,MapStrategy*> MapStrategyFactory::m_mapStrategy;

MapStrategy* MapStrategyFactory::getStrategy( int nSourceId )
{
	MapStrategy* pMapStrategy = NULL;
	std::map<int,MapStrategy*>::iterator l_it;

	l_it=MapStrategyFactory::m_mapStrategy.find(nSourceId);
	if(l_it!=MapStrategyFactory::m_mapStrategy.end ())
	{		
		pMapStrategy = l_it->second;
	}
	return pMapStrategy;
}

MapStrategyFactory::MapStrategyFactory()
{

}

MapStrategyFactory::~MapStrategyFactory()
{
	std::map<int,MapStrategy*>::iterator it = MapStrategyFactory::m_mapStrategy.begin();   
	for(;it!=MapStrategyFactory::m_mapStrategy.end();++it)
	{
		MapStrategy* pMapStrategy = it->second;
		delete pMapStrategy;
		pMapStrategy = NULL;
	}
}

void MapStrategyFactory::initMapStrategy()
{
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::GOOGLE_VECTOR,new GoogleVectorMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::GOOGLE_SATELLITE,new GoogleSatelliteMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::GOOGLE_TERRAIN,new GoogleTerrainMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::OPENSTREET_VECTOR,new OpenStreetMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::TIANDITU_VECTOR,new TiandituMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::BING_VECTOR,new BingMapStrategy()));
//	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::SMCLOUD_VECTOR,new SuperMapCloudMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::AMAP_VECTOR,new AMapStrategy()));
	MapStrategyFactory::m_mapStrategy.insert(std::map<int,MapStrategy*>::value_type(MapStrategyFactory::ArcMAP_VECTOR,new ArcGisMapStrategy()));
}

