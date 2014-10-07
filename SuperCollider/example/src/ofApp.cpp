#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    string synthFile = "/Users/Gene/Code/openFrameworks/tools/SuperCollider/synths.scd";
    SuperColliderLayer *layer1 = sc3.addLayer("source", synthFile);
    SuperColliderLayer *layer2 = sc3.addLayer("modifier", synthFile);
    layer1->setBusOutSelf();
    layer2->setBusIn(layer1->getBus());
    layer2->setBusOutToDac();
}

//--------------------------------------------------------------
void ofApp::update(){
    sc3.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

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
