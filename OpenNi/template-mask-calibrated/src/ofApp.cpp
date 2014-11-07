#include "ofApp.h"

//--------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    gfx.setup("main", ofGetScreenWidth()-640, 20, 1024, 768, false);

    kinect.setup("/Users/Gene/Downloads/hometest_single.oni");
    kinect.enableContourTracking();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setupContourVisuals(gfx.getWidth(), gfx.getHeight());
    kinect.start();
}

//--------
void ofApp::update(){
    kinect.update();
}

//--------
void ofApp::draw(){
    kinect.draw();
    
    gfx.begin();
    ofClear(0, 0);
    kinect.drawVisuals();
    gfx.end();
}

//--------
void ofApp::exit(){
    kinect.stop();
    kinect.close();
}

//--------
void ofApp::keyPressed(int key){
    if (key=='g')   kinect.toggleGuiVisible();
}