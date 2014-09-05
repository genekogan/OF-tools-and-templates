#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    kinect.setTrackingBlobs(true);
    
    fbo.allocate(1024, 768);
    fbo.begin();
    ofBackground(0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofEllipse(ofRandom(1024), ofRandom(768), ofRandom(200), ofRandom(200));
    }
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    /* can either mask a drawable, e.g. */
    //kinect.drawMask(tex);

    
    /* or draw inside mask directly */
    
    kinect.beginMask(ofGetWidth(), ofGetHeight());
    
    ofBackground(0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofEllipse(ofRandom(1024), ofRandom(768), ofRandom(200), ofRandom(200));
    }
    
    kinect.endMask();
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
