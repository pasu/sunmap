#ifndef __COCOS2D_MapStrategyFactory_H__
#define __COCOS2D_MapStrategyFactory_H__

#include "cocos2d.h"
#include "MapStrategy.h"

class MapStrategyFactory
{
public:
	static int GOOGLE_VECTOR;
	static int GOOGLE_SATELLITE;
	static int GOOGLE_TERRAIN;
	static int OPENSTREET_VECTOR;
	static int TIANDITU_VECTOR;
	static int BING_VECTOR;
	static int SMCLOUD_VECTOR;
	static int AMAP_VECTOR;
	static int ArcMAP_VECTOR;
	static int BMAP_VECTOR;

	MapStrategyFactory();
	~MapStrategyFactory();

	static void initMapStrategy();
	static std::map<int,MapStrategy*> m_mapStrategy;

	static MapStrategy* getStrategy(int nSourceId) ;
};

#endif
