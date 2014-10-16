#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aalto.setup();
    
    ofSetFrameRate(30);


}

//--------------------------------------------------------------
void ofApp::update(){
    aalto.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    aalto.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key=='1') {
        aalto.savePreset();
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
