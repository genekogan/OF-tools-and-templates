#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // KEYBOARD CONTROLS
    //  'c' => toggle calibration mode
    //
    // if in calibration mode:
    //  LEFT/RIGHT => switch active quad
    //  'S' => save calibration
    //  'L' => load calibration
    //
    // click-drag mouse on corners to move quad corners
    
    mapping.addQuad(640, 480);
    mapping.addQuad(640, 480);
    mapping.addQuad(640, 480);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    mapping.begin(0);
        ofSetColor(255, 0, 0);
        ofEllipse(250, 250, 100, 200);
    mapping.end(0);
    
    mapping.begin(1);
        ofSetColor(0, 255, 0);
        ofEllipse(250, 250, 100, 200);
    mapping.end(1);
    
    mapping.begin(2);
        ofSetColor(0, 0, 255);
        ofEllipse(250, 250, 100, 200);
    mapping.end(2);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='c')   mapping.toggleDebug();
    
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
