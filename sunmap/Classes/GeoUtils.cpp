#include "GeoUtils.h"


int GeoUtils::TILE_SIZE = 256;
const double GeoUtils::PI = 3.14159265;

cocos2d::CCPoint GeoUtils::getLatLong(int x, int y, int zoom)
{
	double lon = -180; // x
	double lonWidth = 360; // width 360

	// double lat = -90; // y
	// double latHeight = 180; // height 180
	double lat = -1;
	double latHeight = 2;

	int tilesAtThisZoom = 1 << (17 - zoom);
	lonWidth = 360.0 / tilesAtThisZoom;
	lon = -180 + (x * lonWidth);
	latHeight = -2.0 / tilesAtThisZoom;
	lat = 1 + (y * latHeight);

	// convert lat and latHeight to degrees in a transverse mercator projection
	// note that in fact the coordinates go from about -85 to +85 not -90 to 90!
	latHeight += lat;
	latHeight = (2* atan(exp(PI * latHeight))) - PI/2;
	latHeight *= (180 / PI);

	lat = (2 * atan(exp(PI * lat))) - (PI / 2);
	lat *= (180 / PI);

	latHeight -= lat;

	if (lonWidth < 0) {
		lon = lon + lonWidth;
		lonWidth = -lonWidth;
	}

	if (latHeight < 0) {
		lat = lat + latHeight;
		latHeight = -latHeight;
	}

	cocos2d::CCPoint point;
	point.x = (int) lat;
	point.y = (int) lon;

	return point;
}

bool GeoUtils::isValid( RawTile tile )
{
	int tileCount = (int) pow(2.0, (int)(17 - tile.z));
	return (tile.x>-1 && tile.x<tileCount && tile.y>-1 && tile.y<tileCount);
}

cocos2d::CCPoint GeoUtils::toZoomedPixelCoords( double lat, double lng, int zoom )
{
	cocos2d::CCPoint normalised = GeoUtils::toNormalisedPixelCoords(lat, lng);
	double scale = (1 << (17 - zoom)) * TILE_SIZE;

	return cocos2d::CCPoint((int) (normalised.x * scale), (int) (normalised.y * scale));
}

cocos2d::CCPoint GeoUtils::toNormalisedPixelCoords( double lat, double lng )
{
	if (lng > 180) {
		lng -= 360;
	}

	lng /= 360;
	lng += 0.5;

	lat = 0.5 - ((log(tan((PI / 4)
		+ ((0.5 * PI * lat) / 180))) / PI) / 2.0);

	return cocos2d::CCPoint(lng,lat);
}

cocos2d::CCPoint GeoUtils::getPixelOffsetInTile( double lat, double lng, int zoom )
{
	cocos2d::CCPoint pixelCoords = toZoomedPixelCoords(lat, lng, zoom);

	return cocos2d::CCPoint((int)(pixelCoords.x) % TILE_SIZE, (int)(pixelCoords.y) % TILE_SIZE);
}

cocos2d::CCPoint GeoUtils::toTileXY( double lat, double lng, int zoom )
{
	cocos2d::CCPoint normalised = toNormalisedPixelCoords(lat, lng);
	int scale = 1 << (17 - zoom);

	return cocos2d::CCPoint((int) (normalised.x * scale), (int) (normalised.y * scale));
}

cocos2d::CCString GeoUtils::TileXYToQuadKey( int tileX, int tileY, int levelOfDetail )
{
	//StringBuilder quadKey = new StringBuilder();
	//cocos2d::CCString* pStrTemp = NULL;
	std::string quadKey = "";
	for (int i = levelOfDetail; i > 0; i--) {
		char digit = '0';
		int mask = 1 << (i - 1);
		if ((tileX & mask) != 0) {
			digit++;
		}
		if ((tileY & mask) != 0) {
			digit++;
			digit++;
		}
		quadKey += digit;
	}
	return cocos2d::CCString(quadKey);
}

int GeoUtils::getZoomLevel( double x )
{
	if(x<1) x = 1/x;
	int counter = 1;
	while (x > 2) {
		counter++;
		x = x / 2;
	}
	if(x<1) return counter*-1;
	return counter;
}
