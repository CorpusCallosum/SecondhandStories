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
    
    found = lifted = false;
    nPts  = 0;
    story.loadSound(_soundFile);
    
    fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = 128;
    
    //DEFINE THE FONTS
    ofTrueTypeFont::setGlobalDpi(72);
    
	subTitleFont.loadFont("verdana.ttf", 14, true, true);
	subTitleFont.setLineHeight(18.0f);
	subTitleFont.setLetterSpacing(1.037);
    
	titleFont.loadFont("verdana.ttf", 30, true, true);
	titleFont.setLineHeight(34.0f);
	titleFont.setLetterSpacing(1.035);
    
    //define colors
    offColor = ofColor (0,0,0);   
    onColor = ofColor (251,198,94);
    lerpTarget = 0;
    lerpAmt = 0;
}


void smObject::setTrackedArea(int _area){
    
    if(areas.size() >= 20){
        //remove first element
        areas.erase(areas.begin());
    }
    
    areas.push_back(_area);
    
    //average all areas
    int areaSum = 0;
    for(int i=0;i<areas.size();i++){
        areaSum+=areas[i];
    }
    
    smoothedTrackedArea = areaSum/areas.size();
    
    //check for change in area
    if(abs(_area - smoothedTrackedArea) > area*.04){
        //OBJECT HAS BEEN LIFTED!
        lifted = true;
        
        story.play();
        
    }
    
}

void smObject::isFound(bool b){
    story.stop();
    
    found = b;
    lifted = !b;
    
    if(b)
        lerpTarget = 0;
    else
        lerpTarget = 1;
}

void smObject::setX(int _x){
    x = _x*xScale;
}

void smObject::setY(int _y){
    y = _y*yScale;
}









