#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth 		= ofGetWidth();	// try to grab at this size. 
	camHeight 		= ofGetHeight();
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
	
	videoGray 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, GL_LUMINANCE);
	
    threshold = 128;
    
    showProcessedFeed = true;
    //img.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(100,100,100);
	
	vidGrabber.grabFrame();
	
	if (vidGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
        //loop through pixels and convert to greyscale texture
		for (int i = 0; i < totalPixels; i+=3){
            int pAverage = (pixels[i] + pixels[i+1] + pixels[i+2] )/3;
            //do the thresholding here
            if(pAverage > threshold){
                //make white
                videoGray[i/3] = 255;
            }
            else{
                //make black
                videoGray[i/3] = 0;
            }
            
            //videoGray[i/3] = pAverage;
		}
        img.setFromPixels(videoGray, camWidth, camHeight, OF_IMAGE_GRAYSCALE);
        
        flipHorizontal();
        
		videoTexture.loadData(img.getPixels(), camWidth,camHeight, GL_LUMINANCE);
	}
    
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

void testApp::flipHorizontal(){
    for( int x=0; x<camWidth/2; x++ )
    {
        for( int y=0; y<camHeight; y++ )
        {
           /* float grayValue1 = getPixel(x,y);             // get the gray value
            float grayValue2 = getPixel(camWidth-x-1,y);  // get the gray value
            
            setPixel(camWidth-x-1,y,grayValue1);
            setPixel(x,y,grayValue2);*/
            
            ofColor grayValue1 = img.getColor(x,y);             // get the gray value
            ofColor grayValue2 = img.getColor(camWidth-x-1,y);  // get the gray value
            
            img.setColor(camWidth-x-1,y,grayValue1);
            img.setColor(x,y,grayValue2);
            
        }
    }
    
}

//------DRAW--------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
    if(showProcessedFeed){
        videoTexture.draw(0,0);
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
