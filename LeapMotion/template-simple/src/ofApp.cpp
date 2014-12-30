#include "ofApp.h"



//-------
void ofApp::setup(){
    leap.setup();
    
    // enable this to track velocity.
    // second argument refers to frame size of smoothing/averaging window (default 10)
    leap.setVelocityTracking(true, 15);
}

//-------
void ofApp::update(){
    leap.update();
}

//-------
void ofApp::draw(){
    ofBackground(235);
    
    leap.draw(0, 120, 680, 560);
    
    // get tip of a specific finger
    ofPoint *p = leap.getRightHandFingerTipRef(1);
    
    // get fingert tips of a hand
    vector<ofPoint> *rightHandTips = leap.getRightHandFingerTipsRef();
    
    // hand orientation (left hand)
    leap.getLeftHandDirectionRef();
    leap.getLeftHandNormalRef();
    leap.getLeftHandPitchRef();
    leap.getLeftHandRollRef();
    leap.getLeftHandYawRef();
    
    // velocity graphs
    ofPoint *vl = leap.getLeftHandVelocityRef();
    ofPoint *vr = leap.getRightHandVelocityRef();
    
    leap.drawVelocityGraph(LEFT, 700, 30, 300, 300);
    leap.drawVelocityGraph(RIGHT, 700, 380, 300, 300);
}

//-------
ofApp::~ofApp(){
    leap.close();
}

