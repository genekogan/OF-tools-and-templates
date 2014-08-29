#include "Kinect.h"

void Kinect::setup(){
    blobColors[0] = ofColor(255, 0, 0);
    blobColors[1] = ofColor(0, 255, 0);
    blobColors[2] = ofColor(0, 0, 255);
    blobColors[3] = ofColor(255, 255, 0);
    blobColors[4] = ofColor(255, 0, 255);
    blobColors[5] = ofColor(0, 127, 255);
    blobColors[6] = ofColor(0, 255, 127);
    blobColors[7] = ofColor(127, 0, 255);
    blobColors[8] = ofColor(255, 0, 127);
    blobColors[9] = ofColor(127, 255, 0);
    blobColors[10]= ofColor(255, 127, 0);
    
    kinect.setRegistration(true);
    kinect.init();
	kinect.open();
    
    control.registerParameter("fade", &fade, 0.0f, 255.0f);
    control.registerParameter("minArea", &minArea, 0.0f, 255.0f);
    control.registerParameter("maxArea", &maxArea, 0.0f, 255.0f);
    control.registerParameter("threshold", &threshold, 0.0f, 255.0f);
    control.registerParameter("persistence", &persistence, 0.0f, 100.0f);
    control.registerParameter("maxDistance", &maxDistance, 0.0f, 100.0f);
    control.registerParameter("numDilate", &numDilate, 0, 10);
    control.registerParameter("numErode", &numErode, 0, 10);
    control.registerParameter("nearThreshold", &nearThreshold, 0.0f, 255.0f);
    control.registerParameter("farThreshold", &farThreshold, 0.0f, 255.0f);
    control.registerParameter("smoothed", &smoothness, 0, 10);
    control.registerParameter("curved", &curved);
    
    fade = 200;
    minArea = 1000;
    maxArea = 70000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    numDilate = 2;
    numErode = 0;
    nearThreshold = 230;
    farThreshold = 10;
    smoothness = 1;
    curved = false;
}

void Kinect::toggleGuiView() {
    //gui->setVisible(!gui->isVisible());
}

void Kinect::setContourStrategy(ContourStrategy contourStrategy) {
    this->contourStrategy = contourStrategy;
    /*
    if (contourStrategy == BLOBS) {
        grayImage.allocate(kinect.width, kinect.height);
        grayThreshNear.allocate(kinect.width, kinect.height);
        grayThreshFar.allocate(kinect.width, kinect.height);
	    
        // setup gui
        gui = new ofxUICanvas();
        gui->setHeight(800);
        gui->setName("parameters");
        gui->addLabel("kinect");
        gui->addSpacer();
        gui->addSlider("nearThresh", 0, 255, &nearThreshold);
        gui->addSlider("farThresh", 0, 255, &farThreshold);
        gui->addLabel("contours");
        gui->addSpacer();
        gui->addSlider("minArea", 0, 5000, &minArea);
        gui->addSlider("maxArea", 15000, 150000, &maxArea);
        gui->addSlider("threshold", 1, 100, &threshold);
        gui->addSlider("persistence", 1, 100, &persistence);
        gui->addSlider("maxDistance", 1, 100, &maxDistance);
        gui->addIntSlider("smoothness", 1, 12, &smoothness);
        gui->addToggle("curved", &curved);
    }
    else if (contourStrategy == SEGMENTATION) {
        colorImage.allocate(kinect.width, kinect.height);
        grayImage.allocate(kinect.width, kinect.height);
        fboKinect.allocate(kinect.width, kinect.height, GL_RGBA);
        fboEdges.allocate(kinect.width, kinect.height, GL_RGB);
        edgeShader.load("kinect/standard.vert", "kinect/edges.frag");
        
        gui = new ofxUICanvas();
        gui->setHeight(800);
        gui->setName("parameters");
        gui->addLabel("kinect");
        gui->addSpacer();
        gui->addSlider("fade", 0, 255, &fade);
        gui->addSlider("minArea", 0, 5000, &minArea);
        gui->addSlider("maxArea", 5000, 320000, &maxArea);
        gui->addSlider("threshold", 1, 100, &threshold);
        gui->addSlider("persistence", 1, 100, &persistence);
        gui->addSlider("maxDistance", 1, 100, &maxDistance);
        gui->addIntSlider("numDilate", 0, 8, &numDilate);
        gui->addIntSlider("numErode", 0, 8, &numErode);
        gui->addIntSlider("smoothness", 1, 12, &smoothness);
        gui->addToggle("curved", &curved);
    }
     */
}

void Kinect::setCalibration(string path){
    kpt.loadCalibration(path);
}

void Kinect::setGuiPosition(int x, int y){
    //gui->setPosition(x, y);
}

//ContourFinder* Kinect::getContourFinder(){
//    return &contourFinder; //.getTracker();
//}

void Kinect::update(){
    kinect.update();
    if(kinect.isFrameNew()) {
        if (contourStrategy == BLOBS) {
            updateBlobs();
        }
        else if (contourStrategy == SEGMENTATION) {
            updateSegmentation();
        }
    }
}

void Kinect::updateBlobs() {
    // process kinect depth image
    grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
    grayThreshNear = grayImage;
    grayThreshFar = grayImage;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    grayImage.flagImageChanged();
    
    // set contour tracker parameters
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    
    // determine found contours
    contourFinder.findContours(grayImage);
}

void Kinect::updateSegmentation() {
    // depth image fade
    fboKinect.begin();
    ofEnableAlphaBlending();
    ofSetColor(255, fade);
    kinect.drawDepth(0, 0);
    ofDisableAlphaBlending();
    fboKinect.end();
    
    // edges shader
    fboEdges.begin();
    edgeShader.begin();
    fboKinect.draw(0, 0);
    edgeShader.end();
    fboEdges.end();
    
    fboEdges.readToPixels(pixels);
    colorImage.setFromPixels(pixels);
    grayImage.setFromColorImage(colorImage);
    
    // post-processing
    for (int i=0; i<numDilate; i++){
        grayImage.dilate_3x3();
    }
    for (int i=0; i<numErode; i++){
        grayImage.erode_3x3();
    }
    grayImage.invert();
    grayImage.threshold(250);
    
    // run through contour finder
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    contourFinder.findContours(grayImage);
}

void Kinect::drawDebugBlobs(){
    ofBackground(0);
    ofSetColor(255);
    ofPushMatrix();
    kinect.draw(0, 0);
    ofTranslate(640, 0);
    grayImage.draw(0, 0);
    ofTranslate(-640, 480);
    contourFinder.draw();
    ofTranslate(640, 0);
    
	RectTracker& tracker = contourFinder.getTracker();
	
    for(int i = 0; i < contourFinder.size(); i++)
    {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        
        // draw contours
        ofFill();
        ofSetColor(blobColors[label % 11]);
        ofBeginShape();
        for (int j=0; j<points.size(); j++) {
            ofVertex(points[j].x, points[j].y);
        }
        ofEndShape();
        
        
        ofNoFill();
        ofSetLineWidth(3);
        
        // draw approximate contour
        ofBeginShape();
        for (int j=0; j<fitPoints.size(); j++) {
            ofVertex(fitPoints[j].x, fitPoints[j].y);
        }
        ofEndShape();
        
        // draw rotated bounding rect
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255);
        ofTranslate(fitQuad.center.x, fitQuad.center.y);
        ofRotate(fitQuad.angle);
        ofRect(0, 0, fitQuad.size.width, fitQuad.size.height);
        ofPopStyle();
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

void Kinect::drawDebugSegmentation(){
    ofBackground(0);
    
    ofPushMatrix();
    kinect.draw(0, 0);
    ofTranslate(640, 0);
    kinect.drawDepth(0, 0);
    ofTranslate(-640, 480);
    grayImage.draw(0, 0);
    ofTranslate(640, 0);
    grayImage.draw(0, 0);
    //contourFinder.draw();
    
    RectTracker& tracker = contourFinder.getTracker();
    
    for(int i = 0; i < contourFinder.size(); i++) {
        // get contour, label, center point, and age of contour
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        // draw contours
        ofFill();
        ofSetColor(blobColors[label % 11]);
        ofBeginShape();
        for (int j=0; j<points.size(); j++) {
            ofVertex(points[j].x, points[j].y);
        }
        ofEndShape();
        
        ofNoFill();
        ofSetLineWidth(3);
        
        // draw approximate contour
        ofBeginShape();
        for (int j=0; j<fitPoints.size(); j++) {
            ofVertex(fitPoints[j].x, fitPoints[j].y);
        }
        ofEndShape();
        
        // draw rotated bounding rect
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255);
        ofTranslate(fitQuad.center.x, fitQuad.center.y);
        ofRotate(fitQuad.angle);
        ofRect(0, 0, fitQuad.size.width, fitQuad.size.height);
        ofPopStyle();
        ofPopMatrix();
        
    }
    ofPopMatrix();
}

void Kinect::drawDebug(){
    if (contourStrategy == BLOBS) {
        drawDebugBlobs();
    }
    else if (contourStrategy == SEGMENTATION) {
        drawDebugSegmentation();
    }
}

void Kinect::drawMask() {
    RectTracker& tracker = contourFinder.getTracker();
    for(int i = 0; i < contourFinder.size(); i++) {
        vector<cv::Point> points = contourFinder.getContour(i);
        ofBeginShape();
        ofFill();
        ofSetColor(255);
        for (int j=0; j<points.size(); j+=smoothness) {
            ofVec3f wp = kinect.getWorldCoordinateAt(points[j].x, points[j].y);
            ofVec2f pp = kpt.getProjectedPoint(wp);
            ofPoint mp(ofMap(pp.x, 0, 1, 0, ofGetWidth()),
                       ofMap(pp.y, 0, 1, 0, ofGetHeight()));
            if (curved) {
                ofCurveVertex(mp.x, mp.y);
            } else {
                ofVertex(mp.x, mp.y);
            }
        }
        ofEndShape();
    }
}

void Kinect::close() {
    kinect.close();
}