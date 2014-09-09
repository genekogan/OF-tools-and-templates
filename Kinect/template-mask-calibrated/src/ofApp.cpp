#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.setCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setTrackingBlobs(true);
    
    gfx.setup("render", ofGetScreenWidth(), 0, 1280, 800, true);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    kinect.drawDebug(0, 0);
    
    gfx.begin();
    
    ofClear(0, 0);
    kinect.beginMask(gfx.getWidth(), gfx.getHeight(), true);
    for (int i=0; i<200; i++) {
        ofFill();
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofEllipse(ofRandom(1024), ofRandom(768), ofRandom(200), ofRandom(200));
    }
    kinect.endMask();

    gfx.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
