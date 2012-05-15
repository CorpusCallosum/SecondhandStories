#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "smObject.h"
#include "ofxBlurShader.h"
#include "opencv2/opencv.hpp"


#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void captureOutlines();
        void drawObjects();

    
        ofVideoGrabber          vidGrabber;
		
        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff, grayThresh;

        ofxCvContourFinder      contourFinder;

		int                     threshold, camWidth, camHeight;
		bool                    bLearnBakground, debug, fs;
    
        float                   areaThreshold;
        
        //fonts
        ofTrueTypeFont          titleFont;

        
        //add a vector of objects here
        std::vector< smObject* >    objectSet;
    
        ofxBlurShader blur;


};

