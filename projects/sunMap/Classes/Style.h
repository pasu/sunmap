//
//  Style.h
//  sunmap
//
//  Created by 陆 国伟 on 13-6-24.
//
//

#ifndef __sunmap__Style__
#define __sunmap__Style__

#include "cocos2d.h"

USING_NS_CC;

class Style
{
public:
    Style();
    Style(const Style& mStyle);
    Style(GLfloat lineWidth,ccColor4F linecolor,ccColor4F fillcolor);
    
    Style& operator=(const Style& mStyle);
public:
    ccColor4F linecolor;
    ccColor4F fillcolor;
    
    GLfloat lineWidth;
};

#endif /* defined(__sunmap__Style__) */
