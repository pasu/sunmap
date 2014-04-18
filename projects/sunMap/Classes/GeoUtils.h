#ifndef __COCOS2D_GeoUtils_H__
#define __COCOS2D_GeoUtils_H__

#include "cocos2d.h"
#include "RawTile.h"

class GeoUtils
{
public:
	static int TILE_SIZE;

	static double const PI;

	static cocos2d::CCPoint getLatLong(int x, int y, int zoom);

	static bool isValid(RawTile tile);

	static cocos2d::CCPoint toNormalisedPixelCoords(double lat, double lng);

	static cocos2d::CCPoint toZoomedPixelCoords(double lat, double lng, int zoom);

	static cocos2d::CCPoint getPixelOffsetInTile(double lat, double lng, int zoom);

	static cocos2d::CCPoint toTileXY(double lat, double lng, int zoom);

	static cocos2d::CCString TileXYToQuadKey(int tileX, int tileY, int levelOfDetail);

	static int getZoomLevel(double x);
};

#endif