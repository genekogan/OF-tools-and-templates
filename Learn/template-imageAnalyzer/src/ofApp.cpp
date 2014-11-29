#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    learn.setGuiPosition(275, 70);
    learn.toggleGuiSummaryView();

    // add inputs
    //learn.addPositionAsInput();
    
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
    if (key=='g') {
        learn.toggleVisible();
    }
    if (key=='!') {
        learn.toggleGuiSummaryView();
    }

}
