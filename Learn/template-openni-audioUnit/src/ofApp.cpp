#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);

    // setup audio
    audio.setupInstrument(Instrument::AALTO);
    audio.setGuiPosition(265, 420);
    audio.start();
    
    // setup audio unit
    output.setupLearn(&learn);
    output.setupAudioUnit(&audio.getInstrument().getAudioUnit());
    output.setGuiPosition(1030, 5);
    
    // add inputs to learn
    learn.setGuiPosition(275, 70);
    learn.toggleGuiSummaryView();
    learn.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");

    // inputs (manually)
    //learn.addJointsAsInput();
    
    // osc
    learn.setupOscSender("localhost", 1234);
    learn.setupOscReceiver(8000);
}

//--------------------------------------------------------------
void ofApp::update(){
    output.update();
    audio.update();
    learn.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    output.draw();
    audio.draw();
    learn.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if      (key=='!')  learn.toggleGuiSummaryView();
/*
    if      (key=='!')  learn.toggleVisible();
    else if (key=='@')  learn.toggleOpenNiVisible();
    else if (key=='#')  learn.toggleGuiInputsVisible();
 */
    else if (key=='$')  audio.toggleVisible();
    
    else if (key=='z')  output.resetParameterMappings();
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
