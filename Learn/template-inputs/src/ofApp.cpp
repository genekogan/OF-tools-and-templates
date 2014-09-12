#include "ofApp.h"

//----------
void ofApp::setup() {
    kinect.setup();
}

//----------
void ofApp::update(){
    kinect.update();
}

//----------
void ofApp::draw(){
    kinect.draw();
}

//----------
void ofApp::exit(){
    kinect.close();
}

//----------
void ofApp::keyPressed(int key){
    
}