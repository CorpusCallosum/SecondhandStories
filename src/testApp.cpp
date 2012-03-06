#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
    
    debug = true;
    
    //make the objects!!!
    objectSet.push_back(new smObject("cube", 1000));   
    objectSet.push_back(new smObject("rect",700)); 
    objectSet.push_back(new smObject("triangle",200));   


}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 50, (340*240)/3, 10, false);	// find holes

        float areaDif = 5000;
        
        //try to track the objects based on their area?
        for (int i = 0; i < contourFinder.nBlobs; i++){
            
            
            for( int j=0; j<objectSet.size(); j++ )
            {
                
            //compare sizes of blobs to sizes of objects
            
                                
            float blobX = contourFinder.blobs[i].centroid.x;
            float blobY = contourFinder.blobs[i].centroid.y;
            float blobArea = contourFinder.blobs[i].area;
                if(abs(objectSet[j]->area - blobArea) < areaDif){
                    areaDif = abs(objectSet[j]->area - blobArea);
                    objectSet[j]->blobID = i;   
                }

                
            }
        }

      
	}

}

//--------------------------------------------------------------
void testApp::draw(){

    //THIS IS THE DEBUG MODE
    if(debug){
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
        float blobX = contourFinder.blobs[i].centroid.x;
        float blobY = contourFinder.blobs[i].centroid.y;
        float blobArea = contourFinder.blobs[i].area;
        //print area
        char reportStr[1024];
        sprintf(reportStr, "%f", blobArea);
        ofDrawBitmapString(reportStr, blobX+360, blobY+540);

    }

	// finally, a report:

	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
    }
    else{
        //THIS IS THE ACTIVE MODE
        ofSetHexColor(0xffffff);

        grayDiff.draw(0,0, ofGetWidth(), ofGetHeight());
        
        for (int i = 0; i < objectSet.size(); i++){
            int blobID = objectSet[i]->blobID;
            float blobX = contourFinder.blobs[blobID].centroid.x;
            float blobY = contourFinder.blobs[blobID].centroid.y;
            //print id
           // char reportStr[1024];
          //  sprintf(reportStr, "%f", objectSet[i]->name);
            float xRatio = ofGetWidth()/320;
            float yRatio = ofGetHeight()/240;
            ofSetHexColor(0xff0000);
            ofDrawBitmapString(objectSet[i]->name, blobX*xRatio, blobY*yRatio);
        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == 'd'){
        debug = !debug;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
