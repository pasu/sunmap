#include "RawTile.h"

RawTile::RawTile()
{
	x = y = z = s = 0;
}

RawTile::~RawTile()
{
	x = y = z = s = 0;
}

RawTile::RawTile( int x, int y,int z,int s):x(x),y(y),z(z),s(s)
{

}

RawTile::RawTile( const RawTile& other )
{
	x = other.x;
	y = other.y;
	z = other.z;
	s = other.s;
}

cocos2d::CCString RawTile::toString()
{
	cocos2d::CCString strPath;
	strPath.initWithFormat("%d/%d/%d/%d/",
							this->s,this->z,this->x,this->y);
	return strPath;
}

