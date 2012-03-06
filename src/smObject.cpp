//
//  smObject.cpp
//  spatialMediaTest
//
//  Created by Jared Schiffman on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "smObject.h"

smObject::smObject( int _ID, int _W, int _H )
{
    ID = _ID;
    area = 0;
    midX = midY = 0;
    xTotal = 0;
    yTotal = 0;
    //
    left = _W;
    right = 0;
    top = _H;
    bottom = 0;
    
}

void    smObject::acceptPixel( int x, int y )
{
    // process incoming pixels here
    
    //area
    area++;
    
    //midpoint
    xTotal += x;
    yTotal += y;
    midX = xTotal/area;
    midY = yTotal/area;

    //bounding box
    if(x < left)
        left = x;
    if(x > right)
        right = x;
    if(y < top)
        top = y;
    if(y > bottom)
        bottom = y;
}








