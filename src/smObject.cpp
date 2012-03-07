//
//  smObject.cpp
//  spatialMediaTest
//
//  Created by Jared Schiffman on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "smObject.h"

smObject::smObject( std::string _NAME, int _AREA, std::string _soundFile)
{
    
    name = _NAME;
    area = _AREA;
    
    xScale = 1;
    yScale = 1;
    
    found = false;
    nPts  = 0;
    story.loadSound(_soundFile);
    
    fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 128;
    
    

}

/*void smObject::draw(float x, float y){
    ofNoFill();
    ofSetHexColor(0x00FFFF);
    ofBeginShape();
    //draw the outline of the object
    for (int i = 0; i < nPts; i++){
        ofVertex(x + pts[i].x, y + pts[i].y);
    }
    ofEndShape(true);
    ofSetHexColor(0xff0099);
   // ofRect(x + boundingRect.x, y + boundingRect.y, boundingRect.width, boundingRect.height);
}*/
