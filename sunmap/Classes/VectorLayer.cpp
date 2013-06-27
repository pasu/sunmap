//
//  VectorLayer.cpp
//  sunmap
//
//  Created by 陆 国伟 on 13-6-23.
//
//

#include "VectorLayer.h"
#include "VisibleRect.h"
#include "GeoUtils.h"
#include <geos/geom/GeometryFactory.h>
#include "geos/geom/CoordinateSequence.h"
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include "Feature.h"
#include "geos/geom/LinearRing.h"
#include "geos/geom/MultiPolygon.h"

using namespace geos;
using namespace geos::geom;

VectorLayer::VectorLayer(MapControl* pControl)
{
    m_pControl = pControl;
    m_points = NULL;
    m_nPntNum = 0;
}

VectorLayer::~VectorLayer()
{
    for (std::vector<Feature*>::iterator iter = m_Features.begin(); iter != m_Features.end(); ++iter)
    {
        Feature* pFeature = *iter;
        delete  pFeature;
        pFeature = NULL;
    }
    
    m_Features.clear();
    
    if (m_points) {
        delete []m_points;
        m_points = NULL;
    }
}

void VectorLayer::draw()
{
    if (m_Features.size()==0) {
        return;
    }
    
    RawTile dTile = m_pControl->getMap()->m_defTile;
    CCPoint pntOffset = m_pControl->getMap()->getGlobalOffset();
    
    CCPoint pntTopLeft,pntRightBottom,pntMarker;
    pntTopLeft.x= dTile.x*256 - pntOffset.x;
    pntTopLeft.y= dTile.y*256 - pntOffset.y;
    pntRightBottom.x = pntTopLeft.x + m_pControl->getMap()->getWidth();
    pntRightBottom.y = pntTopLeft.y + m_pControl->getMap()->getHeight();
    
    int zoom = 17-m_pControl->getZoomLevel();
    /*
    CCPoint pntMapTL,pntMapRB;
    pntMapTL = GeoUtils::getLatLong(pntTopLeft.x, pntTopLeft.y, zoom);
    pntMapRB = GeoUtils::getLatLong(pntRightBottom.x, pntRightBottom.y, zoom);
    
    std::vector<Coordinate> pntVector;
    pntVector.push_back(Coordinate(pntMapTL.x,pntMapTL.y,0));
    pntVector.push_back(Coordinate(pntMapTL.x,pntMapRB.y,0));
    pntVector.push_back(Coordinate(pntMapRB.x,pntMapRB.y,0));
    pntVector.push_back(Coordinate(pntMapRB.x,pntMapTL.y,0));
    CoordinateSequence * cs = CoordinateArraySequenceFactory::instance()->create(&pntVector);
    LinearRing* viewBounds = geos::geom::GeometryFactory::getDefaultInstance()->createLinearRing(cs);
    //int zoom = 17 - m_pControl->getZoomLevel();
    */
    double scale = (1 << (17 - zoom)) * GeoUtils::TILE_SIZE;
    
    //cocos2d::CCPoint pnt((int) (normalised.x * scale), (int) (normalised.y * scale));
    //CCPoint offsetGps = m_pControl->getGpsOffset();
    
    //double latFix = lat + offsetGps.y*pow(10.0, -5);
    //double lonFix = lon + offsetGps.x*pow(10.0, -5);
    ccDrawColor4B(0,0,255,128);
    for (std::vector<Feature*>::iterator iter = m_Features.begin(); iter != m_Features.end(); ++iter)
    {
        Feature* pFeature = *iter;
        Geometry* pGeo = pFeature->GetGeometry();
        //if(!viewBounds->contains(pGeo))
        //    continue;
        
        int nCount = pGeo->getNumPoints();
        CoordinateSequence* pCs = pGeo->getCoordinates();
        if(pGeo->getGeometryTypeId() != GEOS_MULTIPOLYGON &&
           pGeo->getGeometryTypeId() != GEOS_MULTILINESTRING)
        {
            if(m_points == NULL)
            {
                m_points = new CCPoint[nCount];
            }
            else if (nCount>m_nPntNum)
            {
                delete []m_points;
                m_points = new CCPoint[nCount];
                m_nPntNum = nCount;
            }
            
            for (int i=0;i<nCount; i++) {
                
                m_pnt = GeoUtils::toNormalisedPixelCoords(pCs->getAt(i).y, pCs->getAt(i).x);
                m_points[i] = CCPoint(m_pnt.x*scale-pntTopLeft.x,m_pControl->getMap()->getHeight()-(m_pnt.y*scale-pntTopLeft.y));
                
            }
        }
        
        
        Style style = pFeature->GetStyle();
        ccDrawColor4B(style.linecolor.r, style.linecolor.g, style.linecolor.b, style.linecolor.a);
        switch (pGeo->getGeometryTypeId()) {
            case GEOS_POINT:
                ccPointSize(style.lineWidth);
                ccDrawPoint(m_points[0]);
                break;
            case GEOS_POLYGON:
                glLineWidth(style.lineWidth);
                ccDrawSolidPoly( m_points, nCount, ccc4f(style.fillcolor.r,style.fillcolor.g,style.fillcolor.b,style.fillcolor.a));
                break;
            case GEOS_LINESTRING:
                glLineWidth(style.lineWidth);
                ccDrawPoly( m_points, nCount,false);
                break;
            case GEOS_LINEARRING:
                glLineWidth(style.lineWidth);
                ccDrawPoly( m_points, nCount,true);
                break;
            case GEOS_MULTIPOINT:
                ccPointSize(style.lineWidth);
                ccDrawPoints(m_points, nCount);
                break;
            case GEOS_MULTIPOLYGON:
            case GEOS_MULTILINESTRING:
            {
                int nSubGeo = pGeo->getNumGeometries();
                for (int j=0; j<nSubGeo; j++) {
                    const Geometry* pSubGeo = pGeo->getGeometryN(j);
                    nCount = pSubGeo->getNumPoints();
                    pCs = pSubGeo->getCoordinates();
                    if(m_points == NULL)
                    {
                        m_points = new CCPoint[nCount];
                    }
                    else if (nCount>m_nPntNum)
                    {
                        delete []m_points;
                        m_points = new CCPoint[nCount];
                        m_nPntNum = nCount;
                    }
                    
                    for (int i=0;i<nCount; i++) {
                        
                        m_pnt = GeoUtils::toNormalisedPixelCoords(pCs->getAt(i).y, pCs->getAt(i).x);
                        m_points[i] = CCPoint(m_pnt.x*scale-pntTopLeft.x,m_pControl->getMap()->getHeight()-(m_pnt.y*scale-pntTopLeft.y));
                        
                    }
                    
                    if (pSubGeo->getGeometryTypeId() == GEOS_POLYGON){
                        glLineWidth(style.lineWidth);
                        ccDrawSolidPoly( m_points, nCount, ccc4f(style.fillcolor.r,style.fillcolor.g,style.fillcolor.b,style.fillcolor.a));
                    }else if(pSubGeo->getGeometryTypeId() == GEOS_LINEARRING){
                        glLineWidth(style.lineWidth);
                        ccDrawPoly( m_points, nCount,true);
                    }else{
                        glLineWidth(style.lineWidth);
                        ccDrawPoly( m_points, nCount,false);
                    }                
                
                }
            }
                break;
                
            default:
                break;
        }
    }
}

void VectorLayer::drawFeature(Feature* pF)
{
    
    return;
}

void VectorLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
    CCTouch* touch = (CCTouch*)(*it);
    
    m_tBeginPos = touch->getLocation();
}
void VectorLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    /*
    CCObject* child;
    
    CCARRAY_FOREACH(getChildren(), child)
    {
        CCMenu* pNode = (CCMenu*) child;
        
        {
            CCActionInterval* actionBy = CCMoveBy::create(0.1, CCPoint(touchLocation.x - m_tBeginPos.x,touchLocation.y - m_tBeginPos.y));
            
            pNode->runAction(actionBy);
        }
    }
     */
    
}

void VectorLayer::AddFeatureLayer(Feature* pFeature)
{
    m_Features.push_back(pFeature);
}

void VectorLayer::AddFeatureLayers(Feature* pFeature,int length)
{
    for (int i=0; i<length; i++) {
        m_Features.push_back(pFeature++);
    }
}

