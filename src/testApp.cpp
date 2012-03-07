#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    camWidth = 320;
    camHeight = 240;
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
	

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
    grayThresh.allocate(320,240);

    
	bLearnBakground = true;
	threshold = 40;

    areaThreshold = .4;
    
    debug = true;
    fs = false;
    
    //make the objects!!!
    objectSet.push_back(new smObject("Record",5000, "reed.mp3"));

   // objectSet.push_back(new smObject("rect",700)); 
   // objectSet.push_back(new smObject("triangle",200));
    objectSet.push_back(new smObject("Photo",2500, "reed.mp3"));
    objectSet.push_back(new smObject("China", 300, "reed.mp3"));   

    
    float xScale = ofGetWidth()/camWidth;
    float yScale = ofGetHeight()/camHeight;
    
   // printf("ofGetHeight: %f", ofGetHeight());
 //   printf("camHeight: %d", camHeight); 
    
    printf("xScale: %f", xScale);
    printf("yScale: %f", yScale);

    
    for( int j=0; j<objectSet.size(); j++ )
    {
        objectSet[j]->xScale = 3.8;
        objectSet[j]->yScale = 3.8;
    };

}

//--------------------------------------------------------------
void testApp::update(){
	//ofBackground(100,100,100);

    bool bNewFrame = false;

       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
   

	if (bNewFrame){

            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	   

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
        grayThresh = grayDiff;
        grayThresh.threshold(threshold);
        grayThresh.mirror(false, true);


		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayThresh, 50, (340*240)/3, 10, false);	// find holes

        float areaDif = camHeight*camWidth;
        
        //try to track the objects based on their area?
        //reset object associations
        
        for( int j=0; j<objectSet.size(); j++ )
        {
            objectSet[j]->found = false;
        }
        
        
        //copy blobs from contour finder
        vector<ofxCvBlob>  blobs = contourFinder.blobs;

        
        
        for( int j=0; j<objectSet.size(); j++ )
        {
           for (int i = 0; i < blobs.size(); i++){
                      
            //compare sizes of blobs to sizes of objects
            if(!objectSet[j]->found){
                float blobX = blobs[i].centroid.x;
                float blobY = blobs[i].centroid.y;
                float blobArea = blobs[i].area;
                if(abs(objectSet[j]->area - blobArea) < areaDif){
                    int at = objectSet[j]->area*areaThreshold;
                    if(abs(objectSet[j]->area - blobArea) < at){
                        objectSet[j]->isFound();
                        // objectSet[j]->found = true;
                        areaDif = abs(objectSet[j]->area - blobArea);
                        objectSet[j]->blobID = i;
                        objectSet[j]->x = blobX;
                        objectSet[j]->y = blobY;
                        objectSet[j]->rect = blobs[i].boundingRect;
                        objectSet[j]->width = blobs[i].boundingRect.width;
                        objectSet[j]->height = blobs[i].boundingRect.height;
                        //get the outline
                        objectSet[j]->pts = blobs[i].pts;
                        objectSet[j]->nPts = blobs[i].nPts;
                        
                        //set blob area
                        objectSet[j]->setTrackedArea(blobArea);

                         
                        //remove blob from array
                        blobs.erase(blobs.begin()+i);
                        i=0;
                            
                     }
                }
            }
           }
            
            // (5) grab the fft, and put in into a "smoothed" array,
            //		by taking maximums, as peaks and then smoothing downward
            float * val = ofSoundGetSpectrum( objectSet[j]->nBandsToGet);		// request 128 values for fft
            for (int i = 0;i <  objectSet[j]->nBandsToGet; i++){
                
                // let the smoothed calue sink to zero:
                 objectSet[j]->fftSmoothed[i] *= 0.96f;
                
                // take the max, either the smoothed or the incoming:
                if ( objectSet[j]->fftSmoothed[i] < val[i])  objectSet[j]->fftSmoothed[i] = val[i];
                
            }
            
            
            
        }

      
	}

}

//--------------------------------------------------------------
void testApp::draw(){

    if(!fs){
    //THIS IS THE DEBUG MODE
    if(debug){
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
    grayThresh.draw(360+360,280);

        
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
        ofSetHexColor(0xffffff);

        grayThresh.draw(0,0, ofGetWidth(), ofGetHeight());
        
        for (int i = 0; i < objectSet.size(); i++){
            if(objectSet[i]->found){
            int blobID = objectSet[i]->blobID;
            float blobX = objectSet[i]->x;
            float blobY = objectSet[i]->y;
            //print id
           // char reportStr[1024];
          //  sprintf(reportStr, "%f", objectSet[i]->name);
            float xRatio = ofGetWidth()/320;
            float yRatio = ofGetHeight()/240;
            ofSetHexColor(0xff0000);
            ofDrawBitmapString(objectSet[i]->name, blobX*xRatio+objectSet[i]->rect.width*3, blobY*yRatio+objectSet[i]->rect.height*3);
        }
        }
    }
    }
    else{
        ofBackground(0,0,0);
        //THIS is the active mode!
        drawObjects();
        
        
    }

}

void testApp::drawObjects(){
    for (int i = 0; i < objectSet.size(); i++){
            objectSet[i]->draw(0,0);
            
        if(debug){
            //DISPLAY TEXT
                int blobID = objectSet[i]->blobID;
                float blobX = objectSet[i]->x;
                float blobY = objectSet[i]->y;
                //print id
                // char reportStr[1024];
                //  sprintf(reportStr, "%f", objectSet[i]->name);
                float xRatio = ofGetWidth()/320;
                float yRatio = ofGetHeight()/240;
                ofSetHexColor(0xff0000);
                ofDrawBitmapString(objectSet[i]->name, blobX*xRatio+objectSet[i]->rect.width*2, blobY*yRatio+objectSet[i]->rect.height*2); 
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
    else if(key == 'f'){
        //enter active mode!
        fs = !fs;
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
