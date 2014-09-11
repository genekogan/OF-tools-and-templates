#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    myVal.setName("ofxparam");
    myVal.setMin(2.0);
    myVal.setMax(5.0);
    
    learn.setup();
    
    learn.addInput("hello", 1, 5);
    learn.addInput(&myVal);
    
    learn.addOutput("this", 10, 20);
    learn.addOutput("that", 1, 10);
    learn.addOutput("other", 1, 10);

}

//--------------------------------------------------------------
void ofApp::update(){
    learn.update();
    
    myVal = ofMap(mouseX, 0, ofGetWidth(), 2, 5);
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
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
