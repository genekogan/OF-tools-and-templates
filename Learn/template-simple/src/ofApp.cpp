#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    learn.addInput("input1", &myVal, 5, 8);
    learn.addInput("in2", -30, 30);
    learn.addInput("in3", 100, 200);
    
    learn.addOutput("output1", 20, 50);

    learn.setupOscSender("localhost", 1234);
    learn.setupOscReceiver(555);
}

//--------------------------------------------------------------
void ofApp::update(){
    learn.update();
    
    myVal = ofMap(ofGetMouseX(), 0, ofGetWidth(), 5, 8);
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='g')
        learn.toggleVisible();
    if (key==' ') {
        learn.blah();
    }
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
