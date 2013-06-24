//
//  Feature.cpp
//  sunmap
//
//  Created by 陆 国伟 on 13-6-24.
//
//

#include "Feature.h"

Feature::Feature(Geometry* pGeo,const Style& style)
{
    m_pGeo = pGeo;
    m_Style = style;
}

Feature::Feature(Geometry* pGeo)
{
    m_pGeo = pGeo;
}

Feature::~Feature()
{
    if(m_pGeo)
        delete m_pGeo;
}

void Feature::SetStyle(const Style& style)
{
    m_Style = style;
}
