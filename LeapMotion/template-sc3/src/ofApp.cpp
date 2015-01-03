#include "ofApp.h"



//-------
void ofApp::setup(){
    leap.setup();

    leap.setVelocityTracking(true, 15);
    
    hand = leap.getRightHandPositionRef();
    
    synth = new ofxSCSynth("organ");
    switchMode(0);
    
    
//    organ, strum, sahliquid, ringMod
    f1z = 0.5;
    
    osc.setup("localhost", 57120);
}

//-------
void ofApp::update(){
    leap.update();
    
    
    if      (mode == 0) {
        
    }
    else if (mode == 1) {

    }
    else if (mode == 2) {
        
    }
    else if (mode == 3) {
        
    }
    else if (mode == 4) {
        
    }


    
    if (ofGetKeyPressed()) {
        f1z = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);
        synth->set("f1z", f1z);
    }
    else {
        synth->set("f1x", ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1));
        synth->set("f1y", ofMap(ofGetMouseY(), 0, ofGetHeight(), 0, 1));
    }
}

//-------
void ofApp::draw(){
    ofBackground(235);
    
    leap.draw(0, 120, 680, 560);
}

//-------
void ofApp::switchMode(int mode_){
    if (mode == mode_)  return;
    this->mode = mode_;
    synth->free();

    if      (mode == 0)    synth = new ofxSCSynth("organ");
    else if (mode == 1)    synth = new ofxSCSynth("strum");
    else if (mode == 2)    synth = new ofxSCSynth("sahliquid");
    else if (mode == 3)    synth = new ofxSCSynth("ringMod");
    
    synth->create();

//    else if (mode == 4)    synth = new ofxSCSynth("strum");
}

//-------
void ofApp::keyPressed(int key){
    if      (key=='1')   switchMode(0);
    else if (key=='2')   switchMode(1);
    else if (key=='3')   switchMode(2);
    else if (key=='4')   switchMode(3);
    else if (key=='5')   switchMode(4);
}

//-------
ofApp::~ofApp(){
    synth->free();
    leap.close();
}

