#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    canvas1.setup(600, 796);
    canvas1.addLayer(CANVAS_CREATOR);
    canvas1.addLayer(CANVAS_POST_FX);
    
    canvas2.setup(800, 600);
    canvas2.addLayer(CANVAS_CREATOR);
    canvas2.addLayer(CANVAS_POST_GLITCH);
    
    canvas1.setVisible(true);
    canvas2.setVisible(false);

    window.setup("projection", ofGetScreenWidth(), 0, 1280, 800, true);
    mapping.setMappingResolution(ofGetWidth(), ofGetHeight(),
                                 window.getWidth(), window.getHeight());
    mapping.addCanvas(&canvas1);
    mapping.addCanvas(&canvas2);
}

//--------------------------------------------------------------
void ofApp::update(){
    canvas1.update();
    canvas2.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (mapping.getDebug()) {
        mapping.drawDebug();
    }

    window.begin();
    ofClear(0, 0);
    mapping.draw();
    window.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='1') {
        canvas1.setVisible(true);
        canvas2.setVisible(false);
    }
    else if (key=='2') {
        canvas1.setVisible(false);
        canvas2.setVisible(true);
    }
    else if (key=='3') {
        canvas1.setVisible(false);
        canvas2.setVisible(false);
    }

    if (key=='c') {
        mapping.toggleDebug();
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
