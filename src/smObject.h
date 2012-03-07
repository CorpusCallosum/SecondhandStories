//
//  smObject.h
//  spatialMediaTest
//
//  Created by Jared Schiffman on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef spatialMediaTest_smObject_h
#define spatialMediaTest_smObject_h
#include <string>
#include "ofMain.h"




class smObject
{
    
public:
    
   // void draw(float x, float y);
    
    smObject( std::string _NAME, int _AREA);
        
    int             area;
    std::string     name;
    int             blobID, x, y, width, height;
    ofRectangle     rect;
    bool            found;
    float           xScale, yScale;
    
    vector <ofPoint>    pts;    // the contour of the blob
    int                 nPts;   // number of pts;
    int                 blobArea;

        
    void draw(float x=0, float y=0){
    //    ofNoFill();
        ofFill();
        ofSetHexColor(0xfdce66);
        ofBeginShape();
        //draw the outline of the object
           
        float sX = x;//*xScale;
        float sY = y;//*yScale;
        
        
        
        for (int i = 0; i < nPts; i++){
            
            float pX = pts[i].x * xScale;
            float pY = pts[i].y * yScale;
            
            //float s = .5;
            
            pX = pX - (pX-sX)*0;
            pY = pY - (pY-sY)*0;
            
            ofVertex(pX, pY);
        }
        ofEndShape(true);
        ofSetHexColor(0xff0099);
        // ofRect(x + boundingRect.x, y + boundingRect.y, boundingRect.width, boundingRect.height);
    }

};


#endif
