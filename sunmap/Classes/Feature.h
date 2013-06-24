//
//  Feature.h
//  sunmap
//
//  Created by 陆 国伟 on 13-6-24.
//
//

#ifndef __sunmap__Feature__
#define __sunmap__Feature__

#include "Style.h"

#include <geos/geom/Geometry.h>

USING_NS_CC;

using namespace geos;
using namespace geos::geom;

class Feature
{
public:
    Feature(Geometry* pGeo,const Style& style);
    Feature(Geometry* pGeo);
    ~Feature();
    
    void SetStyle(const Style& style);
private:
    Geometry* m_pGeo;
    Style m_Style;
    
};

#endif /* defined(__sunmap__Feature__) */
