#include "ofApp.h"

//--------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);

    //kinect.setup();
    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.start();
}

//--------
void ofApp::update(){
    kinect.update();
}

//--------
void ofApp::draw(){
    kinect.draw();
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