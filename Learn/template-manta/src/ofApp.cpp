#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    learn.addAllPadsAsInputs();
    learn.addOutput("output1", 20, 50);

    learn.setupOscSender("localhost", 1234);
    learn.setupOscReceiver(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    learn.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='g')
        learn.toggleVisible();
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
