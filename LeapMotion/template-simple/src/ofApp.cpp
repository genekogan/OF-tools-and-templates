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
    ofPoint p = leap.getFingerTip(LEFT, INDEX);
    
    // get fingert tips of a hand
    vector<ofPoint> rightHandTips = leap.getFingerTips(RIGHT);
    
    // "normalized" (true) means palm position subtracted from
    // each finger position, so hand always centered
    rightHandTips = leap.getFingerTips(RIGHT, true);
    for (int i=0; i<rightHandTips.size(); i++) {
        //cout << i << " = " << rightHandTips[i] << endl;
    }
    
    // hand orientation (left hand)
    leap.getHandDirection(LEFT);
    leap.getHandNormal(LEFT);
    leap.getHandPitch(LEFT);
    leap.getHandRoll(LEFT);
    leap.getHandYaw(LEFT);
    
    // velocity graphs
    ofPoint vl = leap.getHandVelocity(LEFT);
    ofPoint vr = leap.getHandVelocity(RIGHT);
    
    leap.drawVelocityGraph(LEFT, 700, 30, 300, 300);
    leap.drawVelocityGraph(RIGHT, 700, 380, 300, 300);
}

//-------
ofApp::~ofApp(){
    leap.close();
}

