#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth 		= ofGetWidth();	// try to grab at this size. 
	camHeight 		= ofGetHeight();
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
	
	videoGray 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_LUMINANCE);
	
    threshold = 50;
    
    showProcessedFeed = true;
    img.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    labelImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
    
}


//--------------------------------------------------------------
void testApp::update(){
	
	
	ofBackground(100,100,100);
    
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
		grayDiff.threshold(threshold);
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

    
}

void testApp::processImage(){
    //clear the label image
    unsigned char *p = labelImage.getPixels();
    
    // start with label equal to 1
    int label = 1;
    
    // scan all pixels, do seed fill for each
    for( int i=0; i<camWidth; i+=1 )
    {
        for( int j=0; j<camHeight; j+=1 )
        {
            if ( seedFill(i,j,label) )
                label += 1;
        }
    }
    
    // print number of shapes
    //printf("number of shapes: %d\n", label );
    
    // make shapes for every label
    /*for( int i=0; i<label; i++ )
     {
     smObject* OBJ = new smObject( i );
     objectSet.push_back( OBJ );
     }
     
     // put pixel into objects (buckets)
     for( int i=0; i<320; i+=1 )
     {
     for( int j=0; j<240; j+=1 )
     {
     // get label at pixel
     int V = labelImage->getPixel(i,j);
     
     // add to pixel to object
     //objectSet[V]->acceptPixel(i,j);
     
     // amplification, just so labels are visible
     labelImage->setPixel(i,j, V*40 );
     }
     }*/
}

bool        testApp::seedFill( int x, int y, int label )
{
    // if out of bounds return
    if ( x<=0 || x>=camWidth || y<=0 || y>=camHeight )
        return false;
    
    // if source pixel is black return
    float pVal = img.getColor(x,y).getBrightness();
    //printf("pVal: %f",pVal);
    if ( pVal==0 )
    {
        return false;
    }
    
    // if label pixel is not black (already handled) return
    pVal = labelImage.getColor(x,y).getBrightness();

    if ( pVal!=0 ){
        return false;
    }
    
    // set the label pixel
    labelImage.setColor(x,y,label);
    
    // do seedFill for neighbors
    seedFill(x,y+1,label);
    seedFill(x-1,y,label);
    seedFill(x+1,y,label);
    seedFill(x,y-1,label);
    
    return true;
}


int     testApp::getPixel( int x, int y )
{
    int index = x + y*camWidth;                    // calculate the pixel index
    if ( index < 0 || index >= camWidth*camHeight )
    {
        printf("ERROR: getPixel out of bounds\n");
        return 0;
    }
    return videoGray[index];                    // return the pixel
}

int    testApp::setPixel( int x, int y, int grayValue )
{
    int index = x + y*camWidth;                    // calculate the pixel index
    if ( index < 0 || index >= camWidth*camHeight )
    {
        printf("ERROR: setPixel out of bounds\n");
        return 0;
    }
    videoGray[index] = grayValue;               // set the pixel
}

/*void testApp::flipHorizontal(){
    for( int x=0; x<camWidth/2; x++ )
    {
        for( int y=0; y<camHeight; y++ )
        {
           /* float grayValue1 = getPixel(x,y);             // get the gray value
            float grayValue2 = getPixel(camWidth-x-1,y);  // get the gray value
            
            setPixel(camWidth-x-1,y,grayValue1);
            setPixel(x,y,grayValue2);*/
            
           /* ofColor grayValue1 = img.getColor(x,y);             // get the gray value
            ofColor grayValue2 = img.getColor(camWidth-x-1,y);  // get the gray value
            
            img.setColor(camWidth-x-1,y,grayValue1);
            img.setColor(x,y,grayValue2);
            
        }
    }
    
}*/

//------DRAW--------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
    if(showProcessedFeed){
        videoTexture.draw(0,0);
        labelImage.draw(50, 50, 400, 200);
        // we could draw the whole contour finder
        //contourFinder.draw(360,540);
        
        // or, instead we can draw each blob individually,
        // this is how to get access to them:
        for (int i = 0; i < contourFinder.nBlobs; i++){
            contourFinder.blobs[i].draw(360,540);
        }

    }
    else{
        vidGrabber.draw(0,0);

    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	
	if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}
	
	else if(key == ' '){
        showProcessedFeed = !showProcessedFeed;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    //set threshold
	//threshold = round(x/ofGetWidth() * 255);//why no working?
    threshold = x;
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
