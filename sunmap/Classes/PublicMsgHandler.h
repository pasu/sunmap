#ifndef __COCOS2D_PublicMsgHandler_H__
#define __COCOS2D_PublicMsgHandler_H__

#include "cocos2d.h"
#include "RawTile.h"

USING_NS_CC;

class PublicMsgHandler : public cocos2d::CCNode
{
public:
	PublicMsgHandler();
	~PublicMsgHandler();

	virtual void update(float fDelta);
	void addEvent(RawTile* pTile,CCImage* pData);

	void attach(void* pMap);

private:
	 std::vector<RawTile*> m_pTile;
	 std::vector<CCImage*> m_pData;
	 void* m_pMap;

};

#endif