#ifndef __COCOS2D_LocalSotrageManager_H__
#define __COCOS2D_LocalSotrageManager_H__

#include "cocos2d.h"
#include "Core/Singleton.h"
#include "Core/CacheInfo.pb.h"
#include "RawTile.h"

USING_NS_CC;

class LocalSotrageManager :  public Singleton<LocalSotrageManager>
{
public:
	LocalSotrageManager();
	~LocalSotrageManager();

	static LocalSotrageManager* GetSingletonPtr();
	static LocalSotrageManager& GetSingleton();

	void AddLocalConfig(CCString strPath,CCString strName);
	CCImage* getTile(RawTile* pTile);

private:
	std::vector<std::string> m_CachePathArray;
	std::map<int,std::string> m_LocalMap;
	
};

#endif