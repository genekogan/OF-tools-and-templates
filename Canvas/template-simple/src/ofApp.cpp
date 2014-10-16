#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    canvas.setup(1024, 768);
    canvas.addLayer(CANVAS_CREATOR);
    //canvas.addLayer(CANVAS_POST_FX);
}

//-----------
void ofApp::update(){
    canvas.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//-----------
void ofApp::draw() {
    canvas.draw(0, 0);
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
}