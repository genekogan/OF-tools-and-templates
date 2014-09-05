#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    kinect.setup();
    kinect.setGuiPosition(825, 5);
    kinect.setTrackingBlobs(true);
    
    fbo.allocate(1024, 768);
    fbo.begin();
    ofBackground(0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofEllipse(ofRandom(1024), ofRandom(768), ofRandom(200), ofRandom(200));
    }
    fbo.end();
    
    canvas.setup(1024, 768);
    canvas.addLayer(CANVAS_CREATOR);
    canvas.addLayer(CANVAS_POST_FX);
}

//-----------
void ofApp::update(){
    kinect.update();
    canvas.update();
}

//-----------
void ofApp::draw() {
    canvas.draw(0, 0);
    
    kinect.beginMask(ofGetWidth(), ofGetHeight());
    ofBackground(0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofEllipse(ofRandom(1024), ofRandom(768), ofRandom(200), ofRandom(200));
    }
    kinect.endMask();
}

//-----------
void ofApp::keyPressed(int key){
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
}