#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"


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
        void processImage();
    bool seedFill(int x, int y, int label );
    
	
        int getPixel(int x, int y);
        int setPixel(int x, int y, int grayValue);
        void flipHorizontal();
    
		ofVideoGrabber 		vidGrabber;
		unsigned char * 	videoGray;
		ofTexture			videoTexture;
		int 				camWidth;
		int 				camHeight;
    
        int                 threshold;
    
    bool                showProcessedFeed;
    
    ofImage img, labelImage;
    
    //CV stuff
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
};
