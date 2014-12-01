#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    vector<ofPoint> bounds;
    bounds.push_back(ofPoint(0, 0));
    bounds.push_back(ofPoint(ofGetWidth(), 0));
    bounds.push_back(ofPoint(ofGetWidth(), ofGetHeight()));
    bounds.push_back(ofPoint(0, ofGetHeight()));
    
    poly.setup(bounds, ofColor(127, 127, 127));
    
    poly.subdivide(8);
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    poly.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key==' ')   setup();
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
