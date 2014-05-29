#ifndef __COCOS2D_CacheMaker_H__
#define __COCOS2D_CacheMaker_H__

#include "TileResolver.h"
#include "cocos2d.h"

USING_NS_CC;

//Éú³É»º´æÍßÆ¬Àà
class CacheMaker
{
public:
	CacheMaker();
	CacheMaker(std::string strName,CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray,int nMapStrategy);
	~CacheMaker();

	void setMaxThread(int nThread);
	int getMaxThread();

	int getMapStrategyId();
	MapStrategy* getMapStrategy();

	void setSavePath(const char* strPath);
	const char* getSavePath();

	void setImageType(std::string nType);
	const char* getImageType();

	bool build(CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray,int nMapStrategy);

	void clear();

	void generate();

	void saveConfig();

	void update(RawTile* pTile);

	//bool createDirectory(const char *path);

private:

	std::vector<int> m_ZoomArray;
	std::vector<int> m_NumberPerZoom;
	std::vector<RawTile*> m_TileArray;
	int m_nTotalCount;
	int m_nNeedTodown;
	int m_nMapStrategy;

	MapStrategy* m_pMapStrategy;

	int m_nMaxThread;

	std::string m_strPath;
	std::string m_strImageType;

	int m_nThreadCount;
	std::vector<RawTile> m_failTile;
	std::string m_strName;

	CCPoint m_pntLeftTop;
	CCPoint m_pntRightBottom;
	int m_nTime;
};

#endif