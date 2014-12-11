#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // can add individual inputs
    learn.addInput("input1", &myVal1, 5, 8);
    learn.addInput("in2", -30, 30);
    learn.addInput("in3", 100, 200);
    
    // or can add groups of inputs
    learn.addInputGroup("myGroup");
    learn.addInputToGroup("myGroup", "myGroup-1", 4, 8);
    learn.addInputToGroup("myGroup", "myGroup-2", 0, 3);
    learn.addInputToGroup("myGroup", "myGroup-3", &myVal2, 1, 10);
    
    // add output
    learn.addOutput("myOutput", 0, 100);
    
    // setup osc
    learn.setupOscSender("localhost", 1234);
    learn.setupOscReceiver(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    learn.update();
    
    // variables can be bound to learn inputs
    myVal1 = ofMap(ofGetMouseX(), 0, ofGetWidth(), 5, 8);
    myVal2 = ofMap(ofGetMouseY(), 0, ofGetHeight(), 1, 10);
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='g')   learn.toggleVisible();
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
