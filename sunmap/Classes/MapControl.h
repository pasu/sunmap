#ifndef __COCOS2D_MapControl_H__
#define __COCOS2D_MapControl_H__

#include "cocos2d.h"
#include "PhysicMap.h"
#include "RawTile.h"

USING_NS_CC;

class MapControl :  public CCLayer
{
public:
	MapControl();
	MapControl(const RawTile& tile);
	~MapControl();

	//virtual void update(float delta);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	void setCenter(double lat, double lon, int zoom);
	void goTo(int x, int y, int z, int offsetX, int offsetY);

	void updateScreen();

	void zoomOut();
	void zoomIn();

	PhysicMap* getMap();

public:
	enum CELL_Type
	{
		CELL_BACKGROUND = 0
	};
private:
	PhysicMap m_Map;
	CCPoint m_GPSOffset;
	bool m_bIsNew;
	CCPoint m_tBeginPos;
	cc_timeval m_now; 
};

#endif