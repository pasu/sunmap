#include "PhysicMap.h"
#include "GeoUtils.h"
#include "MapControl.h"

int PhysicMap::m_nZoom = 0;

PhysicMap::PhysicMap()
{
	m_fScaleFactor = 1.00;
	//m_Cells = NULL;
	m_nTotalCells = m_nTotalCellsX = m_nTotalCellsY = 0;
	m_nWidth = m_nHeight = 0;

	m_pTileResolver = NULL;

	m_nInZoom = 0;
	m_nCorrectionX = m_nCorrectionY = 0;

	m_pMapControl = NULL;

	m_bInMove = false;
	m_update = false;
	m_pHandler = NULL;


	readBgData();
}

PhysicMap::~PhysicMap()
{
	if(m_pTileResolver)
	{
		delete m_pTileResolver;
		m_pTileResolver = NULL;
	}
}

void PhysicMap::goTo( int x, int y, int z, int offsetX, int offsetY )
{
	int fullX = x * 256 + offsetX;
	int fullY = y * 256 + offsetY;

	int tx = fullX - getWidth() / 2;
	int ty = fullY - getHeight() / 2;
	
	m_pntGlobalOffset.x = -(tx - (int)(tx / 256+0.5) * 256);
	m_pntGlobalOffset.y = -(ty - (int)(ty / 256+0.5) * 256);
	m_nZoom = z;
	
	reload(tx / 256, ty / 256, z);
}

int PhysicMap::getWidth()
{
	return m_nWidth;
}

int PhysicMap::getHeight()
{
	return m_nHeight;
}

void PhysicMap::setWidth( int width )
{
	m_nWidth = width;
}

void PhysicMap::setHeight( int height )
{
	m_nHeight = height;
}

void PhysicMap::reload( int x, int y, int z )
{
	m_defTile.x = x;
	m_defTile.y = y;
	m_defTile.z = z;
	loadCells(m_defTile);
}

CCImage* PhysicMap::getBgImage()
{
    return m_pBg;
}

void PhysicMap::loadCells( RawTile tile )
{
	int t =0;
	m_pTileResolver->resetLoaded();
	m_pTileResolver->clearQueue();
    
	for (int i = 0; i < m_nTotalCellsX; i++) {
		for (int j = 0; j < m_nTotalCellsY; j++) {
			int x, y;
			x = (tile.x + i);
			y = (tile.y + j);
			if (m_fScaleFactor-1<0.0000001) {
				setBitmap(MapControl::CELL_BACKGROUND, i, j);
			}
			if (m_pTileResolver->getMapSourceId() >7 || GeoUtils::isValid(tile) ) {
				t++;
				RawTile* pTile = new RawTile(x, y, PhysicMap::m_nZoom, m_pTileResolver->getMapSourceId());
				m_pTileResolver->getTile(pTile);
			} else {
				t++;
				m_pTileResolver->incLoaded();
			}
		}
	}
    
    ((MapControl*)(m_pMapControl))->updateScreen();
}

void PhysicMap::initPhysicMap( int width, int height, RawTile defTile,void* pMapControl )
{
	resetCell(width, height);;
	this->m_defTile = defTile;
	this->m_pMapControl = pMapControl;
	PhysicMap::m_nZoom = defTile.z;
	m_pTileResolver = new TileResolver((void*)this);
	loadCells(defTile);
}

void PhysicMap::resetCell( int width, int height )
{
	setWidth(width);
	setHeight(height);

	if(m_Cells.size()>0)
	{
		releaseCells();
	}

	m_nTotalCellsX = getCellSize(width)+1;
	m_nTotalCellsY = getCellSize(height)+1;
	m_nTotalCells = m_nTotalCellsX * m_nTotalCellsY;

	for (int i=0;i<m_nTotalCells;i++)
	{
		m_Cells.push_back(NULL);
	}
}

int PhysicMap::getCellSize( int length )
{
	int i = 0;
	while (++i*GeoUtils::TILE_SIZE < length) {}
	return ++i;
}

void PhysicMap::setBitmap( CCImage* pTexture, int x, int y )
{
	m_Cells[y*m_nTotalCellsX+x] = pTexture;
}

void PhysicMap::setBitmap( int nType, int x, int y )
{
	switch (nType)
	{
	case MapControl::CELL_BACKGROUND:
		m_Cells[y*m_nTotalCellsX+x] = m_pBg;
		break;
	}
}

void PhysicMap::update( CCImage* pTexture, RawTile* pTile )
{
	bool bIn = false;
	if (!m_bInMove && pTile->z == m_nZoom) {
		int dx = pTile->x - m_defTile.x;
		int dy = pTile->y - m_defTile.y;
		if (dx < m_nTotalCellsX && dy < m_nTotalCellsY && pTile->z == m_defTile.z) {
			if (dx >= 0 && dy >= 0) {
				if(pTexture != NULL)
				{
					setBitmap(pTexture, dx, dy);
					updateMap();
					bIn = true;
				}
			}
		}
	}

	if (pTile)
	{
		delete pTile;
		pTile = NULL;
	}
}

void PhysicMap::updateMap()
{
	int loadedTiles = m_pTileResolver->getLoaded();
	if (loadedTiles >= m_nTotalCells || m_nZoom > 13) {
		if (m_nInZoom != 0) {
			m_pntGlobalOffset.x = (-1) * m_nInZoom * (m_nCorrectionX);
			m_pntGlobalOffset.y = (-1) * m_nInZoom * (m_nCorrectionY);
			m_nInZoom = 0;
			m_fScaleFactor = 1.0;
		}

//		m_update = true;
//		((MapControl*)(m_pMapControl))->updateScreen();
// 		updateScreenCommand.execute();
// 		SmoothZoomEngine.getInstance().nullScaleFactor();
	}
}

CCImage* PhysicMap::getCell( int x, int y )
{
	CCImage* pTexture =m_Cells[y*m_nTotalCellsX+x];
	return pTexture;
}

cocos2d::CCPoint PhysicMap::getGlobalOffset()
{
	return m_pntGlobalOffset;
}

void PhysicMap::releaseCells()
{
	m_Cells.clear();
}

void PhysicMap::setGlobalOffset( cocos2d::CCPoint pnt )
{
	m_pntGlobalOffset = pnt;
}

bool PhysicMap::quickHack(bool bForce)
{
	int dx = 0, dy = 0;
	int tdx = 0, tdy = 0;
	CCPoint globalOffset = getGlobalOffset();

	for (int i = 0; i < 2; i++) {
		if (globalOffset.x > 0) {
			dx = (int)((globalOffset.x + getWidth()) / GeoUtils::TILE_SIZE + 0.5);
		} else {
			dx = (int)((globalOffset.x) / GeoUtils::TILE_SIZE+ 0.5);
		}

		if (globalOffset.y > 0) {
			dy = (int)((globalOffset.y + getHeight()) / GeoUtils::TILE_SIZE + 0.5);
		} else {
			dy = (int)(globalOffset.y / GeoUtils::TILE_SIZE + 0.5);
		}

		globalOffset.x = globalOffset.x - dx * GeoUtils::TILE_SIZE;
		globalOffset.y = globalOffset.y - dy * GeoUtils::TILE_SIZE;

		tdx += dx;
		tdy += dy;
	}

	setGlobalOffset(globalOffset);

	if (!(tdx == 0 && tdy == 0) || bForce==true) {
		move(tdx, tdy);
        return  true;
	}
    return  false;
}

void PhysicMap::move( int dx, int dy )
{
    RawTile tempTile(m_defTile.x - dx, m_defTile.y - dy, m_defTile.z,0);
    if(GeoUtils::isValid(tempTile))
    {
        reload(m_defTile.x - dx, m_defTile.y - dy, m_defTile.z);
    }
}

void PhysicMap::readBgData()
{
	m_pBg = new CCImage;
	m_pBg->initWithImageFile("bg.png");
}

void PhysicMap::zoomS( double dz )
{
	int offsetX = getWidth() / 2;
	int offsetY = getHeight() / 2;
	int zoomTo = GeoUtils::getZoomLevel(dz);
	if (dz > 1) {
        if(m_nZoom - zoomTo<-2)
            return;
		int currentZoomX = (int) (m_defTile.x * GeoUtils::TILE_SIZE
			- m_pntGlobalOffset.x + offsetX);
		int currentZoomY = (int) (m_defTile.y * GeoUtils::TILE_SIZE
			- m_pntGlobalOffset.y + offsetY);

		// obtain the coordinates of a new level
		int nextZoomX = (int) (currentZoomX * dz);
		int nextZoomY = (int) (currentZoomY * dz);

		// obtain the coordinates of the angle of the screen to the next level
		nextZoomX = nextZoomX - offsetX;
		nextZoomY = nextZoomY - offsetY;

		// obtaining angular tile
		int tileX = nextZoomX / GeoUtils::TILE_SIZE;
		int tileY = nextZoomY / GeoUtils::TILE_SIZE;

		// indentation is always one point - to be located in the center of the screen
		m_nCorrectionX = nextZoomX - tileX * GeoUtils::TILE_SIZE;
		m_nCorrectionY = nextZoomY - tileY * GeoUtils::TILE_SIZE;

		m_nInZoom = 1;
		m_nZoom = m_nZoom - zoomTo;
		zoom(tileX, tileY, m_nZoom);
	} else {
        if(m_nZoom + zoomTo>16)
            return;

		int currentZoomX = (int) (m_defTile.x * GeoUtils::TILE_SIZE
			- m_pntGlobalOffset.x + offsetX);
		int currentZoomY = (int) (m_defTile.y * GeoUtils::TILE_SIZE
			- m_pntGlobalOffset.y + offsetY);

		// obtain the coordinates of a new level
		int nextZoomX = (int) (currentZoomX / (1 / dz));
		int nextZoomY = (int) (currentZoomY / (1 / dz));

		// obtain the coordinates of the angle of the screen to the next level
		nextZoomX = nextZoomX - offsetX;
		nextZoomY = nextZoomY - offsetY;

		// obtaining angular tile
		int tileX = nextZoomX / GeoUtils::TILE_SIZE;
		int tileY = nextZoomY / GeoUtils::TILE_SIZE;

		// indentation is always one point - to be located in the center of the screen
		m_nCorrectionX = -(nextZoomX - tileX * GeoUtils::TILE_SIZE);
		m_nCorrectionY= -(nextZoomY - tileY * GeoUtils::TILE_SIZE);

		m_nInZoom = -1;
		m_nZoom = m_nZoom + zoomTo;
		zoom(tileX, tileY, m_nZoom);
	}
}

void PhysicMap::zoom( int x, int y, int z )
{
	m_pTileResolver->clearQueue();
    
    m_pTileResolver->clearCache();
	memset((void*)&m_Cells[0], NULL, m_nTotalCells);
  
	reload(x, y, z);
}

TileResolver* PhysicMap::getTileResolver()
{
	return m_pTileResolver;
}

void PhysicMap::reloadTiles()
{
	loadCells(m_defTile);
}

int PhysicMap::getZoomLevel()
{
	return 17 - m_nZoom;
}

