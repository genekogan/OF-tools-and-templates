#include "ofApp.h"

//-----------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    
    syphon = new Syphon();
    syphon->setup(400, 400);
    ((Syphon *) syphon)->setClient("Processing Syphon", "SendFrames");
}

//-----------
void ofApp::update(){
}

//-----------
void ofApp::draw() {
    if (syphon->getActive())
        syphon->draw(0, 0);
}

//-----------
void ofApp::keyPressed(int key){
}