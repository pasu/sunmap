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
	CacheMaker(CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray,int nMapStrategy);
	~CacheMaker();

	void setMaxThread(int nThread);
	int getMaxThread();

	int getMapStrategyId();
	MapStrategy* getMapStrategy();

	void setSavePath(const char* strPath);
	const char* getSavePath();

	void setImageType(int nType);
	const char* getImageType();

	bool build(CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray,int nMapStrategy);

	void clear();

	void generate();

	void saveConfig();

	//bool createDirectory(const char *path);

private:

	std::vector<int> m_ZoomArray;
	std::vector<int> m_NumberPerZoom;
	std::vector<RawTile*> m_TileArray;
	int m_nTotalCount;
	int m_nMapStrategy;

	MapStrategy* m_pMapStrategy;

	int m_nMaxThread;

	std::string m_strPath;
	std::string m_strImageType;
public:
	int m_nThreadCount;
};

#endif