#include "CacheMaker/CacheMaker.h"
#include "GeoUtils.h"
#include "MapStrategyFactory.h"

#include "curl/curl.h"
#include "TileResolver.h"
#ifdef WIN32
#include "pthread/pthread.h"
#else
#include <pthread.h>
#endif

#include "SunFile.h"
#include "Core/CacheInfo.pb.h"
#include "FileOperation.h"


static pthread_mutex_t cachemutex = PTHREAD_MUTEX_INITIALIZER;


#include "libtiff/tiffio.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;


void save_tiff_rgb(string output_filename,int nWidth,int nHeight,const char* pImageData) {
	TIFF *output_image;

	// Open the TIFF file
	if((output_image = TIFFOpen(output_filename.c_str(), "w")) == NULL){
		cerr << "Unable to write tif file: " << output_filename << endl;
	}

	// We need to set some values for basic tags before we can add any data
	TIFFSetField(output_image, TIFFTAG_IMAGEWIDTH, nWidth);
	TIFFSetField(output_image, TIFFTAG_IMAGELENGTH, nHeight);
	TIFFSetField(output_image, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(output_image, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(output_image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	TIFFSetField(output_image, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
	TIFFSetField(output_image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	// Write the information to the file
	TIFFWriteEncodedStrip(output_image, 0, (void* )pImageData, nWidth*nHeight* 3);

	// Close the file
	TIFFClose(output_image);
}
struct tileData
{
	RawTile* pRawTile;
	char nTryTime;
	CacheMaker* pMaker;

};

static size_t OnWriteImageData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{  
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);  
	if( NULL == str || NULL == buffer )  
	{  
		return -1;  
	}  

	char* pData = (char*)buffer;  
	str->append(pData, size * nmemb);  
	return nmemb;  
}  

void* downloadToSave(void* ppCurl)
{
	tileData* pData = (tileData*)ppCurl;

	pData->nTryTime--;
	pData->pRawTile;

	CCString strUrl = pData->pMaker->getMapStrategy()->getUrl(pData->pRawTile->x,pData->pRawTile->y,pData->pRawTile->z);

	std::string strResponse;

	CURLcode res;
	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.getCString());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.64 Safari/537.31");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteImageData);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

	res = curl_easy_perform(curl);

	int retcode = 0;  
	res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
	double length;
	res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 

	if (res==CURLE_OK && retcode == 200 && ((int)length)>0 && strResponse.size()>0)
	{
		CCImage* pImage = new CCImage;
		if(pImage->initWithImageData((void*)strResponse.c_str(),(int)length) == false)
		{
			delete pImage;
			pImage = NULL;

			curl_easy_cleanup(curl);

			if(pData->nTryTime>0)
			{
				pData->nTryTime--;
				pthread_t pid;
				pthread_create(&pid, NULL, downloadToSave, (void*)pData);
			}
			else
			{
				pthread_mutex_lock(&cachemutex);
				pData->pMaker->update(pData->pRawTile);
				pthread_mutex_unlock(&cachemutex );
				delete pData;
				pData = NULL;
			}
		}
		else
		{
			pthread_mutex_lock(&cachemutex);
			pData->pMaker->update(NULL);
			pthread_mutex_unlock(&cachemutex );

			curl_easy_cleanup(curl);

			//////////////////////////////////////////////////////////////////////////
			CCString strPath;
			std::string strRootPath = pData->pMaker->getSavePath();
			strPath.initWithFormat("%s//%d//%d//",strRootPath.c_str(),
				pData->pRawTile->z,pData->pRawTile->y);

			if (!SunFile::IsExist(strPath.getCString(),true))
			{
				SunFile::MkDirEx(strPath.getCString());
			}
			
			CCString strFile;
			strFile.initWithFormat("%s%d.%s",strPath.getCString(),
				pData->pRawTile->x,
				pData->pMaker->getImageType());

			if (!SunFile::IsExist(strFile.getCString(),false))
			{
				std::string strName = pData->pMaker->getImageType();
				if ( strName.compare("tiff")== 0)
				{
					unsigned char* m_pData = pImage->getData();
					int m_nWidth,m_nHeight;
					m_nWidth = m_nHeight = 256;
					unsigned char *pTempData = new unsigned char[m_nWidth * m_nHeight * 3];

					for (int i = 0; i < m_nHeight; ++i)
					{
						for (int j = 0; j < m_nWidth; ++j)
						{
							pTempData[(i * m_nWidth + j) * 3] = m_pData[(i * m_nWidth + j) * 4];
							pTempData[(i * m_nWidth + j) * 3 + 1] = m_pData[(i * m_nWidth + j) * 4 + 1];
							pTempData[(i * m_nWidth + j) * 3 + 2] = m_pData[(i * m_nWidth + j) * 4 + 2];
						}
					}

					save_tiff_rgb(strFile.getCString(),256,256,(const char*)pTempData);
					delete pTempData;
				}
				else
				{
					pImage->saveToFile(strFile.getCString());
				}
			}

			delete pData;
			pData = NULL;
		}

		delete pImage;
		pImage = NULL;
	}
	else
	{
		curl_easy_cleanup(curl);

		if(pData->nTryTime>0)
		{
			pData->nTryTime--;
			pthread_t pid;
			pthread_create(&pid, NULL, downloadToSave, (void*)pData);
		}
		else
		{
			pthread_mutex_lock(&cachemutex);
			pData->pMaker->update(pData->pRawTile);
			pthread_mutex_unlock(&cachemutex );
			delete pData;
			pData = NULL;
		}
	}
	return NULL;
}

CacheMaker::CacheMaker(std::string strName, CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray, int nMapStrategy)
{
	m_strName = strName;
	m_nMapStrategy = -1;
	m_nTotalCount = 0;
	m_nMaxThread = 10;
	m_nThreadCount = 0;

	m_pntLeftTop = pntLeftTop;
	m_pntRightBottom = pntRightBottom;

	m_nTime = 0;

	m_pMapStrategy = NULL;
	build(pntLeftTop,pntRightBottom,nZoomArray,nMapStrategy);
}

CacheMaker::CacheMaker()
{
	m_pMapStrategy = NULL;

	m_nMapStrategy = -1;
	m_nTotalCount = 0;
	m_nMaxThread = 10; 
	m_nThreadCount = 0;
	m_nTime = 0;
}

CacheMaker::~CacheMaker()
{
	clear();
}

bool CacheMaker::build( CCPoint pntLeftTop,CCPoint pntRightBottom,std::vector<int>& nZoomArray,int nMapStrategy )
{
	clear();

	m_nMapStrategy = nMapStrategy;

	int nSize = nZoomArray.size();

	double lon1 = pntLeftTop.x;
	double lat1 = pntLeftTop.y;
	double lon2 = pntRightBottom.x;
	double lat2 = pntRightBottom.y;
	m_nTotalCount = 0;
	for (int i = 0;i<nSize;i++)
	{
		int nZoom = nZoomArray[i];
		m_ZoomArray.push_back(nZoom);
		CCPoint pnt1 = GeoUtils::toTileXY(lat1,lon1,nZoom);
		CCPoint pnt2 = GeoUtils::toTileXY(lat2,lon2,nZoom);

		int x1 = pnt1.x;
		int y1 = pnt1.y;
		int x2 = pnt2.x+1;
		int y2 = pnt2.y+1;

		int nRow = fabs((float)(x2 - x1));
		int nCol = fabs((float)(y2 - y1));
		int nCount = nRow*nCol;
		m_nTotalCount += nCount;
		m_NumberPerZoom.push_back(nCount);

		for (int m = x1;m<x2;m++)
		{
			for (int n = y1;n<y2;n++)
			{
				RawTile* pTile = new RawTile(m,n,nZoom,nMapStrategy);
				m_TileArray.push_back(pTile);
			}
		}
	}

	return true;
}

void CacheMaker::clear()
{
	int nCount = m_TileArray.size();

	for (int i=0;i<nCount;i++)
	{
		RawTile* pTile = m_TileArray[i];
		delete pTile;
		pTile = NULL;
	}

	m_ZoomArray.clear();
	m_NumberPerZoom.clear();
	m_TileArray.clear();
}

void CacheMaker::generate()
{
	m_pMapStrategy = MapStrategyFactory::getStrategy(m_nMapStrategy);
	m_nNeedTodown = m_nTotalCount;

	clock_t begin = clock();
	while (m_nNeedTodown>0)
	{
		if (m_nThreadCount<m_nMaxThread && m_TileArray.size()>0)
		{
			RawTile* pTile = m_TileArray[m_TileArray.size()-1];
			m_TileArray.pop_back();

			pthread_mutex_lock(&cachemutex);
			m_nThreadCount++;
			pthread_mutex_unlock(&cachemutex );
			
			tileData* pData = new tileData;
			pData->pRawTile = pTile;
			pData->nTryTime = 5;
			pData->pMaker = this;

			pthread_t pid;
			pthread_create(&pid, NULL, downloadToSave, (void*)pData);
		}
	}
	clock_t end = clock();

	m_nTime = (end-begin)/1000;

	saveConfig();
}

void CacheMaker::setMaxThread( int nThread )
{
	m_nMaxThread = nThread;
}

int CacheMaker::getMaxThread()
{
	return m_nMaxThread;
}

int CacheMaker::getMapStrategyId()
{
	return m_nMapStrategy;
}

MapStrategy* CacheMaker::getMapStrategy()
{
	return m_pMapStrategy;
}

void CacheMaker::setSavePath( const char* strPath )
{
	m_strPath = strPath;
}

const char* CacheMaker::getSavePath()
{
	return m_strPath.c_str();
}

void CacheMaker::setImageType(std::string nType )
{
	m_strImageType = nType;
}

const char* CacheMaker::getImageType()
{
	return m_strImageType.c_str();
}

void CacheMaker::update( RawTile* pTile )
{
	m_nNeedTodown--;
	m_nThreadCount--;
	if(pTile != NULL)
	{
		RawTile tile(pTile->x,pTile->y,pTile->z,pTile->s);
		m_failTile.push_back(tile);
	}
}

void CacheMaker::saveConfig()
{
	sunMap::CacheInfo* pCache = new sunMap::CacheInfo;
	pCache->set_name(m_strName.c_str());

	sunMap::Bounds* pBounds = new sunMap::Bounds;
	pBounds->set_left(m_pntLeftTop.x);
	pBounds->set_top(m_pntLeftTop.y);
	pBounds->set_right(m_pntRightBottom.x);
	pBounds->set_bottom(m_pntRightBottom.y);

	pCache->set_allocated_bound(pBounds);

	CCString strMapId;
	strMapId.initWithFormat("%d",m_nMapStrategy);
	pCache->set_mapid(strMapId.getCString());

	pCache->set_imagetype(m_strImageType.c_str());

	pCache->set_time(m_nTime);

	sunMap::LodInfos* pInfos = new sunMap::LodInfos;
	pInfos->set_numlevel(m_ZoomArray.size());
	pInfos->set_numtile2need(m_nTotalCount);
	pInfos->set_numtileindisk(m_nTotalCount-m_failTile.size());

	int nSize = m_ZoomArray.size();
	for (int i = 0;i<nSize;i++)
	{
		sunMap::LodInfo* pInfo = pInfos->add_lodinfos();;
		pInfo->set_level(m_ZoomArray[i]);
		pInfo->set_numtile2need(m_NumberPerZoom[i]);
	}

	nSize = m_failTile.size();
	for (int i=0;i<nSize;i++ )
	{
		RawTile tile = m_failTile[i];
		int j = 0;
		int count = m_ZoomArray.size();
		for (j=0;j<count;j++)
		{
			if (m_ZoomArray[j] == tile.z)
				break;
		}
		sunMap::LodInfo* pInfo = pInfos->mutable_lodinfos(j);
		sunMap::CacheTile* pTile = pInfo->add_tilelost();
		pTile->set_x(tile.x);
		pTile->set_y(tile.y);
		pTile->set_z(tile.z);
	}

	pCache->set_allocated_lods(pInfos);
	
	int length = pCache->ByteSize();
	char* buf = new char[length];
	pCache->SerializeToArray(buf,length);

	FileOperation::saveFile(buf,length,m_strPath+"//"+m_strName + ".config");
	delete pCache;
	pCache = NULL;

// 	delete pBounds;
// 	pBounds = NULL;
// 
// 	delete pInfos;
// 	pInfos = NULL;

	delete []buf;
	buf = NULL;
}

/*
bool CacheMaker::createDirectory( const char *path )
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	mode_t processMask = umask(0);
	int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	umask(processMask);
	if (ret != 0 && (errno != EEXIST))
	{
		return false;
	}

	return true;
#else
	BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
	return true;
#endif
}
*/