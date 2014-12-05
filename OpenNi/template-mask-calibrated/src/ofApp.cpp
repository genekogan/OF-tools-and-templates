#include "ofApp.h"

//--------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    gfx.setup("main", ofGetScreenWidth(), 0, 1280, 800, true);

    kinect.setup();
    //kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
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
    if (key=='g')   kinect.toggleVisible();
}