#ifndef __COCOS2D_RawTile_H__
#define __COCOS2D_RawTile_H__

#include "cocos2d.h"

struct Cell_Data
{
	cocos2d::CCImage* pImage;
// 	char* data;
// 	int length;
};

class RawTile
{
public:
	int x,y,z,s;

public:
	RawTile();
	~RawTile();

	RawTile(int x, int y,int z,int s);
	RawTile(const RawTile& other);

	cocos2d::CCString toString();
};

#endif