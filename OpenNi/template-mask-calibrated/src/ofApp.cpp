#include "ofApp.h"

//--------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //kinect.setup("/Users/Gene/Downloads/hometest_single.oni");
    kinect.setup();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    //kinect.setMaxUsers(2);

    gfx.setup("main", ofGetScreenWidth()/4, 0, 1280, 800, false);
}

//--------
void ofApp::update(){
    kinect.update();
}

//--------
void ofApp::draw(){
    kinect.draw();
    
    gfx.begin();
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255);
    kinect.drawCalibratedContours(gfx.getWidth(), gfx.getHeight());
    ofDisableBlendMode();
    gfx.end();
}

//--------
void ofApp::exit(){
    kinect.close();
}

//--------
void ofApp::keyPressed(int key){
    
}