#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    //window.setup("projection", ofGetScreenWidth()-500, 0, 1280, 800, false);
    window.setup("projection", ofGetScreenWidth(), 0, 1280, 800, true);
    
    canvas.setup(window.getWidth(), window.getHeight());
    canvas.addLayer(CANVAS_CREATOR);
}

//-----------
void ofApp::update(){
    canvas.update();
}

//-----------
void ofApp::draw() {
    ofBackground(0);
    
    window.begin();
    canvas.draw(0, 0);
    window.end();
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
}