#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);

    // setup audio
    audio.setupInstrument(Instrument::AALTO);
    audio.setGuiPosition(270, 440);
    audio.start();
    
    // setup audio unit
    output.setupLearn(&learn);
    output.setupAudioUnit(&audio.getInstrument().getAudioUnit());
    output.setGuiPosition(1030, 5);
    
    // add inputs to learn
    //learn.setGuiPosition(275, 70);
    learn.setGuiPosition(5, 255);
    learn.toggleGuiSummaryView();
    learn.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");

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
    else if (key=='@')  audio.toggleVisible();
    else if (key=='z')  output.resetParameterMappings();
}
