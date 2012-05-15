#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    ofHideCursor();
    
    camWidth = 320;
    camHeight = 240;
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,240);
	

    colorImg.allocate(camWidth,camHeight);
	grayImage.allocate(camWidth,camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);
    grayThresh.allocate(camWidth,camHeight);

    
	bLearnBakground = true;
	threshold = 17;

    areaThreshold = .4;
    
    debug = true;
    fs = false;
    
    //make the objects!!!
    objectSet.push_back(new smObject("Mike",5200, "record.mp3", .5));
   // objectSet.push_back(new smObject("rect",700)); 
   // objectSet.push_back(new smObject("triangle",200));
    objectSet.push_back(new smObject("Merche",2700, "merche.mp3", 1));
   // objectSet.push_back(new smObject("Spike", 1500, "reed.mp3"));   
    objectSet.push_back(new smObject("Gabby", 300, "pitcher.mp3", 1));

    
    float xScale = ofGetWidth()/camWidth;
    float yScale = ofGetHeight()/camHeight;
    
   // printf("ofGetHeight: %f", ofGetHeight());
 //   printf("camHeight: %d", camHeight); 
    
    printf("xScale: %f", xScale);
    printf("yScale: %f", yScale);

    float scale = 3.2;
    
    for( int j=0; j<objectSet.size(); j++ )
    {
        objectSet[j]->xScale = scale;
        objectSet[j]->yScale = scale+.3;
    };
    
    //DEFINE THE FONTS
    //ofTrueTypeFont::setGlobalDpi(72);
    
	titleFont.loadFont("din.ttf", 15, true, true);
	//titleFont.setLineHeight(34.0f);
	//titleFont.setLetterSpacing(1.035);

    blur.setup(ofGetWidth(),ofGetHeight());


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
        
        
       // grayThresh.blurGaussian(20);


		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayThresh, 50, (340*240)/3, 3, false);	// find holes

        float areaDif = camHeight*camWidth;
        
        //try to track the objects based on their area?
        //reset object associations
        
        for( int j=0; j<objectSet.size(); j++ )
        {
          //  objectSet[j]->isFound(false);
            objectSet[j]->found=false;

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
                        objectSet[j]->isFound(true);
                        // objectSet[j]->found = true;
                        areaDif = abs(objectSet[j]->area - blobArea);
                        objectSet[j]->blobID = i;
                        objectSet[j]->setX(blobX);
                        objectSet[j]->setY(blobY);
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
            
            //get incoming nbands
            float * val = ofSoundGetSpectrum( objectSet[j]->nBandsToGet);		// request 128 values for fft
            for (int i = 0;i <  objectSet[j]->nBandsToGet; i++){
                
                // map input to alpha value for on color
                objectSet[j]->onAlpha = ofMap(val[2], 0, .8, 150, 255); 
                
                
                //printf("fftval: %f\n", val[2]);
                
              //  printf("fftval: %d\n", objectSet[j]->onAlpha);
                
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
    
    //apply blur
    blur.begin();
    

    blur.amount = 3;//ofMap(mouseX,0,ofGetWidth(),0,10,true);
    blur.iterations = 1;//ofMap(mouseY,0,ofGetHeight(),1,10,true);

    ofFill();
    ofSetColor(0);
   // ofRect(0, 0, ofGetWidth()*2, ofGetHeight()*2);
    ofClear(0,255);
    
    int xOffSet = 50;
    int yOffSet = -30;

    
    for (int i = 0; i < objectSet.size(); i++){
       // if( objectSet[i]->lifted){            
            objectSet[i]->draw(xOffSet,yOffSet);
          //  ofSetHexColor(0x000000);
        //}
    }
    
    blur.end();
   // ofDrawBitmapString("amount: " + ofToString(blur.amount), 10,15);
   // ofDrawBitmapString("iterations: " + ofToString(blur.iterations), 10,25);
//DRAW TEXT
    for (int i = 0; i < objectSet.size(); i++){
        if( objectSet[i]->lifted){            
            //draw text
            ofSetHexColor(0x000000);
            titleFont.drawString(objectSet[i]->name, objectSet[i]->x-titleFont.stringWidth(objectSet[i]->name)/2+xOffSet, objectSet[i]->y+yOffSet);
        }
        
      /*  if(debug){
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
        }*/
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
