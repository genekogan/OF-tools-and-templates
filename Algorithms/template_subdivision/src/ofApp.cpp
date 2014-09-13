#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    vector<ofPoint> points;
    points.push_back(ofPoint(0, 0));
    points.push_back(ofPoint(ofGetWidth(), 0));
    points.push_back(ofPoint(ofGetWidth(), ofGetHeight()));
    points.push_back(ofPoint(0, ofGetHeight()));
    
    poly.setup(points, ofColor(127, 127, 127));
    
    int n = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 20);
    poly.subdivide(n);
    
    ofBackground(255);
    
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
