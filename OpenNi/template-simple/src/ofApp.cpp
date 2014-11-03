#include "ofApp.h"

//---------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    kinect.setup("/Users/Gene/Downloads/hometest_single.oni");
}

//---------
void ofApp::update(){
    kinect.update();
}

//---------
void ofApp::draw() {
    kinect.draw();
}

//---------
void ofApp::exit(){
    kinect.close();
}

//---------
void ofApp::keyPressed(int key){
    
}