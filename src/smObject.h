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
    
    smObject( std::string _NAME, int _AREA, std::string _soundFile);
        
    int             area;
    std::string     name;
    int             blobID, x, y, width, height;
    ofRectangle     rect;
    bool            found, lifted;
    float           xScale, yScale;
    
    vector <ofPoint>    pts;        // the contour of the blob
    int                 nPts;       // number of pts;
    int                 blobArea;
    vector <int>        areas;      //
    float               smoothedTrackedArea;

    
    //soundFile
    ofSoundPlayer       story;
    std::string         soundFile;
    float 				* fftSmoothed;
    int                 nBandsToGet;
    
    ofTrueTypeFont      titleFont;
    ofTrueTypeFont      subTitleFont;
    
    //METHODS
    void setTrackedArea(int _area);
    void isFound();
    void setX(int _x);
    void setY(int _y);
    
    
    void draw(float x=0, float y=0){
        ofFill();
        if(lifted){
            //BRIGHTER
            ofSetHexColor(0xfdce66);
           // ofSetColor(100*fftSmoothed[3]);
        }
        else{
            //DARKER
           // ofSetHexColor(0x6d592e);
            ofSetHexColor(0x000000);
            

        }
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
        
        //Draw text
        ofSetHexColor(0xffffff);
        titleFont.drawString(name, 0, 0);

       // titleFont.drawString(name, x-titleFont.stringWidth(name)/2, y);
       // subTitleFont.drawString(name, x, y);

    }

};


#endif
