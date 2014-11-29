#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    learn.setGuiPosition(275, 70);
    learn.toggleGuiSummaryView();
    learn.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");

    // decide which openni inputs are available
    learn.addJointsAsInput();
    learn.addNormalizedJointsAsInput();
    learn.addRelativeJointsAsInput();
    learn.addVelocityJointsAsInput();
    learn.addAccelerationJointsAsInput();
    
    learn.addRelativeDistanceJointsAsInput();
    learn.addVelocityMagnitudeJointsAsInput();
    learn.addVelocityMeanJointsAsInput();
    learn.addAccelerationMagnitudeJointsAsInput();
    learn.addAccelerationMeanJointsAsInput();
    learn.addAccelerationTrajectoryJointsAsInput();
 
    learn.addSymmetryAsInput();
    learn.addQomAsInput();
    learn.addCiAsInput();
    learn.addDepthAsInput();
    learn.addYMaxHandsAsInput();

    // add outputs
    learn.addOutput("output1", 20, 50);

    // osc
    learn.setupOscSender("localhost", 1234);
    learn.setupOscReceiver(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    learn.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if      (key=='!') {
        learn.toggleGuiSummaryView();
    }
}
