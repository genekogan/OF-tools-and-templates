#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aalto.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    aalto.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='q') {
        aalto.noteOn(60, 90);
    }
    else if (key=='w') {
        aalto.noteOff(60, 90);
    }
    
    if (key=='1') {
        aalto.savePreset();
    }
    /*
    if (key=='2') {
        aalto.loadPreset("thistestpr");
    }
     */
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
