#include "Layer/LocalTileManager.h"
#include "FileOperation.h"
#include "platform/CCFileUtils.h"
#include "SunFile.h"

template<> LocalSotrageManager* Singleton<LocalSotrageManager>::msSingleton = NULL;
LocalSotrageManager* LocalSotrageManager::GetSingletonPtr()
{
	if(msSingleton == NULL)
	{
		msSingleton = new LocalSotrageManager();
	} 
	return msSingleton;
}
LocalSotrageManager& LocalSotrageManager::GetSingleton()
{  
	if(msSingleton == NULL)
	{
		msSingleton = new LocalSotrageManager();
	} 
	return ( *msSingleton );  
}

LocalSotrageManager::LocalSotrageManager()
{

}

LocalSotrageManager::~LocalSotrageManager()
{

}

void LocalSotrageManager::AddLocalConfig( CCString strPath,CCString strName )
{
	CCString strFile;
	strFile.initWithFormat("%s//%s",strPath.getCString(),strName.getCString());
	//读取的字节数，读取失败则为0
	unsigned long len = 0;
	//读取的内容	
	unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(strFile.getCString(), "r", &len);

	sunMap::CacheInfo* pCache = new sunMap::CacheInfo;

	pCache->ParseFromArray(data,len);
	CCString strMapId = pCache->mapid();
	int nID = strMapId.intValue();

	std::map<int ,std::string >::iterator l_it;
	l_it=m_LocalMap.find(nID);
	if(l_it==m_LocalMap.end())
	{
		m_LocalMap.insert(std::make_pair(nID,strPath.getCString()));
	}
}

CCImage* LocalSotrageManager::getTile( RawTile* pTile )
{
	int nID = (pTile->s);
	std::map<int ,std::string >::iterator l_it;
	l_it=m_LocalMap.find(nID);
	if(l_it==m_LocalMap.end())
	{
		return NULL;
	}
	else
	{
		std::string strPath = l_it->second;
		
		CCString strFilePath;
		strFilePath.initWithFormat("%s//%d//%d//%d.%s",strPath.c_str(),
			pTile->z,pTile->y,pTile->x,"png");

		if (!SunFile::IsExist(strFilePath.getCString(),true))
		{
			return NULL;
		}

		CCImage* pImage = new CCImage;
		pImage->initWithImageFile(strFilePath.getCString());
		return pImage;
	}
}
