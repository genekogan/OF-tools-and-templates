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
    fbo.allocate(640, 480);
    
    visual = new Amoeba();
    ((Scene *) visual)->setup(640, 480);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
        visual->draw(0, 0);
    fbo.end();
    
    mapping.begin(0);
        fbo.draw(0, 0);
    mapping.end(0);
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
